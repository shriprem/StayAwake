#pragma once

#define WIN32_LEAN_AND_MEAN
#include <string>
#include <Psapi.h>

#pragma comment(lib, "version.lib")

#define PREFS_TIP_MAX_WIDTH         400

using std::string;
using std::wstring;
using std::to_wstring;

namespace Utils {
   HWND addTooltip(HINSTANCE hApp, HWND hDlg, int controlID, const wstring& pTitle, const wstring& pMessage, bool bBalloon = TRUE);
   HWND addTooltip(HINSTANCE hApp, HWND hDlg, int controlID, const wstring& pTitle, const wstring& pMessage, int duration, bool bBalloon);
   void loadBitmap(HINSTANCE hApp, HWND hDlg, int controlID, int resource);

   void addSecondsToTime(SYSTEMTIME& st, int seconds);
   wstring formatSystemTime(SYSTEMTIME& st, wstring prefix);

   int getProcessRunCount(wstring sBaseName);
   wstring getVersionInfo(HINSTANCE hApp, LPCWSTR key);
}
