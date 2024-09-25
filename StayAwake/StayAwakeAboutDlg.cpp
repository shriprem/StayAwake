#include "pch.h"
#include "framework.h"
#include "StayAwake.h"
#include "afxdialogex.h"
#include "Utils.h"
#include "StayAwakeAboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
