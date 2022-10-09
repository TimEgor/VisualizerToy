#pragma once

#include <string>
#include <locale>
#include <codecvt>

namespace VT
{
	inline std::wstring convertStringToWString(const std::string& str)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterer;
		return converterer.from_bytes(str);
	}

	inline std::string convertWStringToString(const std::wstring& wstr)
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
		return converterX.to_bytes(wstr);
	}
}