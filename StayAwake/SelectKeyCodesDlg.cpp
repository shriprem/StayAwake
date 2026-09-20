// CSelectKeyCodesDlg.cpp : implementation file
//

#include "pch.h"
#include "StayAwake.h"
#include "afxdialogex.h"
#include "SelectKeyCodesDlg.h"

static_assert(IDC_KEY_UNASSIGNED_10 - IDC_KEY_SCROLL_LOCK + 1 == LEN_ROSTER_KEYCODES, "Roster checkbox IDs must be consecutive");

// CSelectKeyCodesDlg dialog

IMPLEMENT_DYNAMIC(CSelectKeyCodesDlg, CDialogEx)

CSelectKeyCodesDlg::CSelectKeyCodesDlg(CStayAwakeDlg* pCaller)
	: CDialogEx(IDD_SELECT_KEYCODES_DIALOG, pCaller)
	, m_pCaller(pCaller)
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
	ON_BN_CLICKED(IDOK, &CSelectKeyCodesDlg::OnOK)
	ON_BN_CLICKED(IDC_KEY_SELECT_ALL_BTN, &CSelectKeyCodesDlg::OnClickedKeySelectAllBtn)
	ON_BN_CLICKED(IDC_KEY_SELECT_NONE_BTN, &CSelectKeyCodesDlg::OnClickedKeySelectNoneBtn)
END_MESSAGE_MAP()


BOOL CSelectKeyCodesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CheckAllBoxes(m_pCaller->GetSelectedKeyCodes());

	return TRUE;  // return TRUE unless you set the focus to a control
}

void CSelectKeyCodesDlg::OnOK()
{
	wstring sSelectedKeyCodes{};

	for (int i{}; i < LEN_ROSTER_KEYCODES; i++)
		sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_SCROLL_LOCK + i)) ? L"1" : L"0";

	if (sSelectedKeyCodes == wstring(LEN_ROSTER_KEYCODES, L'0'))
	{
		MessageBox(L"Please select at least one Key Code", L"Select multiple Key Codes", MB_ICONEXCLAMATION);
		return;
	}

	if (!m_pCaller->SaveSelectedKeyCodes(sSelectedKeyCodes))
	{
		MessageBox(L"Unable to save to the StayAwake.ini file.", L"Select multiple Key Codes", MB_ICONEXCLAMATION);
		return;
	}

	CDialogEx::OnOK();
}

void CSelectKeyCodesDlg::CheckAllBoxes(const wstring& sSelectedKeyCodes)
{
	for (int i{}; i < LEN_ROSTER_KEYCODES; i++)
		CheckDlgButton(IDC_KEY_SCROLL_LOCK + i, sSelectedKeyCodes.at(i) == L'1');
}

void CSelectKeyCodesDlg::OnClickedKeySelectAllBtn()
{
	CheckAllBoxes(wstring(LEN_ROSTER_KEYCODES, L'1'));
}

void CSelectKeyCodesDlg::OnClickedKeySelectNoneBtn()
{
	CheckAllBoxes(wstring(LEN_ROSTER_KEYCODES, L'0'));
}
