
// StayAwakeDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "StayAwake.h"
#include "StayAwakeDlg.h"
#include "afxdialogex.h"
#include "Utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
   CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
   enum { IDD = IDD_ABOUTBOX };
#endif

protected:
   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
   virtual BOOL OnInitDialog();
   afx_msg void OnProdUrlClick(NMHDR* pNotifyStruct, LRESULT* result);
   afx_msg void OnProdUrlReturn(NMHDR* pNotifyStruct, LRESULT* result);
   DECLARE_MESSAGE_MAP()
};

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
   ON_NOTIFY(NM_CLICK, IDC_ABOUT_PROD_URL, OnProdUrlClick)
   ON_NOTIFY(NM_RETURN, IDC_ABOUT_PROD_URL, OnProdUrlReturn)
END_MESSAGE_MAP()

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
}


BOOL CAboutDlg::OnInitDialog()
{
   CDialogEx::OnInitDialog();

#ifdef _WIN64
   wstring buildBit{ L" (64-bit)" };
#else
   wstring buildBit{ L" (32-bit)" };
#endif // _WIN64

   SetDlgItemText(IDC_ABOUT_NAME, Utils::getVersionInfo(theApp.m_hInstance, L"FileDescription").c_str());
   SetDlgItemText(IDC_ABOUT_VERSION, (L"Version: " + Utils::getVersionInfo(theApp.m_hInstance, L"FileVersion") + buildBit).c_str());
   SetDlgItemTextA(m_hWnd, IDC_ABOUT_BUILD_TIME, ("Build time: " + string(__DATE__) + " - " + string(__TIME__)).c_str());
   SetDlgItemText(IDC_ABOUT_ATTRIBUTION, Utils::getVersionInfo(theApp.m_hInstance, L"LegalCopyright").c_str());

   return TRUE;
}


afx_msg void CAboutDlg::OnProdUrlClick(NMHDR* pNotifyStruct, LRESULT* result)
{
   ShellExecute(NULL, L"open", Utils::getVersionInfo(theApp.m_hInstance, L"CompanyName").c_str(), NULL, NULL, SW_SHOW);
}


afx_msg void CAboutDlg::OnProdUrlReturn(NMHDR* pNotifyStruct, LRESULT* result)
{
   ShellExecute(NULL, L"open", Utils::getVersionInfo(theApp.m_hInstance, L"CompanyName").c_str(), NULL, NULL, SW_SHOW);
}


// CStayAwakeDlg dialog


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
   ON_WM_PAINT()
   ON_WM_QUERYDRAGICON()
   ON_MESSAGE(WM_POST_OPEN, &CStayAwakeDlg::OnPostOpen)
   ON_MESSAGE(WM_TRAY_NOTIFY, &CStayAwakeDlg::OnTrayNotify)
   ON_COMMAND(IDCANCEL, &CStayAwakeDlg::OnCancel)
   ON_COMMAND(IDM_RESTORE, &CStayAwakeDlg::OnRestore)
   ON_COMMAND(IDM_EXIT, &CStayAwakeDlg::OnExit)
   ON_WM_DESTROY()
   ON_WM_TIMER()
   ON_BN_CLICKED(IDC_MINIMIZE, &CStayAwakeDlg::OnMinimize)
   ON_BN_CLICKED(IDC_SET_INTERVAL, &CStayAwakeDlg::OnSetInterval)
   ON_EN_KILLFOCUS(IDC_INTERVAL, &CStayAwakeDlg::OnKillfocusInterval)
   ON_BN_CLICKED(IDC_ABOUT_BUTTON, &CStayAwakeDlg::OnClickedAboutButton)
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

   Utils::addTooltip(theApp.m_hInstance, m_hWnd, IDC_INTERVAL, L"", L"Number between 30 and 3600", 3, TRUE);
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CStayAwakeDlg::OnPaint()
{
   if (IsIconic())
   {
      CPaintDC dc(this); // device context for painting

      SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

      // Center icon in client rectangle
      int cxIcon = GetSystemMetrics(SM_CXICON);
      int cyIcon = GetSystemMetrics(SM_CYICON);
      CRect rect;
      GetClientRect(&rect);
      int x = (rect.Width() - cxIcon + 1) / 2;
      int y = (rect.Height() - cyIcon + 1) / 2;

      // Draw the icon
      dc.DrawIcon(x, y, m_hIcon);
   }
   else
   {
      CDialogEx::OnPaint();
   }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CStayAwakeDlg::OnQueryDragIcon()
{
   return static_cast<HCURSOR>(m_hIcon);
}


afx_msg LRESULT CStayAwakeDlg::OnPostOpen(WPARAM wParam, LPARAM lParam)
{
   m_TimerSeconds = GetPrivateProfileInt(PREF_DEFAULTS, PREF_TIMER_INTERVAL, m_TimerSeconds, PREF_INI_FILE);
   SetDlgItemInt(IDC_INTERVAL, m_TimerSeconds, FALSE);

   wchar_t sMulti[MAX_PATH + 1];
   GetPrivateProfileString(PREF_DEFAULTS, PREF_MULTI_INSTANCE, L"N", sMulti, MAX_PATH, PREF_INI_FILE);

   if (wstring{ sMulti } != L"Y" && Utils::getProcessRunCount(L"StayAwake.exe") > 1)
   {
      MessageBox(L"Another instance of StayAwake is already running.\n\nHence this instance will exit.");
      DestroyWindow();
      return 0;
   }

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


void CStayAwakeDlg::OnKillfocusInterval()
{
   int nInterval{};

   nInterval = GetDlgItemInt(IDC_INTERVAL, nullptr, FALSE);

   if (nInterval < 30 || nInterval > 3600)
   {
      Utils::showEditBalloonTip(GetDlgItem(IDC_INTERVAL)->m_hWnd, L"Timer Interval in seconds", L"Please enter a value between 30 and 3600");
      SetDlgItemInt(IDC_INTERVAL, m_TimerSeconds, FALSE);
      return;
   }

   m_TimerSeconds = nInterval;
}


void CStayAwakeDlg::OnTimer(UINT_PTR nIDEvent)
{
   ToggleScrollLock();
   CDialogEx::OnTimer(nIDEvent);
}


void CStayAwakeDlg::InitTimer()
{
   ToggleScrollLock();
   m_TimerID = SetTimer(m_TimerID, m_TimerSeconds * 1000, NULL);
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
   SendDlgItemMessage(IDC_INTERVAL, EM_HIDEBALLOONTIP, 0, 0);

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


void CStayAwakeDlg::ToggleScrollLock()
{
   keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
   keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
   Sleep(10);
   keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
   keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);

   SYSTEMTIME lastTime{};
   GetLocalTime(&lastTime);
   SetDlgItemText(IDC_STATIC_LAST_TOGGLE, Utils::formatSystemTime(lastTime, L"Scroll Lock was last toggled").c_str());

   SYSTEMTIME nextTime{};
   GetSystemTime(&nextTime);
   Utils::addSecondsToTime(nextTime, m_TimerSeconds);
   SetDlgItemText(IDC_STATIC_NEXT_TOGGLE, Utils::formatSystemTime(nextTime, L"Scroll Lock will next toggle").c_str());
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
