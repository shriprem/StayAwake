#include "pch.h"
#include "StayAwakeCore.h"

void StayAwakeCore::SimulateInput(int inputCode, wstring& inputName)
{
   switch (inputCode)
   {
   case 1:
      inputName = L"Volume Down && Up";
      PressTwoKeys(VK_VOLUME_DOWN, VK_VOLUME_UP, true);
      break;

   case 2:
   case 3:
   case 4:
   case 5:
   case 6:
   case 7:
   case 8:
   case 9:
   case 10:
   {
      inputName = L"Unassigned Key #" + to_wstring(inputCode - 1);
      PressOneKey(static_cast<BYTE>(VK_UNASSIGNED_01 + inputCode - 2), false);
      break;
   }

   case 11:
      inputName = L"Unassigned Key #10";
      PressOneKey(VK_UNASSIGNED_10, false);
      break;

   default:
      inputName = L"Scroll Lock cycling";
      PressTwoKeys(VK_SCROLL, VK_SCROLL, false);
      break;
   }
}

void StayAwakeCore::PressOneKey(BYTE vk, bool extended)
{
   INPUT input[2] = {};

   auto flags = (extended) ? KEYEVENTF_EXTENDEDKEY : 0;

   input[0].type = INPUT_KEYBOARD;
   input[0].ki.wVk = vk;
   input[0].ki.wScan = 0;
   input[0].ki.dwFlags = flags;

   input[1].type = INPUT_KEYBOARD;
   input[1].ki.wVk = vk;
   input[1].ki.wScan = 0;
   input[1].ki.dwFlags = KEYEVENTF_KEYUP | flags;

   SendInput(2, input, sizeof(INPUT));
}

void StayAwakeCore::PressTwoKeys(BYTE vkFirst, BYTE vkSecond, bool extended)
{
   INPUT input[4] = {};

   auto flags = (extended) ? KEYEVENTF_EXTENDEDKEY : 0;

   input[0].type = INPUT_KEYBOARD;
   input[0].ki.wVk = vkFirst;
   input[0].ki.wScan = 0;
   input[0].ki.dwFlags = flags;

   input[1].type = INPUT_KEYBOARD;
   input[1].ki.wVk = vkFirst;
   input[1].ki.wScan = 0;
   input[1].ki.dwFlags = KEYEVENTF_KEYUP | flags;

   input[2].type = INPUT_KEYBOARD;
   input[2].ki.wVk = vkSecond;
   input[2].ki.wScan = 0;
   input[2].ki.dwFlags = flags;

   input[3].type = INPUT_KEYBOARD;
   input[3].ki.wVk = vkSecond;
   input[3].ki.wScan = 0;
   input[3].ki.dwFlags = KEYEVENTF_KEYUP | flags;

   SendInput(4, input, sizeof(INPUT));
}
