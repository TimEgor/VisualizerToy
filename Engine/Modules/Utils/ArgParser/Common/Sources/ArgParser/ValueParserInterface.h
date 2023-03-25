#pragma once

namespace VT
{
	namespace ArgParserInterface
	{
		template <typename T>
		void parseValue(const char* str, T& val)
		{
			static_assert(false && "Invalid type for Args parser");
		}

		template <typename T>
		void parseValue(const char* str, T* val)
		{
			static_assert(false && "Invalid type for Args parser");
		}
	}
}