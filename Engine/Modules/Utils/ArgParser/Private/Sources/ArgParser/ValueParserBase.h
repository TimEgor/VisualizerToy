#pragma once

#include "ArgParser/ValueParserInterface.h"

#include "Core/FileName/FileName.h"

#include <string>

namespace VT
{
	template<>
	inline void parseValue<int8_t>(const char* str, int8_t& val)
	{
		val = std::stoi(str);
	}

	template<>
	inline void parseValue<int16_t>(const char* str, int16_t& val)
	{
		val = std::stoi(str);
	}

	template<>
	inline void parseValue<int32_t>(const char* str, int32_t& val)
	{
		val = std::stoi(str);
	}

	template<>
	inline void parseValue<int64_t>(const char* str, int64_t& val)
	{
		val = std::stoll(str);
	}

	template<>
	inline void parseValue<uint8_t>(const char* str, uint8_t& val)
	{
		val = static_cast<uint8_t>(std::stoul(str));
	}

	template<>
	inline void parseValue<uint16_t>(const char* str, uint16_t& val)
	{
		val = static_cast<uint16_t>(std::stoul(str));
	}

	template<>
	inline void parseValue<uint32_t>(const char* str, uint32_t& val)
	{
		val = std::stoul(str);
	}

	template<>
	inline void parseValue<uint64_t>(const char* str, uint64_t& val)
	{
		val = std::stoull(str);
	}

	template<>
	inline void parseValue<float>(const char* str, float& val)
	{
		val = std::stof(str);
	}

	template<>
	inline void parseValue<double>(const char* str, double& val)
	{
		val = std::stod(str);
	}

	template<>
	inline void parseValue<char*>(const char* str, char** val)
	{
		strcpy(*val, str);
	}

	template<>
	inline void parseValue<std::string>(const char* str, std::string& val)
	{
		val = str;
	}

	template<>
	inline void parseValue<FileName>(const char* str, FileName& val)
	{
		val = str;
	}
}