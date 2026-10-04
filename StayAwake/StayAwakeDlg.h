#pragma once

#include "StayAwakeCore.h"
#include "Utils.h"


#define WM_POST_OPEN       (WM_APP + 1)
#define WM_TRAY_NOTIFY     (WM_APP + 2)
#define WM_RESTORE_DIALOG  theApp.WM_SHOWFIRSTINSTANCE

//#define DEBUG_DISPLAY_IDLE_TICKS

constexpr auto BTN_TEXT_PAUSE = L"&Pause";
constexpr auto BTN_TEXT_RESUME = L"&Resume";

const wstring MIN_MAX_PERIOD = to_wstring(MIN_PERIOD) + L" and " + to_wstring(MAX_PERIOD);
const wstring INTERVAL_TOOLTIP = L"Number between " + MIN_MAX_PERIOD;
const wstring INTERVAL_WARNING = L"Please enter a value between " + MIN_MAX_PERIOD;
const LPCWSTR INTERVAL_WARN_TITLE = L"Timer Interval in seconds";

class CStayAwakeDlg : public CDialogEx
{
public:
   CStayAwakeDlg(CWnd* pParent = nullptr);	// standard constructor

#ifdef AFX_DESIGN_TIME
   enum { IDD = IDD_STAYAWAKE_DIALOG };
#endif

protected:
   virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


protected:
   HICON m_hIcon;

   virtual BOOL OnInitDialog();
   afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
   DECLARE_MESSAGE_MAP()

private:
   bool m_bMinimized{}, m_bSystemLocked{};
   UINT_PTR m_TimerID{42};

   UINT m_RosterKeyCodes[LEN_ROSTER_KEYCODES]{};
   UINT m_RosterLength{};

   UINT m_IntervalMinSeconds{};
   UINT m_IntervalMaxSeconds{};

   StayAwakeCore m_AwakeCore{};
   NOTIFYICONDATA m_TrayData{};
   CMenu m_menu;

   afx_msg LRESULT OnPostOpen(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnRestoreDialog(WPARAM wParam, LPARAM lParam);
   afx_msg void OnSelectInputOptionsClicked();
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   afx_msg void OnKillfocusIntervalMin();
   afx_msg void OnKillfocusIntervalMax();
   afx_msg void OnSetTimerClicked();
   afx_msg void OnPauseResumeClicked();
   afx_msg void OnMinimize() { MinimizeToTray(); };
   afx_msg void OnClickedAboutButton();
   afx_msg void OnEnter() { /* Ignore ENTER key press */ };
   afx_msg void OnCancel() { MinimizeToTray(); };
   afx_msg void OnRestore() { RestoreFromTray(); };
   afx_msg void OnExit() { DestroyWindow(); };
   afx_msg void OnDestroy();
   afx_msg void OnStartMinimized();
   afx_msg void OnSessionChange(UINT nSessionState, UINT nId);

   void InitConfigFilePath();
   void InitRosterKeyCodes();
   void InitAwakes();
   void InitTrayIcon();
   void MinimizeToTray();
   void OnTrayButtonDown(CPoint pt);
   void RestoreFromTray();
   void SimulateAwakeKeyPress();
   void ShowPausedInfo(bool both);

   bool IsTimerPaused();
public:
    afx_msg void OnUpdateCheckClicked();
};
