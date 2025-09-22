#include "pch.h"
#include "framework.h"
#include "StayAwake.h"
#include "StayAwakeDlg.h"
#include "StayAwakeAboutDlg.h"
#include "afxdialogex.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CStayAwakeDlg::CStayAwakeDlg(CWnd* pParent /*=nullptr*/)
   : CDialogEx(IDD_STAYAWAKE_DIALOG, pParent)
{
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CStayAwakeDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CStayAwakeDlg, CDialogEx)
   ON_WM_SYSCOMMAND()
   ON_MESSAGE(WM_POST_OPEN, &CStayAwakeDlg::OnPostOpen)
   ON_MESSAGE(WM_TRAY_NOTIFY, &CStayAwakeDlg::OnTrayNotify)
   ON_REGISTERED_MESSAGE(WM_RESTORE_DIALOG, &CStayAwakeDlg::OnRestoreDialog)
   ON_COMMAND(IDCANCEL, &CStayAwakeDlg::OnCancel)
   ON_COMMAND(IDM_RESTORE, &CStayAwakeDlg::OnRestore)
   ON_COMMAND(IDM_EXIT, &CStayAwakeDlg::OnExit)
   ON_WM_DESTROY()
   ON_WM_TIMER()
   ON_BN_CLICKED(IDC_MINIMIZE, &CStayAwakeDlg::OnMinimize)
   ON_BN_CLICKED(IDC_STAYAWAKE_SET_INTERVAL_BTN, &CStayAwakeDlg::OnSetInterval)
   ON_EN_KILLFOCUS(IDC_STAYAWAKE_INTERVAL, &CStayAwakeDlg::OnKillfocusInterval)
   ON_BN_CLICKED(IDC_ABOUT_BUTTON, &CStayAwakeDlg::OnClickedAboutButton)
   ON_CBN_SELCHANGE(IDC_STAYAWAKE_KEY_LIST, &CStayAwakeDlg::OnStayawakeKeyChange)
   ON_BN_CLICKED(IDC_STAYAWAKE_PAUSE_RESUME_BTN, &CStayAwakeDlg::OnPauseResume)
END_MESSAGE_MAP()


// CStayAwakeDlg message handlers

BOOL CStayAwakeDlg::OnInitDialog()
{
   CDialogEx::OnInitDialog();

   // Add "About..." menu item to system menu.

   // IDM_ABOUTBOX must be in the system command range.
   ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
   ASSERT(IDM_ABOUTBOX < 0xF000);

   CMenu* pSysMenu = GetSystemMenu(FALSE);
   if (pSysMenu != nullptr)
   {
      BOOL bNameValid;
      CString strAboutMenu;
      bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
      ASSERT(bNameValid);
      if (!strAboutMenu.IsEmpty())
      {
         pSysMenu->AppendMenu(MF_SEPARATOR);
         pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
      }
   }

   // Set the icon for this dialog.  The framework does this automatically
   //  when the application's main window is not a dialog
   SetIcon(m_hIcon, TRUE);			// Set big icon
   SetIcon(m_hIcon, FALSE);		// Set small icon

   // Init KeyCodes List
   SendDlgItemMessage(IDC_STAYAWAKE_KEY_LIST, CB_ADDSTRING, NULL, (LPARAM)L"Scroll Lock cycling");
   SendDlgItemMessage(IDC_STAYAWAKE_KEY_LIST, CB_ADDSTRING, NULL, (LPARAM)L"Volume Down & Up");

   for (int i{ 1 }; i <= 10; i++) {
      SendDlgItemMessage(IDC_STAYAWAKE_KEY_LIST, CB_ADDSTRING, NULL, (LPARAM)(L"Unassigned Key #" + to_wstring(i)).c_str());
   }

   m_AwakeKeyCode = GetPrivateProfileInt(PREF_DEFAULTS, PREF_AWAKE_KEYCODE, m_TimerSeconds, PREF_INI_FILE);
   m_AwakeKeyCode %= 12;

   SendDlgItemMessage(IDC_STAYAWAKE_KEY_LIST, CB_SETCURSEL, m_AwakeKeyCode, NULL);

   // Init Timer Seconds
   m_TimerSeconds = GetPrivateProfileInt(PREF_DEFAULTS, PREF_TIMER_INTERVAL, m_TimerSeconds, PREF_INI_FILE);
   if (m_TimerSeconds < MIN_PERIOD || m_TimerSeconds > MAX_PERIOD)
      m_TimerSeconds = 240;
   SetDlgItemInt(IDC_STAYAWAKE_INTERVAL, m_TimerSeconds, FALSE);

   Utils::addTooltip(theApp.m_hInstance, m_hWnd, IDC_STAYAWAKE_INTERVAL, L"",
      wstring{ L"Number between " } + to_wstring(MIN_PERIOD) + L" and " + to_wstring(MAX_PERIOD), 3, TRUE);

   SetDlgItemText(IDC_STAYAWAKE_PAUSE_RESUME_BTN, IsTimerPaused() ? BTN_TEXT_RESUME : BTN_TEXT_PAUSE);

   Utils::addTooltip(theApp.m_hInstance, m_hWnd, IDC_ABOUT_BUTTON, L"", L"About StayAwake", 3, TRUE);
   Utils::loadBitmap(theApp.m_hInstance, m_hWnd, IDC_ABOUT_BUTTON, IDB_ABOUT_BITMAP);

   InitTrayIcon();
   PostMessage(WM_POST_OPEN, 0, 0);

   return TRUE;  // return TRUE  unless you set the focus to a control
}

void CStayAwakeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
   if ((nID & 0xFFF0) == IDM_ABOUTBOX)
   {
      CAboutDlg dlgAbout;
      dlgAbout.DoModal();
   }
   else
   {
      CDialogEx::OnSysCommand(nID, lParam);
   }
}


afx_msg LRESULT CStayAwakeDlg::OnPostOpen(WPARAM wParam, LPARAM lParam)
{
   if (GetPreference(PREF_MULTI_INSTANCE, L"N") != L"Y" && Utils::getProcessRunCount(L"StayAwake.exe") > 1)
   {
      ::PostMessage(HWND_BROADCAST, theApp.WM_SHOWFIRSTINSTANCE, 0, 0);

      DestroyWindow();
      return 0;
   }

   if (IsTimerPaused())
      ShowPausedInfo(TRUE);
   else
      InitTimer();

   return 0;
}


afx_msg LRESULT CStayAwakeDlg::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
   if (wParam != 1) return 0;

   CPoint pt{};

   switch (lParam)
   {
   case WM_LBUTTONDOWN:
   case WM_RBUTTONDOWN:
   case WM_CONTEXTMENU:
      GetCursorPos(&pt);
      OnTrayButtonDown(pt);
      break;
   }

   return 0;
}

afx_msg LRESULT CStayAwakeDlg::OnRestoreDialog(WPARAM wParam, LPARAM lParam)
{
   RestoreFromTray();
   SetForegroundWindow();
   return 0;
}


void CStayAwakeDlg::OnKillfocusInterval()
{
   int nInterval{};

   nInterval = GetDlgItemInt(IDC_STAYAWAKE_INTERVAL, nullptr, FALSE);

   if (nInterval < MIN_PERIOD || nInterval > MAX_PERIOD)
   {
      Utils::showEditBalloonTip(GetDlgItem(IDC_STAYAWAKE_INTERVAL)->m_hWnd, L"Timer Interval in seconds",
         (wstring{ L"Please enter a value between " } + to_wstring(MIN_PERIOD) + L" and " + to_wstring(MAX_PERIOD)).c_str());
      SetDlgItemInt(IDC_STAYAWAKE_INTERVAL, m_TimerSeconds, FALSE);
      return;
   }

   m_TimerSeconds = nInterval;
}


void CStayAwakeDlg::OnTimer(UINT_PTR nIDEvent)
{
   SimulateAwakeKeyPress();
   CDialogEx::OnTimer(nIDEvent);
}


void CStayAwakeDlg::InitTimer()
{
   SimulateAwakeKeyPress();
   m_TimerID = SetTimer(m_TimerID, m_TimerSeconds * 1000, NULL);

   SetDlgItemText(IDC_STAYAWAKE_PAUSE_RESUME_BTN, BTN_TEXT_PAUSE);
   WritePrivateProfileString(PREF_DEFAULTS, PREF_AWAKE_PAUSED, L"N", PREF_INI_FILE);
}

void CStayAwakeDlg::InitTrayIcon()
{
   m_TrayData.cbSize = sizeof(NOTIFYICONDATA);
   m_TrayData.hWnd = this->m_hWnd;
   m_TrayData.uID = 1;
   m_TrayData.uCallbackMessage = WM_TRAY_NOTIFY;
   m_TrayData.hIcon = this->m_hIcon;

   StrCpy(m_TrayData.szTip, L"StayAwake");
   m_TrayData.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;

   if (!m_menu.LoadMenu(IDR_MENU1))
      MessageBox(L"Unable to Init Tray Icon", L"Error!");
}


void CStayAwakeDlg::MinimizeToTray()
{
   if (m_bMinimized) return;
   SendDlgItemMessage(IDC_STAYAWAKE_INTERVAL, EM_HIDEBALLOONTIP, 0, 0);

   if (!Shell_NotifyIcon(NIM_ADD, &m_TrayData))
      MessageBox(L"Unable to Display Tray Icon", L"Error!");

   this->ShowWindow(SW_MINIMIZE);
   this->ShowWindow(SW_HIDE);
   m_bMinimized = true;
}


void CStayAwakeDlg::OnTrayButtonDown(CPoint pt)
{
   m_menu.GetSubMenu(0)->TrackPopupMenu(TPM_BOTTOMALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, pt.x, pt.y, this);
}


void CStayAwakeDlg::RestoreFromTray()
{
   Shell_NotifyIcon(NIM_DELETE, &m_TrayData);

   this->ShowWindow(SW_RESTORE);
   this->ShowWindow(SW_SHOW);
   m_bMinimized = false;
}


void CStayAwakeDlg::OnMinimize()
{
   MinimizeToTray();
}


void CStayAwakeDlg::OnCancel()
{
   MinimizeToTray();
}


void CStayAwakeDlg::OnRestore()
{
   RestoreFromTray();
}


void CStayAwakeDlg::OnExit()
{
   DestroyWindow();
}


void CStayAwakeDlg::OnDestroy()
{
   if (m_bMinimized) Shell_NotifyIcon(NIM_DELETE, &m_TrayData);
   m_menu.DestroyMenu();

   CDialogEx::OnDestroy();
}


void CStayAwakeDlg::SimulateAwakeKeyPress()
{
   switch (m_AwakeKeyCode) {
   case 1:
      keybd_event(VK_VOLUME_DOWN, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_VOLUME_DOWN, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      Sleep(10);
      keybd_event(VK_VOLUME_UP, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_VOLUME_UP, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      break;

   case 2:
   case 3:
   case 4:
   case 5:
   case 6:
   case 7:
   case 8:
   case 9:
   case 10:
   {
      BYTE keycode{ static_cast<BYTE>(VK_UNASSIGNED_01 + m_AwakeKeyCode - 2) };
      keybd_event(keycode, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(keycode, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      break;
   }

   case 11:
      keybd_event(VK_UNASSIGNED_10, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_UNASSIGNED_10, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      break;

   default:
      keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      Sleep(10);
      keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      break;
   }

   SYSTEMTIME lastTime{};
   GetLocalTime(&lastTime);
   SetDlgItemText(IDC_STAYAWAKE_LAST_EVENT, Utils::formatSystemTime(lastTime, L"Last StayAwake event").c_str());

   SYSTEMTIME nextTime{};
   GetSystemTime(&nextTime);
   Utils::addSecondsToTime(nextTime, m_TimerSeconds);
   SetDlgItemText(IDC_STAYAWAKE_NEXT_EVENT, Utils::formatSystemTime(nextTime, L"Next StayAwake event").c_str());
}


void CStayAwakeDlg::OnSetInterval()
{
   OnKillfocusInterval();
   WritePrivateProfileString(PREF_DEFAULTS, PREF_TIMER_INTERVAL, to_wstring(m_TimerSeconds).c_str(), PREF_INI_FILE);
   InitTimer();
}


void CStayAwakeDlg::OnClickedAboutButton()
{
   CAboutDlg dlgAbout;
   dlgAbout.DoModal();
}

void CStayAwakeDlg::OnStayawakeKeyChange()
{
   m_AwakeKeyCode = static_cast<int>(SendDlgItemMessage(IDC_STAYAWAKE_KEY_LIST, CB_GETCURSEL, 0, 0));
   m_AwakeKeyCode %= 12;
   WritePrivateProfileString(PREF_DEFAULTS, PREF_AWAKE_KEYCODE, to_wstring(m_AwakeKeyCode).c_str(), PREF_INI_FILE);
}

void CStayAwakeDlg::OnPauseResume()
{
   if (IsTimerPaused()) {
      InitTimer();
   }
   else {
      KillTimer(m_TimerID);

      SetDlgItemText(IDC_STAYAWAKE_PAUSE_RESUME_BTN, BTN_TEXT_RESUME);
      WritePrivateProfileString(PREF_DEFAULTS, PREF_AWAKE_PAUSED, L"Y", PREF_INI_FILE);
      ShowPausedInfo(false);
   }
}

wstring CStayAwakeDlg::GetPreference(wstring key, wstring defaultVal)
{
   const int bufSize{ MAX_PATH };
   wstring ftBuf(bufSize, '\0');

   GetPrivateProfileString(PREF_DEFAULTS, key.c_str(), defaultVal.c_str(), ftBuf.data(), bufSize, PREF_INI_FILE);

   return wstring{ ftBuf.c_str() };
}

bool CStayAwakeDlg::IsTimerPaused()
{
   return (GetPreference(PREF_AWAKE_PAUSED, L"N") == L"Y");
}

void CStayAwakeDlg::ShowPausedInfo(bool both)
{
   if (both)
      SetDlgItemText(IDC_STAYAWAKE_LAST_EVENT, L"Last StayAwake event:         PAUSED");

   SetDlgItemText(IDC_STAYAWAKE_NEXT_EVENT, L"Next StayAwake event:         PAUSED");
}
