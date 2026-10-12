#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"

constexpr auto LAST_CODE_UPDATE_TIME = L"Oct 12 2026 - 12:59:31";


class CStayAwakeApp : public CWinApp
{
public:
	const UINT WM_SHOWFIRSTINSTANCE{RegisterWindowMessage(L"WM_SHOWFIRSTINSTANCE|StayAwake_Restore_Tray_Icon")};
	CStayAwakeApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CStayAwakeApp theApp;
