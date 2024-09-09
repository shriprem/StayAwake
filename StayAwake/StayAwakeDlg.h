
// StayAwakeDlg.h : header file
//

#pragma once

#define WM_POST_OPEN    (WM_APP + 1)
#define WM_TRAY_NOTIFY  (WM_APP + 2)

constexpr auto PREFERENCES_INI = L".\\StayAwake.ini";
constexpr auto PREFERENCES_KEY = L"TimerIntervalInSeconds";

#include <string>

using std::wstring;
using std::to_wstring;

// CStayAwakeDlg dialog
class CStayAwakeDlg : public CDialogEx
{
// Construction
public:
   CStayAwakeDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
   enum { IDD = IDD_STAYAWAKE_DIALOG };
#endif

   protected:
   virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
   HICON m_hIcon;

   // Generated message map functions
   virtual BOOL OnInitDialog();
   afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
   afx_msg void OnPaint();
   afx_msg HCURSOR OnQueryDragIcon();
   DECLARE_MESSAGE_MAP()

private:
   bool m_bMinimized{};
   UINT_PTR m_TimerID{};
   UINT m_TimerSeconds{ 240 };
   NOTIFYICONDATA m_TrayData{};
   CMenu m_menu;

   afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnPostOpen(WPARAM wParam, LPARAM lParam);
   afx_msg void OnRestore();
   afx_msg void OnSetInterval();
   afx_msg void OnMinimize();
   afx_msg void OnExit();
   afx_msg void OnDestroy();
   afx_msg void OnTimer(UINT_PTR nIDEvent);

   void TrayIconInit();
   void MinimizeToTray();
   void OnTrayButtonDown(CPoint pt);
   void RestoreFromTray();
   void ToggleScrollLock();
   void AddSecondsToTime(SYSTEMTIME& st, int seconds);
};
