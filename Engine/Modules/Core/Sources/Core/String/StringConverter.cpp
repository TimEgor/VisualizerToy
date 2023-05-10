#include "StringConverter.h"

#include <locale>
#include <codecvt>

std::wstring VT::convertStringToWString(const std::string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterer;
	return converterer.from_bytes(str);
}

std::string VT::convertWStringToString(const std::wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}
