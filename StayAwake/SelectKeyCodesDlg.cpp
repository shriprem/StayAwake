// CSelectKeyCodesDlg.cpp : implementation file
//

#include "pch.h"
#include "StayAwake.h"
#include "afxdialogex.h"
#include "SelectKeyCodesDlg.h"
#include "StayAwakeDlg.h"


// CSelectKeyCodesDlg dialog

IMPLEMENT_DYNAMIC(CSelectKeyCodesDlg, CDialogEx)

CSelectKeyCodesDlg::CSelectKeyCodesDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SELECT_KEYCODES_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CSelectKeyCodesDlg::~CSelectKeyCodesDlg()
{
}

void CSelectKeyCodesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSelectKeyCodesDlg, CDialogEx)
	ON_BN_CLICKED(IDC_KEY_SELECT_APPLY_BTN, &CSelectKeyCodesDlg::OnOK)
	ON_BN_CLICKED(IDC_KEY_SELECT_ALL_BTN, &CSelectKeyCodesDlg::OnClickedKeySelectAllBtn)
	ON_BN_CLICKED(IDC_KEY_SELECT_NONE_BTN, &CSelectKeyCodesDlg::OnClickedKeySelectNoneBtn)
END_MESSAGE_MAP()


BOOL CSelectKeyCodesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CheckAllBoxes(CStayAwakeDlg::GetSelectedKeyCodes());

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSelectKeyCodesDlg::OnOK()
{
	wstring sSelectedKeyCodes{};

	for (int i{}; i < LEN_KEYCODES_ROSTER; i++)
		sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_SCROLL_LOCK + i)) ? L"1" : L"0";

	if (sSelectedKeyCodes == wstring(LEN_KEYCODES_ROSTER, L'0'))
	{
		MessageBox(L"Please select at least one Key Simulation", L"Select multiple Key Codes", MB_ICONERROR);
		return;
	}

	CStayAwakeDlg::SaveSelectedKeyCodes(sSelectedKeyCodes);

	CDialogEx::OnOK();
}

void CSelectKeyCodesDlg::CheckAllBoxes(const wstring& sSelectedKeyCodes)
{
	for (int i{}; i < LEN_KEYCODES_ROSTER; i++)
		CheckDlgButton(IDC_KEY_SCROLL_LOCK + i, sSelectedKeyCodes.at(i) == L'1');
}

void CSelectKeyCodesDlg::OnClickedKeySelectAllBtn()
{
	CheckAllBoxes(wstring(LEN_KEYCODES_ROSTER, L'1'));
}

void CSelectKeyCodesDlg::OnClickedKeySelectNoneBtn()
{
	CheckAllBoxes(wstring(LEN_KEYCODES_ROSTER, L'0'));
}
