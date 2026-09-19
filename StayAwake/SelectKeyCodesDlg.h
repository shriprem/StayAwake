#pragma once

#include "Utils.h"

class CSelectKeyCodesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectKeyCodesDlg)

public:
	CSelectKeyCodesDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CSelectKeyCodesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_KEYCODES_DIALOG };
#endif

protected:
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnClickedKeySelectAllBtn();
	afx_msg void OnClickedKeySelectNoneBtn();
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	void CheckAllBoxes(const wstring& sSelectedKeyCodes);
};
