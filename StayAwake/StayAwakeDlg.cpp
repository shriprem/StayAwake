#include "pch.h"
#include "framework.h"
#include "StayAwake.h"
#include "StayAwakeDlg.h"
#include "SelectKeyCodesDlg.h"
#include "StayAwakeAboutDlg.h"
#include "afxdialogex.h"
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

wstring CStayAwakeDlg::GetSelectedKeyCodes()
{
   const int bufSize{ LEN_KEYCODES_ROSTER + 1 };
   wchar_t sBuf[bufSize]{};

   GetPrivateProfileString(PREF_DEFAULTS, PREF_SELECTED_KEYCODES, L"N/A", sBuf, bufSize, m_IniFilePath);

   wstring sKeyCodes{ sBuf };

   if (sKeyCodes == L"N/A") {
      UINT nLegacyKeyCode = GetPrivateProfileInt(PREF_DEFAULTS, PREF_LEGACY_KEYCODE, 99, m_IniFilePath);

      if (nLegacyKeyCode == 99)
         sKeyCodes = DEF_SELECTED_KEYCODES;
      else
      {
         sKeyCodes = wstring(LEN_KEYCODES_ROSTER, L'0');
         sKeyCodes.replace(nLegacyKeyCode % LEN_KEYCODES_ROSTER, 1, L"1");
      }

      WritePrivateProfileString(PREF_DEFAULTS, PREF_LEGACY_KEYCODE, nullptr, m_IniFilePath);
      SaveSelectedKeyCodes(sKeyCodes);
   }
   else if (!CheckSelectedKeyCodes(sKeyCodes))
   {
      sKeyCodes = DEF_SELECTED_KEYCODES;
      SaveSelectedKeyCodes(sKeyCodes);
   }

   return sKeyCodes;
}

bool CStayAwakeDlg::CheckSelectedKeyCodes(wstring sKeyCodes)
{
   return (sKeyCodes.length() == LEN_KEYCODES_ROSTER &&
      sKeyCodes != wstring(LEN_KEYCODES_ROSTER, L'0') &&
      sKeyCodes.find_first_not_of(L"01") == std::string::npos);
}

bool CStayAwakeDlg::SaveSelectedKeyCodes(wstring sKeyCodes)
{
   return CheckSelectedKeyCodes(sKeyCodes) &&
      WritePrivateProfileString(PREF_DEFAULTS, PREF_SELECTED_KEYCODES, sKeyCodes.c_str(), m_IniFilePath);
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
   ON_BN_CLICKED(IDC_MINIMIZE, &CStayAwakeDlg::OnMinimize)
   ON_BN_CLICKED(IDC_EXIT, &CStayAwakeDlg::OnExit)
   ON_BN_CLICKED(IDC_STAYAWAKE_KEYS_ROSTER_BTN, &CStayAwakeDlg::OnSetupKeyCodesRosterClicked)
   ON_BN_CLICKED(IDC_STAYAWAKE_SET_INTERVAL_BTN, &CStayAwakeDlg::OnSetInterval)
   ON_EN_KILLFOCUS(IDC_STAYAWAKE_INTERVAL_MIN, &CStayAwakeDlg::OnKillfocusIntervalMin)
   ON_EN_KILLFOCUS(IDC_STAYAWAKE_INTERVAL_MAX, &CStayAwakeDlg::OnKillfocusIntervalMax)
   ON_BN_CLICKED(IDC_ABOUT_BUTTON, &CStayAwakeDlg::OnClickedAboutButton)
   ON_BN_CLICKED(IDC_STAYAWAKE_PAUSE_RESUME_BTN, &CStayAwakeDlg::OnPauseResume)
   ON_BN_CLICKED(IDC_START_MINIMIZED, &CStayAwakeDlg::OnStartMinimized)
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
   InitIntervals();
   SetDlgItemInt(IDC_STAYAWAKE_INTERVAL_MIN, m_IntervalMinSeconds, FALSE);
   SetDlgItemInt(IDC_STAYAWAKE_INTERVAL_MAX, m_IntervalMaxSeconds, FALSE);

   Utils::addTooltip(theApp.m_hInstance, m_hWnd, IDC_STAYAWAKE_INTERVAL_MIN, L"", INTERVAL_TOOLTIP, 3, TRUE);
   Utils::addTooltip(theApp.m_hInstance, m_hWnd, IDC_STAYAWAKE_INTERVAL_MAX, L"", INTERVAL_TOOLTIP, 3, TRUE);

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
      InitAwakes();

   BOOL bMinimized = (GetPreference(PREF_START_MINIMIZED, L"N") == L"Y");
   CheckDlgButton(IDC_START_MINIMIZED, bMinimized ? BST_CHECKED : BST_UNCHECKED);
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

   nInterval = GetDlgItemInt(IDC_STAYAWAKE_INTERVAL_MIN, nullptr, FALSE);

   if (nInterval < MIN_PERIOD || nInterval > MAX_PERIOD)
   {
      Utils::showEditBalloonTip(GetDlgItem(IDC_STAYAWAKE_INTERVAL_MIN)->m_hWnd, INTERVAL_WARN_TITLE, INTERVAL_WARNING.c_str());
      SetDlgItemInt(IDC_STAYAWAKE_INTERVAL_MIN, m_IntervalMinSeconds, FALSE);
      return;
   }

   m_IntervalMinSeconds = nInterval;
}


void CStayAwakeDlg::OnKillfocusIntervalMax()
{
   int nInterval{};

   nInterval = GetDlgItemInt(IDC_STAYAWAKE_INTERVAL_MAX, nullptr, FALSE);

   if (nInterval < MIN_PERIOD || nInterval > MAX_PERIOD)
   {
      Utils::showEditBalloonTip(GetDlgItem(IDC_STAYAWAKE_INTERVAL_MAX)->m_hWnd, INTERVAL_WARN_TITLE, INTERVAL_WARNING.c_str());
      SetDlgItemInt(IDC_STAYAWAKE_INTERVAL_MAX, m_IntervalMaxSeconds, FALSE);
      return;
   }

   m_IntervalMaxSeconds = nInterval;
}


void CStayAwakeDlg::OnSetupKeyCodesRosterClicked()
{
   CSelectKeyCodesDlg dlgSelectKeyCodes(this);

   if (dlgSelectKeyCodes.DoModal() == IDOK)
   {
      InitRoster();
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
   DWORD size = GetModuleFileNameW(nullptr, m_IniFilePath, MAX_PATH);

   if (size > 0 && size < MAX_PATH &&
      SUCCEEDED(PathCchRemoveFileSpec(m_IniFilePath, MAX_PATH)) &&
      SUCCEEDED(PathCchAppend(m_IniFilePath, MAX_PATH, PREF_INI_FILE)))
      return;

   wcscpy_s(m_IniFilePath, MAX_PATH, L".\\");    // last resort: working directory
   wcscat_s(m_IniFilePath, MAX_PATH, PREF_INI_FILE);
}


void CStayAwakeDlg::InitIntervals()
{
   int nIntervalLegacy{}, nIntervalMin{}, nIntervalMax{};

   nIntervalLegacy = GetPrivateProfileInt(PREF_DEFAULTS, PREF_INTERVAL_LEGACY, DEF_PERIOD, m_IniFilePath);
   if (nIntervalLegacy < MIN_PERIOD || nIntervalLegacy > MAX_PERIOD)
      nIntervalLegacy = DEF_PERIOD;

   nIntervalMin = GetPrivateProfileInt(PREF_DEFAULTS, PREF_INTERVAL_MINIMUM, nIntervalLegacy, m_IniFilePath);
   if (nIntervalMin < MIN_PERIOD || nIntervalMin > MAX_PERIOD)
      nIntervalMin = DEF_PERIOD;

   nIntervalMax = GetPrivateProfileInt(PREF_DEFAULTS, PREF_INTERVAL_MAXIMUM, nIntervalLegacy, m_IniFilePath);
   if (nIntervalMax < MIN_PERIOD || nIntervalMax > MAX_PERIOD)
      nIntervalMax = DEF_PERIOD;

   m_IntervalMinSeconds = (nIntervalMin <= nIntervalMax) ? nIntervalMin : nIntervalMax;
   m_IntervalMaxSeconds = (nIntervalMin >= nIntervalMax) ? nIntervalMin : nIntervalMax;
}

void CStayAwakeDlg::InitRoster()
{
   wstring sSelectedKeyCodes{ GetSelectedKeyCodes() };

   m_RosterLength = 0;

   for (int i{}; i < LEN_KEYCODES_ROSTER; i++)
   {
      if (sSelectedKeyCodes.at(i) == L'1')
         m_RosterKeyCodes[m_RosterLength++] = i;
   }
}

void CStayAwakeDlg::InitAwakes()
{
   InitRoster();
   SimulateAwakeKeyPress();

   SetDlgItemText(IDC_STAYAWAKE_PAUSE_RESUME_BTN, BTN_TEXT_PAUSE);
   WritePrivateProfileString(PREF_DEFAULTS, PREF_AWAKE_PAUSED, L"N", m_IniFilePath);
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
   SendDlgItemMessage(IDC_STAYAWAKE_INTERVAL_MIN, EM_HIDEBALLOONTIP, 0, 0);
   SendDlgItemMessage(IDC_STAYAWAKE_INTERVAL_MAX, EM_HIDEBALLOONTIP, 0, 0);

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

   CDialogEx::OnDestroy();
}


void CStayAwakeDlg::SimulateAwakeKeyPress()
{
   if (!m_RosterLength) InitRoster();

   UINT nAwakeKeyCode{ m_RosterKeyCodes[rand() % m_RosterLength] };
   wstring sAwakeKeyCode{};

   switch (nAwakeKeyCode)
   {
   case 1:
      sAwakeKeyCode = L"Volume Down && Up";
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
      sAwakeKeyCode = L"Unassigned Key #" + to_wstring(nAwakeKeyCode - 1);
      BYTE keycode{ static_cast<BYTE>(VK_UNASSIGNED_01 + nAwakeKeyCode - 2) };
      keybd_event(keycode, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(keycode, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      break;
   }

   case 11:
      sAwakeKeyCode = L"Unassigned Key #10";
      keybd_event(VK_UNASSIGNED_10, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_UNASSIGNED_10, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      break;

   default:
      sAwakeKeyCode = L"Scroll Lock cycling";
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
   SetDlgItemText(IDC_STAYAWAKE_LAST_KEYCODE, (L"[" + sAwakeKeyCode + L"]").c_str());

   UINT nTimerSeconds{ m_IntervalMinSeconds };
   if (m_IntervalMinSeconds != m_IntervalMaxSeconds)
      nTimerSeconds += rand() % (abs(static_cast<int>(m_IntervalMaxSeconds - m_IntervalMinSeconds)) + 1);

   m_TimerID = SetTimer(m_TimerID, nTimerSeconds * 1000, NULL);

   SYSTEMTIME nextTime{};
   GetSystemTime(&nextTime);
   Utils::addSecondsToTime(nextTime, nTimerSeconds);
   SetDlgItemText(IDC_STAYAWAKE_NEXT_EVENT, Utils::formatSystemTime(nextTime, L"Next StayAwake event").c_str());
}


void CStayAwakeDlg::OnSetInterval()
{
   OnKillfocusIntervalMin();
   OnKillfocusIntervalMax();

   if (m_IntervalMinSeconds > m_IntervalMaxSeconds)
   {
      UINT nTemp{m_IntervalMinSeconds};
      m_IntervalMinSeconds = m_IntervalMaxSeconds;
      m_IntervalMaxSeconds = nTemp;

      SetDlgItemInt(IDC_STAYAWAKE_INTERVAL_MIN, m_IntervalMinSeconds, FALSE);
      SetDlgItemInt(IDC_STAYAWAKE_INTERVAL_MAX, m_IntervalMaxSeconds, FALSE);
   }

   WritePrivateProfileString(PREF_DEFAULTS, PREF_INTERVAL_MINIMUM, to_wstring(m_IntervalMinSeconds).c_str(), m_IniFilePath);
   WritePrivateProfileString(PREF_DEFAULTS, PREF_INTERVAL_MAXIMUM, to_wstring(m_IntervalMaxSeconds).c_str(), m_IniFilePath);

   InitAwakes();
}


void CStayAwakeDlg::OnClickedAboutButton()
{
   CAboutDlg dlgAbout;
   dlgAbout.DoModal();
}

void CStayAwakeDlg::OnPauseResume()
{
   if (IsTimerPaused())
   {
      InitAwakes();
   }
   else
   {
      KillTimer(m_TimerID);

      SetDlgItemText(IDC_STAYAWAKE_PAUSE_RESUME_BTN, BTN_TEXT_RESUME);
      WritePrivateProfileString(PREF_DEFAULTS, PREF_AWAKE_PAUSED, L"Y", m_IniFilePath);
      ShowPausedInfo(false);
   }
}

wstring CStayAwakeDlg::GetPreference(wstring key, wstring defaultVal)
{
   const int bufSize{ MAX_PATH };
   wstring ftBuf(bufSize, '\0');

   GetPrivateProfileString(PREF_DEFAULTS, key.c_str(), defaultVal.c_str(), ftBuf.data(), bufSize, m_IniFilePath);

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

void CStayAwakeDlg::OnStartMinimized()
{
   WritePrivateProfileString(PREF_DEFAULTS, PREF_START_MINIMIZED,
      (IsDlgButtonChecked(IDC_START_MINIMIZED) == BST_CHECKED) ? L"Y" : L"N", m_IniFilePath);
}
