#include "pch.h"
#include "StayAwakeCore.h"

void StayAwakeCore::SimulateInput(int inputCode, wstring& inputName)
{
   switch (inputCode)
   {
   case 0:
      inputName = L"Scroll Lock cycling";
      PressTwoKeys(VK_SCROLL, VK_SCROLL, false);
      break;

   case 1:
      inputName = L"Volume Down && Up";
      if (GetMuteState())
         PressThreeKeys(VK_VOLUME_DOWN, VK_VOLUME_UP, VK_VOLUME_MUTE, true);
      else
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
      inputName = L"Unassigned Key #" + to_wstring(inputCode - 1);
      PressOneKey(static_cast<BYTE>(VK_UNASSIGNED_01 + inputCode - 2), false);
      break;

   case 11:
      inputName = L"Unassigned Key #10";
      PressOneKey(VK_UNASSIGNED_10, false);
      break;

   case 13:
   case 14:
   case 15:
   case 16:
   case 17:
   case 18:
   case 19:
   case 20:
   case 21:
   case 22:
   case 23:
   case 24:
      inputName = L"F" + to_wstring(inputCode);
      PressOneKey(static_cast<BYTE>(VK_F13 + inputCode - 13), false);
      break;

   case 25:
      inputName = L"Invisible Mouse Move";
      MouseMove();
      break;


   default:
      inputName = L"Invalid Awake Key";
      break;
   }
}

ULONGLONG StayAwakeCore::GetIdleTimeMilliseconds()
{
   LASTINPUTINFO lii = {};
   lii.cbSize = sizeof(LASTINPUTINFO);

   if (!GetLastInputInfo(&lii))
      return 0; // error case

   return GetTickCount64() - lii.dwTime;
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

void StayAwakeCore::PressThreeKeys(BYTE vkFirst, BYTE vkSecond, BYTE vkThird, bool extended)
{
   INPUT input[6] = {};

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

   input[4].type = INPUT_KEYBOARD;
   input[4].ki.wVk = vkThird;
   input[4].ki.wScan = 0;
   input[4].ki.dwFlags = flags;

   input[5].type = INPUT_KEYBOARD;
   input[5].ki.wVk = vkThird;
   input[5].ki.wScan = 0;
   input[5].ki.dwFlags = KEYEVENTF_KEYUP | flags;

   SendInput(6, input, sizeof(INPUT));
}

void StayAwakeCore::MouseMove()
{
   INPUT input[2] = {};

   // Tiny move
   input[0].type = INPUT_MOUSE;
   input[0].mi.dx = 1;
   input[0].mi.dy = 1;
   input[0].mi.dwFlags = MOUSEEVENTF_MOVE;

   // Move back
   input[1].type = INPUT_MOUSE;
   input[1].mi.dx = -1;
   input[1].mi.dy = -1;
   input[1].mi.dwFlags = MOUSEEVENTF_MOVE;

   SendInput(2, input, sizeof(INPUT));
}

void StayAwakeCore::MouseMoveZero()
{
   INPUT input[1] = {};

   // Zero move
   input[0].type = INPUT_MOUSE;
   input[0].mi.dx = 0;
   input[0].mi.dy = 0;
   input[0].mi.dwFlags = MOUSEEVENTF_MOVE;

   SendInput(1, input, sizeof(INPUT));
}

bool StayAwakeCore::GetMuteState()
{
   HRESULT hr{};

   hr = CoInitialize(NULL);
   if (FAILED(hr)) return false;

   IMMDeviceEnumerator* enumerator = nullptr;
   IMMDevice* device = nullptr;
   IAudioEndpointVolume* endpointVolume = nullptr;

   hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL,
      CLSCTX_INPROC_SERVER,
      __uuidof(IMMDeviceEnumerator),
      (void**)&enumerator);
   if (FAILED(hr)) return false;

   hr = enumerator->GetDefaultAudioEndpoint(eRender, eConsole, &device);
   if (FAILED(hr)) { enumerator->Release(); return false; }

   hr = device->Activate(__uuidof(IAudioEndpointVolume),
      CLSCTX_INPROC_SERVER, NULL,
      (void**)&endpointVolume);
   if (FAILED(hr)) {
      device->Release();
      enumerator->Release();
      return false;
   }

   BOOL bMuted{};
   hr = endpointVolume->GetMute(&bMuted);
   if (FAILED(hr)) return false;

   return (bMuted==TRUE);
}

