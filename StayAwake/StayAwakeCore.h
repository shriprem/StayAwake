#pragma once
#include <string>
#include <windows.h>

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

constexpr auto MIN_PERIOD{ 10 };
constexpr auto MAX_PERIOD{ 9990 };
constexpr auto DEF_PERIOD{ 240 };

constexpr auto LEN_ROSTER_KEYCODES{ 12 };
constexpr auto DEF_SELECTED_KEYCODES = L"001111111111";


using std::string;
using std::wstring;
using std::to_wstring;


class StayAwakeCore
{
public:
   StayAwakeCore() {};
   ~StayAwakeCore() {};

   void SimulateInput(int inputCode, wstring& inputName);
   static ULONGLONG GetIdleTimeMilliseconds();

private:
   static void PressOneKey(BYTE vk, bool extended = false);
   static void PressTwoKeys(BYTE vkFirst, BYTE vkSecond, bool extended = false);
   static void MouseMove();
   static void MouseMoveZero();
};

