#include "pch.h"

#include "afxdialogex.h"
#include "StayAwake.h"
#include "UpdateCheckDlg.h"
#include "Utils.h"
#include "VersionUpdate.h"


// CUpdateCheckDlg dialog

IMPLEMENT_DYNAMIC(CUpdateCheckDlg, CDialogEx)

CUpdateCheckDlg::CUpdateCheckDlg(CWnd* pParent, StayAwakeCore& pAwakeCore)
   : CDialogEx(IDD_UPDATE_CHECK_DIALOG, pParent)
   , m_AwakeCore(pAwakeCore)
{
   m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CUpdateCheckDlg::~CUpdateCheckDlg()
{
}

void CUpdateCheckDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CUpdateCheckDlg, CDialogEx)
   ON_NOTIFY(NM_CLICK, IDC_LATEST_RELEASE_URL, &CUpdateCheckDlg::OnClickLatestReleaseUrl)
   ON_BN_CLICKED(IDC_START_VERSION_CHECK_CBX, &CUpdateCheckDlg::OnClickedStartVersionCheck)
   ON_NOTIFY(NM_RETURN, IDC_LATEST_RELEASE_URL, &CUpdateCheckDlg::OnReturnLatestReleaseUrl)
END_MESSAGE_MAP()


// CUpdateCheckDlg message handlers

BOOL CUpdateCheckDlg::OnInitDialog()
{
   CDialogEx::OnInitDialog();

   wstring currentVersionInfo{ Utils::getVersionInfo(theApp.m_hInstance, L"FileVersion") };

   VersionUpdate versionUpdate;
   wstring latestVersionInfo{ versionUpdate.GetVersion() };

   SetDlgItemText(IDC_INSTALLED_VERSION_INFO, currentVersionInfo.c_str());
   SetDlgItemText(IDC_LATEST_VERSION_INFO, latestVersionInfo.c_str());
   SetDlgItemText(IDC_VERSION_CHECK_STATUS,
      (latestVersionInfo == currentVersionInfo) ?
      L"This is the latest available version." :
      L"A newer version is available. Click the link below.");

   BOOL bStartUpdateCheck = (m_AwakeCore.GetPreference(PREF_START_UPDATE_CHECK, L"N") == L"Y");
   CheckDlgButton(IDC_START_VERSION_CHECK_CBX, bStartUpdateCheck ? BST_CHECKED : BST_UNCHECKED);

   return TRUE;  // return TRUE unless you set the focus to a control
}

void CUpdateCheckDlg::OnClickLatestReleaseUrl(NMHDR* pNMHDR, LRESULT* pResult)
{
   ShellExecute(nullptr, L"open", L"https://github.com/shriprem/StayAwake/releases/latest", nullptr, nullptr, SW_SHOW);
   *pResult = 0;
}

void CUpdateCheckDlg::OnReturnLatestReleaseUrl(NMHDR* pNMHDR, LRESULT* pResult)
{
   OnClickLatestReleaseUrl(pNMHDR, pResult);
}

void CUpdateCheckDlg::OnClickedStartVersionCheck()
{
   m_AwakeCore.SetPreference(PREF_START_UPDATE_CHECK, (IsDlgButtonChecked(IDC_START_VERSION_CHECK_CBX) == BST_CHECKED) ? L"Y" : L"N");
}
