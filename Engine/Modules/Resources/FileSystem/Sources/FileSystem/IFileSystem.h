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
		using FileTime = long long;

		class IStream
		{
		public:
			enum class SeekDir
			{
				BEGIN,
				CURRENT,
				END
			};

		public:
			IStream() = default;
			virtual ~IStream() = default;

			virtual bool isOpen() const = 0;
			virtual size_t getPos() = 0;
			virtual void seek(size_t pos, SeekDir seekPos) = 0;
			virtual int getError() const = 0;
		};

		class IWriteStream : virtual public IStream
		{
		public:
			IWriteStream() = default;

			virtual size_t write(const void* data, size_t dataSize) = 0;
		};

		class IReadStream : virtual public IStream
		{
		public:
			IReadStream() = default;

			virtual size_t read(void* data, size_t dataSize) = 0;
		};

		class IReadWriteStream : public IWriteStream, public IReadStream
		{
		public:
			IReadWriteStream() = default;
		};

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

		virtual IWriteStream* openWriteStream(const FileName& resourceName) const = 0;
		virtual IWriteStream* openBinaryWriteStream(const FileName& resourceName) const = 0;
		virtual void closeWriteStream(IWriteStream* stream) const = 0;

		virtual IReadStream* openReadStream(const FileName& resourceName) const = 0;
		virtual IReadStream* openBinaryReadStream(const FileName& resourceName) const = 0;
		virtual void closeReadStream(IReadStream* stream) const = 0;

		virtual IReadWriteStream* openReadWriteStream(const FileName& resourceName) const = 0;
		virtual IReadWriteStream* openBinaryReadWriteStream(const FileName& resourceName) const = 0;
		virtual void closeReadWriteStream(IReadWriteStream* stream) const = 0;

		virtual void createDirectory(const FileName& resourceName) = 0;

		virtual bool isDirectory(const FileName& resourceName) const = 0;
		virtual bool exist(const FileName& resourceName) const = 0;

		virtual FileTime getModificationTime(const FileName& resourceName) const = 0;

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

		virtual FileTime getModificationTime(ResourceID resourceID) const = 0;

		virtual bool isIndexed() const override { return true; }
	};
}