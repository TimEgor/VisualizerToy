#pragma once

namespace VT
{
	template <typename T>
	void parseValue(const char* str, T& val);

	template <typename T>
	void parseValue(const char* str, T* val);
}