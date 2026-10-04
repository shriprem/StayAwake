#include "pch.h"
#include "framework.h"
#include "afxdialogex.h"

#include "StayAwake.h"
#include "StayAwakeDlg.h"
#include "SelectKeyCodesDlg.h"
#include "StayAwakeAboutDlg.h"
#include "VersionUpdate.h"
#include "Utils.h"

#include <PathCch.h>
#pragma comment(lib, "Pathcch.lib")

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
   ON_COMMAND(IDOK, &CStayAwakeDlg::OnEnter)
   ON_COMMAND(IDCANCEL, &CStayAwakeDlg::OnCancel)
   ON_COMMAND(IDM_RESTORE, &CStayAwakeDlg::OnRestore)
   ON_COMMAND(IDM_EXIT, &CStayAwakeDlg::OnExit)
   ON_WM_DESTROY()
   ON_WM_TIMER()
   ON_BN_CLICKED(IDC_MINIMIZE_BTN, &CStayAwakeDlg::OnMinimize)
   ON_BN_CLICKED(IDC_EXIT_BTN, &CStayAwakeDlg::OnExit)
   ON_BN_CLICKED(IDC_SELECT_INPUT_OPTIONS_BTN, &CStayAwakeDlg::OnSelectInputOptionsClicked)
   ON_BN_CLICKED(IDC_SET_TIMER_BTN, &CStayAwakeDlg::OnSetTimerClicked)
   ON_EN_KILLFOCUS(IDC_INTERVAL_MIN_EDIT, &CStayAwakeDlg::OnKillfocusIntervalMin)
   ON_EN_KILLFOCUS(IDC_INTERVAL_MAX_EDIT, &CStayAwakeDlg::OnKillfocusIntervalMax)
   ON_BN_CLICKED(IDC_ABOUT_BTN, &CStayAwakeDlg::OnClickedAboutButton)
   ON_BN_CLICKED(IDC_PAUSE_RESUME_BTN, &CStayAwakeDlg::OnPauseResumeClicked)
   ON_BN_CLICKED(IDC_START_MINIMIZED_CBX, &CStayAwakeDlg::OnStartMinimized)
   ON_WM_WTSSESSION_CHANGE()
   ON_BN_CLICKED(IDC_UPDATE_CHECK_BTN, &CStayAwakeDlg::OnUpdateCheckClicked)
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

   InitConfigFilePath();

   m_AwakeCore.InitIntervals(m_IntervalMinSeconds, m_IntervalMaxSeconds);
   SetDlgItemInt(IDC_INTERVAL_MIN_EDIT, m_IntervalMinSeconds, FALSE);
   SetDlgItemInt(IDC_INTERVAL_MAX_EDIT, m_IntervalMaxSeconds, FALSE);

   Utils::addTooltip(theApp.m_hInstance, m_hWnd, IDC_INTERVAL_MIN_EDIT, L"", INTERVAL_TOOLTIP, 3, TRUE);
   Utils::addTooltip(theApp.m_hInstance, m_hWnd, IDC_INTERVAL_MAX_EDIT, L"", INTERVAL_TOOLTIP, 3, TRUE);

   SetDlgItemText(IDC_PAUSE_RESUME_BTN, IsTimerPaused() ? BTN_TEXT_RESUME : BTN_TEXT_PAUSE);

   Utils::addTooltip(theApp.m_hInstance, m_hWnd, IDC_ABOUT_BTN, L"", L"About StayAwake", 3, TRUE);
   Utils::loadBitmap(theApp.m_hInstance, m_hWnd, IDC_ABOUT_BTN, IDB_ABOUT_BITMAP);

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
   WTSRegisterSessionNotification(m_hWnd, NOTIFY_FOR_THIS_SESSION);

   if (m_AwakeCore.GetPreference(PREF_MULTI_INSTANCE, L"N") != L"Y" && Utils::getProcessRunCount(L"StayAwake.exe") > 1)
   {
      ::PostMessage(HWND_BROADCAST, theApp.WM_SHOWFIRSTINSTANCE, 0, 0);

      DestroyWindow();
      return 0;
   }

   if (IsTimerPaused())
      ShowPausedInfo(TRUE);
   else
      InitAwakes();

   BOOL bMinimized = (m_AwakeCore.GetPreference(PREF_START_MINIMIZED, L"N") == L"Y");
   CheckDlgButton(IDC_START_MINIMIZED_CBX, bMinimized ? BST_CHECKED : BST_UNCHECKED);
   if (bMinimized) MinimizeToTray();

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


void CStayAwakeDlg::OnKillfocusIntervalMin()
{
   int nInterval{};

   nInterval = GetDlgItemInt(IDC_INTERVAL_MIN_EDIT, nullptr, FALSE);

   if (nInterval < MIN_PERIOD || nInterval > MAX_PERIOD)
   {
      Utils::showEditBalloonTip(GetDlgItem(IDC_INTERVAL_MIN_EDIT)->m_hWnd, INTERVAL_WARN_TITLE, INTERVAL_WARNING.c_str());
      SetDlgItemInt(IDC_INTERVAL_MIN_EDIT, m_IntervalMinSeconds, FALSE);
      return;
   }

   m_IntervalMinSeconds = nInterval;
}


void CStayAwakeDlg::OnKillfocusIntervalMax()
{
   int nInterval{};

   nInterval = GetDlgItemInt(IDC_INTERVAL_MAX_EDIT, nullptr, FALSE);

   if (nInterval < MIN_PERIOD || nInterval > MAX_PERIOD)
   {
      Utils::showEditBalloonTip(GetDlgItem(IDC_INTERVAL_MAX_EDIT)->m_hWnd, INTERVAL_WARN_TITLE, INTERVAL_WARNING.c_str());
      SetDlgItemInt(IDC_INTERVAL_MAX_EDIT, m_IntervalMaxSeconds, FALSE);
      return;
   }

   m_IntervalMaxSeconds = nInterval;
}


void CStayAwakeDlg::OnSelectInputOptionsClicked()
{
   CSelectKeyCodesDlg dlgSelectKeyCodes(this, m_AwakeCore);

   if (dlgSelectKeyCodes.DoModal() == IDOK)
   {
      InitRosterKeyCodes();
      if (!IsTimerPaused()) SimulateAwakeKeyPress();
   }
}

void CStayAwakeDlg::OnTimer(UINT_PTR nIDEvent)
{
   SimulateAwakeKeyPress();
   CDialogEx::OnTimer(nIDEvent);
}


void CStayAwakeDlg::InitConfigFilePath()
{
   wchar_t sIniFilePath[MAX_PATH]{};
   DWORD size = GetModuleFileNameW(nullptr, sIniFilePath, MAX_PATH);

   if (size <= 0 || size > MAX_PATH ||
      FAILED(PathCchRemoveFileSpec(sIniFilePath, MAX_PATH)) ||
      FAILED(PathCchAppend(sIniFilePath, MAX_PATH, PREF_INI_FILE)))
   {
      wcscpy_s(sIniFilePath, MAX_PATH, L".\\");    // last resort: working directory
      wcscat_s(sIniFilePath, MAX_PATH, PREF_INI_FILE);
   }

   m_AwakeCore.SetConfigFilePath(sIniFilePath);
}

void CStayAwakeDlg::InitRosterKeyCodes()
{
   wstring sSelectedKeyCodes{ m_AwakeCore.GetSelectedKeyCodes() };

   m_RosterLength = 0;

   for (int i{}; i < LEN_ROSTER_KEYCODES; i++)
   {
      if (sSelectedKeyCodes.at(i) == L'1')
         m_RosterKeyCodes[m_RosterLength++] = i;
   }
}

void CStayAwakeDlg::InitAwakes()
{
   InitRosterKeyCodes();
   SimulateAwakeKeyPress();

   SetDlgItemText(IDC_PAUSE_RESUME_BTN, BTN_TEXT_PAUSE);
   m_AwakeCore.SetPreference(PREF_AWAKE_PAUSED, L"N");
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
   SendDlgItemMessage(IDC_INTERVAL_MIN_EDIT, EM_HIDEBALLOONTIP, 0, 0);
   SendDlgItemMessage(IDC_INTERVAL_MAX_EDIT, EM_HIDEBALLOONTIP, 0, 0);

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


void CStayAwakeDlg::OnDestroy()
{
   if (m_bMinimized) Shell_NotifyIcon(NIM_DELETE, &m_TrayData);
   m_menu.DestroyMenu();

   WTSUnRegisterSessionNotification(m_hWnd);
   CDialogEx::OnDestroy();
}


void CStayAwakeDlg::SimulateAwakeKeyPress()
{
   if (!m_RosterLength) InitRosterKeyCodes();

   if (m_bSystemLocked)
   {
      SetDlgItemText(IDC_NEXT_EVENT_TIME_INFO, L"PAUSED since Windows is LOCKED");
      return;
   }

   UINT nAwakeKeyCode{ m_RosterKeyCodes[rand() % m_RosterLength] };
   wstring sAwakeKeyCode{};

   m_AwakeCore.SimulateInput(nAwakeKeyCode, sAwakeKeyCode);

   SYSTEMTIME lastTime{};
   GetLocalTime(&lastTime);
   SetDlgItemText(IDC_LAST_EVENT_TIME_INFO, Utils::formatSystemTime(lastTime, L"Last StayAwake event").c_str());

#ifdef DEBUG_DISPLAY_IDLE_TICKS
   Sleep((rand() % 20) + 1); // simulate a small delay to get a more accurate idle time
   ULONGLONG idleTime = m_AwakeCore.GetIdleTimeMilliseconds();
   SetDlgItemText(IDC_LAST_EVENT_INPUT_INFO, (L"[" + sAwakeKeyCode + L":" + to_wstring(idleTime) + L" ms]").c_str());
#else
   SetDlgItemText(IDC_LAST_EVENT_INPUT_INFO, (L"[" + sAwakeKeyCode + L"]").c_str());
#endif

   UINT nTimerSeconds{ m_IntervalMinSeconds };
   if (m_IntervalMinSeconds != m_IntervalMaxSeconds)
      nTimerSeconds += rand() % (abs(static_cast<int>(m_IntervalMaxSeconds - m_IntervalMinSeconds)) + 1);

   m_TimerID = SetTimer(m_TimerID, nTimerSeconds * 1000, NULL);

   SYSTEMTIME nextTime{};
   GetSystemTime(&nextTime);
   Utils::addSecondsToTime(nextTime, nTimerSeconds);
   SetDlgItemText(IDC_NEXT_EVENT_TIME_INFO, Utils::formatSystemTime(nextTime, L"Next StayAwake event").c_str());
}


void CStayAwakeDlg::OnSetTimerClicked()
{
   OnKillfocusIntervalMin();
   OnKillfocusIntervalMax();

   if (m_IntervalMinSeconds > m_IntervalMaxSeconds)
   {
      UINT nTemp{m_IntervalMinSeconds};
      m_IntervalMinSeconds = m_IntervalMaxSeconds;
      m_IntervalMaxSeconds = nTemp;

      SetDlgItemInt(IDC_INTERVAL_MIN_EDIT, m_IntervalMinSeconds, FALSE);
      SetDlgItemInt(IDC_INTERVAL_MAX_EDIT, m_IntervalMaxSeconds, FALSE);
   }

   m_AwakeCore.SetPreference(PREF_INTERVAL_MINIMUM, to_wstring(m_IntervalMinSeconds));
   m_AwakeCore.SetPreference(PREF_INTERVAL_MAXIMUM, to_wstring(m_IntervalMaxSeconds));

   InitAwakes();
}


void CStayAwakeDlg::OnClickedAboutButton()
{
   CAboutDlg dlgAbout;
   dlgAbout.DoModal();
}

void CStayAwakeDlg::OnPauseResumeClicked()
{
   if (IsTimerPaused())
   {
      InitAwakes();
   }
   else
   {
      KillTimer(m_TimerID);

      SetDlgItemText(IDC_PAUSE_RESUME_BTN, BTN_TEXT_RESUME);
      m_AwakeCore.SetPreference(PREF_AWAKE_PAUSED, L"Y");
      ShowPausedInfo(false);
   }
}

bool CStayAwakeDlg::IsTimerPaused()
{
   return (m_AwakeCore.GetPreference(PREF_AWAKE_PAUSED, L"N") == L"Y");
}

void CStayAwakeDlg::ShowPausedInfo(bool both)
{
   if (both)
      SetDlgItemText(IDC_LAST_EVENT_TIME_INFO, L"Last StayAwake event:         PAUSED");

   SetDlgItemText(IDC_NEXT_EVENT_TIME_INFO, L"Next StayAwake event:         PAUSED");
}

void CStayAwakeDlg::OnStartMinimized()
{
   m_AwakeCore.SetPreference(PREF_START_MINIMIZED, (IsDlgButtonChecked(IDC_START_MINIMIZED_CBX) == BST_CHECKED) ? L"Y" : L"N");
}

void CStayAwakeDlg::OnSessionChange(UINT nSessionState, UINT nId)
{
   switch (nSessionState)
   {
   case WTS_SESSION_LOCK:
      m_bSystemLocked = true;
      break;

   case WTS_SESSION_UNLOCK:
      m_bSystemLocked = false;
      break;
   }

   CDialogEx::OnSessionChange(nSessionState, nId);
}

void CStayAwakeDlg::OnUpdateCheckClicked()
{
}
