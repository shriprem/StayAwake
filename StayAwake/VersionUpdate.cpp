#include "pch.h"

#include "VersionUpdate.h"
#include <winhttp.h>

#pragma comment(lib, "winhttp.lib")

wstring VersionUpdate::DownloadUrl(const wstring& host, const wstring& path)
{
   wstring response{};

   HINTERNET hSession =
      WinHttpOpen(L"StayAwake Update Checker/1.0",
         WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
         WINHTTP_NO_PROXY_NAME,
         WINHTTP_NO_PROXY_BYPASS,
         0);

   if (!hSession) return response;

   HINTERNET hConnect = WinHttpConnect(hSession, host.c_str(), INTERNET_DEFAULT_HTTPS_PORT, 0);

   if (!hConnect)
   {
      WinHttpCloseHandle(hSession);
      return response;
   }

   HINTERNET hRequest =
      WinHttpOpenRequest(hConnect, L"GET", path.c_str(), nullptr, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);

   if (!hRequest)
   {
      WinHttpCloseHandle(hConnect);
      WinHttpCloseHandle(hSession);
      return response;
   }

   // GitHub likes having a User-Agent.
   BOOL bSuccess = WinHttpSendRequest(hRequest, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);

   if (bSuccess)
      bSuccess = WinHttpReceiveResponse(hRequest, nullptr);

   if (bSuccess)
   {
      DWORD dwSize = 0;

      do
      {
         dwSize = 0;

         if (!WinHttpQueryDataAvailable(hRequest, &dwSize))
            break;

         if (dwSize == 0)
            break;

         string buffer(dwSize, '\0');

         DWORD dwDownloaded = 0;

         if (!WinHttpReadData(hRequest, buffer.data(), dwSize, &dwDownloaded))
            break;

         response.append(wstring(buffer.begin(), buffer.begin() + dwDownloaded));
      } while (dwSize > 0);
   }

   WinHttpCloseHandle(hRequest);
   WinHttpCloseHandle(hConnect);
   WinHttpCloseHandle(hSession);

   return response;
}

wstring VersionUpdate::ExtractTagNameValue(const wstring& json)
{
   const wchar_t* tagStart = wcsstr(json.c_str(), GITHUB_API_TAGNAME);
   if (!tagStart) return L"";

   const wchar_t* valStart = wcschr(tagStart + wcslen(GITHUB_API_TAGNAME), L'"');
   if (!valStart) return L"";
   ++valStart; // Move past the first quote

   const wchar_t* valEnd = wcschr(valStart, L'"');
   if (!valEnd) return L"";

   return wstring (valStart, valEnd - valStart);
}

wstring VersionUpdate::GetVersion()
{
   auto json = DownloadUrl(GITHUB_API_HOST, GITHUB_APP_PATH);
   if (json.empty()) return L"";

   wstring tag{ ExtractTagNameValue(json) };
   if (!tag.empty() && tag[0] == L'v')
      tag.erase(0, 1); // Remove leading 'v'
   return tag;
}
