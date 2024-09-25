#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"


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
