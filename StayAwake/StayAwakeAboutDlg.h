#pragma once

class CAboutDlg : public CDialogEx
{
public:
   CAboutDlg();

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
