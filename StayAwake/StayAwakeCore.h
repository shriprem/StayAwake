#pragma once

#include <mmdeviceapi.h>
#include <endpointvolume.h>
#include <string>
#include <windows.h>
#include <wtsapi32.h>

#pragma comment(lib, "Ole32.lib")
#pragma comment(lib, "Wtsapi32.lib")

using std::string;
using std::wstring;
using std::to_wstring;


constexpr auto VK_UNASSIGNED_01 = 0x97;
constexpr auto VK_UNASSIGNED_10 = 0xE8;

constexpr auto PREF_INI_FILE = L"StayAwake.ini";
constexpr auto PREF_DEFAULTS = L"Defaults";
constexpr auto PREF_LEGACY_KEYCODE = L"AwakeKeyCode";
constexpr auto PREF_SELECTED_KEYCODES = L"SelectedKeyCodes";
constexpr auto PREF_AWAKE_PAUSED = L"AwakePaused";
constexpr auto PREF_START_MINIMIZED = L"StartMinimized";
constexpr auto PREF_MULTI_INSTANCE = L"MultipleInstancesAllowed";
constexpr auto PREF_INTERVAL_LEGACY = L"TimerIntervalInSeconds";
constexpr auto PREF_INTERVAL_MINIMUM = L"MinimumIntervalInSeconds";
constexpr auto PREF_INTERVAL_MAXIMUM = L"MaximumIntervalInSeconds";
constexpr auto PREF_MOUSE_MOVE_ZERO = L"MouseMoveZeroPixels";

constexpr auto MIN_PERIOD{ 10 };
constexpr auto MAX_PERIOD{ 9990 };
constexpr auto DEF_PERIOD{ 240 };

constexpr auto LEN_ROSTER_KEYCODES{ 25 };
const auto DEF_SELECTED_KEYCODES = L"00" + wstring(LEN_ROSTER_KEYCODES - 2, L'1');



class StayAwakeCore
{
public:
   StayAwakeCore() {};
   ~StayAwakeCore() {};

   void SetConfigFilePath(LPTSTR iniFilePath);

   wstring GetSelectedKeyCodes();
   bool CheckSelectedKeyCodes(wstring sKeyCodes);
   bool SaveSelectedKeyCodes(wstring sKeyCodes);
   void InitIntervals(UINT& minSeconds, UINT& maxSeconds) const;
   wstring GetPreference(wstring key, wstring defaultVal) const;
   bool SetPreference(wstring key, wstring setVal) const;

   void SimulateInput(int inputCode, wstring& inputName);
   ULONGLONG GetIdleTimeMilliseconds();

private:
   wchar_t sIniFilePath[MAX_PATH]{};

   void PressOneKey(BYTE vk, bool extended = false);
   void PressTwoKeys(BYTE vkFirst, BYTE vkSecond, bool extended = false);
   void PressThreeKeys(BYTE vkFirst, BYTE vkSecond, BYTE vkThird, bool extended = false);
   void MouseMoveTiny();
   void MouseMoveZero();

   bool GetMuteState();
};

