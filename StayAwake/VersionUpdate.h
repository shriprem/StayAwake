#pragma once

#include <string>

using std::string;
using std::wstring;

constexpr auto GITHUB_API_HOST = L"api.github.com";
constexpr auto GITHUB_APP_PATH = L"/repos/shriprem/StayAwake/releases/latest";
constexpr auto GITHUB_API_TAGNAME = L"\"tag_name\"";

class VersionUpdate
{
public:
   wstring GetVersion();

private:
   wstring DownloadUrl(const wstring& host, const wstring& path);
   wstring ExtractTagNameValue(const wstring& json);
};

