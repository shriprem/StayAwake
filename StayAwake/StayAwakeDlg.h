#pragma once

#include "Utils.h"

#define WM_POST_OPEN       (WM_APP + 1)
#define WM_TRAY_NOTIFY     (WM_APP + 2)
#define WM_RESTORE_DIALOG  theApp.WM_SHOWFIRSTINSTANCE

constexpr auto VK_UNASSIGNED_01 = 0x97;
constexpr auto VK_UNASSIGNED_10 = 0xE8;

constexpr auto BTN_TEXT_PAUSE = L"&Pause";
constexpr auto BTN_TEXT_RESUME = L"&Resume";
constexpr auto PREF_INI_FILE = L".\\StayAwake.ini";
constexpr auto PREF_DEFAULTS = L"Defaults";
constexpr auto PREF_AWAKE_KEYCODE = L"AwakeKeyCode";
constexpr auto PREF_AWAKE_PAUSED = L"AwakePaused";
constexpr auto PREF_START_MINIMIZED = L"StartMinimized";
constexpr auto PREF_MULTI_INSTANCE = L"MultipleInstancesAllowed";
constexpr auto PREF_TIMER_INTERVAL = L"TimerIntervalInSeconds";

constexpr auto MIN_PERIOD{ 10 };
constexpr auto MAX_PERIOD{ 9990 };

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
   bool m_bMinimized{};
   UINT_PTR m_TimerID{42};
   UINT m_AwakeKeyCode{ 0 };
   UINT m_TimerSeconds{ 240 };
   NOTIFYICONDATA m_TrayData{};
   CMenu m_menu;

   afx_msg LRESULT OnPostOpen(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
   afx_msg LRESULT OnRestoreDialog(WPARAM wParam, LPARAM lParam);
   afx_msg void OnTimer(UINT_PTR nIDEvent);
   afx_msg void OnKillfocusInterval();
   afx_msg void OnStayawakeKeyChange();
   afx_msg void OnSetInterval();
   afx_msg void OnPauseResume();
   afx_msg void OnMinimize() { MinimizeToTray(); };
   afx_msg void OnClickedAboutButton();
   afx_msg void OnEnter() { /* Ignore ENTER key press */ };
   afx_msg void OnCancel() { MinimizeToTray(); };
   afx_msg void OnRestore() { RestoreFromTray(); };
   afx_msg void OnExit() { DestroyWindow(); };
   afx_msg void OnDestroy();
   afx_msg void OnStartMinimized();

   void InitTimer();
   void InitTrayIcon();
   void MinimizeToTray();
   void OnTrayButtonDown(CPoint pt);
   void RestoreFromTray();
   void SimulateAwakeKeyPress();
   void ShowPausedInfo(bool both);

   wstring GetPreference(wstring key, wstring defaultVal);
   bool IsTimerPaused();
};
