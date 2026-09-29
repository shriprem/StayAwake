#pragma once

#include "Utils.h"
#include "StayAwakeDlg.h"

constexpr auto SELECT_KEYCODES_DIALOG_TITLE = L"Select multiple simulation options";

class CSelectKeyCodesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectKeyCodesDlg)

public:
	explicit CSelectKeyCodesDlg(CWnd* pParent, StayAwakeCore& pAwakeCore);
	virtual ~CSelectKeyCodesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SELECT_KEYCODES_DIALOG };
#endif

protected:
	StayAwakeCore m_AwakeCore{};
	HICON m_hIcon;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();
	afx_msg void OnClickedKeySelectAllBtn();
	afx_msg void OnClickedKeySelectNoneBtn();
	afx_msg void OnClickedSelectAllUnassignedKeys();
	afx_msg void OnClickedSelectAllExtFnKeys();
	afx_msg void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	void CheckAllBoxes(const wstring& sSelectedKeyCodes, int start = IDC_KEY_SCROLL_LOCK, int endNext = IDC_KEY_SCROLL_LOCK + LEN_ROSTER_KEYCODES);
};
