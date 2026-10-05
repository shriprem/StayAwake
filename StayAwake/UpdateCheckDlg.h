#pragma once

#include "afxdialogex.h"
#include "StayAwakeDlg.h"


// CUpdateCheckDlg dialog

class CUpdateCheckDlg : public CDialogEx
{
   DECLARE_DYNAMIC(CUpdateCheckDlg)

public:
   explicit CUpdateCheckDlg(CWnd* pParent, StayAwakeCore& pAwakeCore);
   virtual ~CUpdateCheckDlg();

   static bool IsNewerVersionAvailable(wstring& currentVersion, wstring& latestVersion);

// Dialog Data
#ifdef AFX_DESIGN_TIME
   enum { IDD = IDD_UPDATE_CHECK_DIALOG };
#endif

protected:
   StayAwakeCore m_AwakeCore{};
   HICON m_hIcon;

   virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

   virtual BOOL OnInitDialog();
   afx_msg void OnClickLatestReleaseUrl(NMHDR* pNMHDR, LRESULT* pResult);
   afx_msg void OnClickedStartVersionCheck();
   afx_msg void OnReturnLatestReleaseUrl(NMHDR* pNMHDR, LRESULT* pResult);

   DECLARE_MESSAGE_MAP()
};
