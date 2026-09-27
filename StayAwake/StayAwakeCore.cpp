#include "pch.h"
#include "StayAwakeCore.h"

void StayAwakeCore::SimulateInput(int inputCode, wstring& inputName)
{
   switch (inputCode)
   {
   case 1:
      inputName = L"Volume Down && Up";
      keybd_event(VK_VOLUME_DOWN, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_VOLUME_DOWN, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      Sleep(10);
      keybd_event(VK_VOLUME_UP, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_VOLUME_UP, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
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
      BYTE keycode{ static_cast<BYTE>(VK_UNASSIGNED_01 + inputCode - 2) };
      keybd_event(keycode, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(keycode, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      break;
   }

   case 11:
      inputName = L"Unassigned Key #10";
      keybd_event(VK_UNASSIGNED_10, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_UNASSIGNED_10, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      break;

   default:
      inputName = L"Scroll Lock cycling";
      keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      Sleep(10);
      keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | 0, 0);
      keybd_event(VK_SCROLL, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
      break;
   }
}
