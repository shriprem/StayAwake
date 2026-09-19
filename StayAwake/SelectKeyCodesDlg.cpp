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
	ON_BN_CLICKED(IDC_KEY_SELECT_APPLY_BTN, &CSelectKeyCodesDlg::OnBnClickedApply)
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

void CSelectKeyCodesDlg::OnBnClickedApply()
{
	wstring sSelectedKeyCodes{};

	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_SCROLL_LOCK)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_VOLUME_UP_DOWN)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_UNASSIGNED_1)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_UNASSIGNED_2)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_UNASSIGNED_3)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_UNASSIGNED_4)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_UNASSIGNED_5)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_UNASSIGNED_6)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_UNASSIGNED_7)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_UNASSIGNED_8)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_UNASSIGNED_9)) ? L"1" : L"0";
	sSelectedKeyCodes += (IsDlgButtonChecked(IDC_KEY_UNASSIGNED_10)) ? L"1" : L"0";

	if (sSelectedKeyCodes == L"000000000000")
	{
		MessageBox(L"Please select at least one Key Simulation", L"Select Key Simulations", MB_ICONERROR);
		return;
	}

	CStayAwakeDlg::SaveSelectedKeyCodes(sSelectedKeyCodes);

	CDialogEx::OnOK();
}

void CSelectKeyCodesDlg::CheckAllBoxes(const wstring& sSelectedKeyCodes)
{
	CheckDlgButton(IDC_KEY_SCROLL_LOCK, sSelectedKeyCodes.substr(0, 1) == L"1");
	CheckDlgButton(IDC_KEY_VOLUME_UP_DOWN, sSelectedKeyCodes.substr(1, 1) == L"1");
	CheckDlgButton(IDC_KEY_UNASSIGNED_1, sSelectedKeyCodes.substr(2, 1) == L"1");
	CheckDlgButton(IDC_KEY_UNASSIGNED_2, sSelectedKeyCodes.substr(3, 1) == L"1");
	CheckDlgButton(IDC_KEY_UNASSIGNED_3, sSelectedKeyCodes.substr(4, 1) == L"1");
	CheckDlgButton(IDC_KEY_UNASSIGNED_4, sSelectedKeyCodes.substr(5, 1) == L"1");
	CheckDlgButton(IDC_KEY_UNASSIGNED_5, sSelectedKeyCodes.substr(6, 1) == L"1");
	CheckDlgButton(IDC_KEY_UNASSIGNED_6, sSelectedKeyCodes.substr(7, 1) == L"1");
	CheckDlgButton(IDC_KEY_UNASSIGNED_7, sSelectedKeyCodes.substr(8, 1) == L"1");
	CheckDlgButton(IDC_KEY_UNASSIGNED_8, sSelectedKeyCodes.substr(9, 1) == L"1");
	CheckDlgButton(IDC_KEY_UNASSIGNED_9, sSelectedKeyCodes.substr(10, 1) == L"1");
	CheckDlgButton(IDC_KEY_UNASSIGNED_10, sSelectedKeyCodes.substr(11, 1) == L"1");
}

void CSelectKeyCodesDlg::OnClickedKeySelectAllBtn()
{
	CheckAllBoxes(L"111111111111");
}

void CSelectKeyCodesDlg::OnClickedKeySelectNoneBtn()
{
	CheckAllBoxes(L"000000000000");
}
