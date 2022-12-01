#include "FileName.h"

#include "Core/HashFunctions/CRC32.h"

#include <cassert>

namespace VT
{
	void FileName::assign(const ResourceNameCharType* newName)
	{
		m_name = newName;

#if RESOURCE_NAME_LOWCASE
		size_t newSize = m_name.size();
		for (size_t i = 0; i < newSize; ++i)
		{
			m_name[i] = std::tolower(m_name[i]);
		}
#endif
	}

	FileName::FileName(const ResourceNameCharType* newName)
	{
		assign(newName);
	}

	FileName::FileName(ResourceNameCharType ch)
	{
#if RESOURCE_NAME_LOWCASE
		ch = std::tolower(ch);
#endif

		m_name = ch;
	}

	FileName::FileName(const std::string& str)
		: FileName(str.c_str())
	{}

	FileName::FileName(const FileName& newName)
		: FileName(newName.m_name)
	{}

	FileName& FileName::operator=(const ResourceNameCharType* newName)
	{
		assign(newName);
		return *this;
	}

	FileName& FileName::operator=(ResourceNameCharType ch)
	{
#if RESOURCE_NAME_LOWCASE
		m_name[0] = tolower(ch);
#else
		m_name[0] = ch;
#endif
		m_name[1] = '\0';

		return *this;
	}

	FileName& FileName::operator=(const std::string& str)
	{
		return operator=(str.c_str());
	}

	FileName& FileName::operator=(const FileName& newName)
	{
		return operator=(newName.m_name.c_str());
	}

	FileName& FileName::operator+=(const ResourceNameCharType* name)
	{
#if RESOURCE_NAME_LOWCASE
		size_t baseIndex = m_name.size();

		m_name += name;

		size_t newSize = m_name.size();
		for (size_t i = baseIndex; i < newSize; ++i)
		{
			m_name[i] = std::tolower(newName[i]);
		}

#else
		m_name += name;
#endif

		return *this;
	}

	FileName& FileName::operator+=(ResourceNameCharType ch)
	{
		m_name += ch;
		return *this;
	}

	FileName& FileName::operator+=(const std::string& str)
	{
		return operator+=(str.c_str());
	}

	FileName& FileName::operator+=(const FileName& name)
	{
		return operator+=(name.m_name);
	}

	const FileName::ResourceNameCharType& FileName::operator[](size_t index) const
	{
		return m_name[index];
	}

	FileName::ResourceNameCharType& FileName::operator[](size_t index)
	{
		return m_name[index];
	}

	size_t FileName::length() const
	{
		return m_name.size();
	}

	FileNameID FileName::hash() const
	{
		return crc32(m_name.c_str());
	}

	void FileName::clear()
	{
		m_name.clear();
	}

	void FileName::release()
	{
		m_name = ResourceNameContainerType();
	}

	FileName operator+(const FileName& resName, const FileName::ResourceNameCharType* name)
	{
		FileName newName(resName);
		newName += name;
		return newName;
	}

	FileName operator+(const FileName::ResourceNameCharType* name, const FileName& resName)
	{
		FileName newName(name);
		newName += resName;
		return newName;
	}

	FileName operator+(const FileName& resName, FileName::ResourceNameCharType ch)
	{
		FileName newName(resName);
		newName += ch;
		return newName;
	}

	FileName operator+(FileName::ResourceNameCharType ch, const FileName& resName)
	{
		FileName newName(ch);
		newName += resName;
		return newName;
	}

	FileName operator+(const FileName& resName, const std::string& str)
	{
		FileName newName(resName);
		newName += str;
		return newName;
	}

	FileName operator+(const std::string& str, const FileName& resName)
	{
		FileName newName(str);
		newName += resName;
		return newName;
	}

	FileName operator+(const FileName& resName1, const FileName& resName2)
	{
		FileName newName(resName1);
		newName += resName2;
		return newName;
	}

	void parseValue(const char* strVal, FileName& value)
	{
		value = strVal;
	}
}