#pragma once

#include <cstdint>

namespace VT
{
	enum class WriteResourceFileFlag
	{
		APPEND,
		OVERRIDE
	};

	class FileName;

	class IFileSystem
	{
	public:
		IFileSystem() {}
		virtual ~IFileSystem() {}

		virtual bool init(const FileName& path) = 0;
		virtual void release() = 0;

		virtual bool getResourceSize(const FileName& resourceName, size_t& resourceSize) const = 0;
		virtual bool readResourceData(const FileName& resourceName, void* data, size_t resourceSize) const = 0;
		virtual void* readResourceData(const FileName& resourceName, size_t& resourceSize) const = 0;
		virtual bool readResourceBinaryData(const FileName& resourceName, void* data, size_t resourceSize) const = 0;
		virtual void* readResourceBinaryData(const FileName& resourceName, size_t& resourceSize) const = 0;

		virtual bool writeResource(const FileName& resourceName, void* data, size_t resourceSize, WriteResourceFileFlag flag) = 0;

		virtual void createDirectory(const FileName& resourceName) = 0;

		virtual bool isDirectory(const FileName& resourceName) const = 0;
		virtual bool exist(const FileName& resourceName) const = 0;

		virtual bool isIndexed() const { return false; }
	};

	class IIndexedFileSystem : public IFileSystem
	{
		using ResourceID = uint32_t;

	public:
		IIndexedFileSystem() {}

		virtual bool getResourceSize(ResourceID resourceID, size_t& resourceSize) const = 0;
		virtual bool readResourceData(ResourceID resourceID, void* data, size_t resourceSize) const = 0;
		virtual void* readResourceData(ResourceID resourceID, size_t& resourceSize) const = 0;
		virtual bool readResourceBinaryData(ResourceID resourceID, void* data, size_t resourceSize) const = 0;
		virtual void* readResourceBinaryData(ResourceID resourceID, size_t& resourceSize) const = 0;

		virtual bool writeResource(ResourceID resourceID, void* data, size_t resourceSize, WriteResourceFileFlag flag) = 0;

		virtual FileName getResourceName(ResourceID resourceID) const = 0;

		virtual bool isDirectory(ResourceID resourceID) const = 0;
		virtual bool exist(ResourceID resourceID) const = 0;

		virtual bool isIndexed() const override { return true; }
	};
}