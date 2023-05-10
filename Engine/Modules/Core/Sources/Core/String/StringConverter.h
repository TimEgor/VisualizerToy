#pragma once

#include <string>

namespace VT
{
	std::wstring convertStringToWString(const std::string& str);
	std::string convertWStringToString(const std::wstring& wstr);

	template <typename T>
	std::string convertToString(T val)
	{
		return std::to_string(val);
	}
}
