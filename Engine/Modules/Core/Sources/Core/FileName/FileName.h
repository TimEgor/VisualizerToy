#pragma once

#include <string>

#define VT_FILE_NAME_LOWCASE FALSE

namespace VT
{
	using FileNameID = uint32_t;
	constexpr FileNameID InvalidFileNameID = 0;

	class FileName final
	{
	public:
		using ResourceNameCharType = char;
		using ResourceNameContainerType = std::basic_string<ResourceNameCharType>;

	private:
		ResourceNameContainerType m_name;

		void assign(const ResourceNameCharType* name);

	public:
		FileName() = default;
		FileName(const ResourceNameCharType* newName);
		FileName(ResourceNameCharType ch);
		FileName(const std::string& str);
		FileName(const FileName& newName);

		FileName& operator=(const ResourceNameCharType* name);
		FileName& operator=(ResourceNameCharType ch);
		FileName& operator=(const std::string& str);
		FileName& operator=(const FileName& name);
		FileName& operator+=(const ResourceNameCharType* name);
		FileName& operator+=(ResourceNameCharType ch);
		FileName& operator+=(const std::string& str);
		FileName& operator+=(const FileName& name);

		operator bool() const { return !empty(); }

		const ResourceNameCharType& operator[](size_t index) const;
		ResourceNameCharType& operator[](size_t index);

		const ResourceNameCharType* c_str() const { return m_name.c_str(); }
		size_t length() const;
		bool empty() const { return m_name[0] == '\0'; }

		void clear();
		void release();

		FileNameID hash() const;
	};

	FileName operator+(const FileName& resName, const FileName::ResourceNameCharType* name);
	FileName operator+(const FileName::ResourceNameCharType* name, const FileName& resName);
	FileName operator+(const FileName& resName, FileName::ResourceNameCharType ch);
	FileName operator+(FileName::ResourceNameCharType ch, const FileName& resName);
	FileName operator+(const FileName& resName, const std::string& str);
	FileName operator+(const std::string& str, const FileName& resName);
	FileName operator+(const FileName& resName1, const FileName& resName2);
}