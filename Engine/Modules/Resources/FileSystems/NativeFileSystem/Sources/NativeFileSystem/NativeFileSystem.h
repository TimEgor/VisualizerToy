#pragma once

#include "FileSystem/IFileSystem.h"
#include "Core/FileName/FileName.h"

namespace VT
{
	class NativeFileSystem final : public IFileSystem
	{
		using OpenMode = int;

	private:
		FileName m_rootPath;

		bool loadRawDataBase(const FileName& resourceName, void* data, size_t size, OpenMode mode) const;
		void prepareTargetName(const FileName& resourceName, FileName& targetName) const;

		void createDirectoryInternal(const FileName& path);
		bool isDirectoryInternal(const FileName& path) const;
		bool existInternal(const FileName& path) const;

	public:
		NativeFileSystem() = default;
		virtual ~NativeFileSystem() {}

		virtual bool init() override;
		virtual bool init(const FileName& path) override;
		virtual void release() override;

		virtual bool getResourceSize(const FileName& resourceName, size_t& resourceSize) const override;
		virtual bool readResourceData(const FileName& resourceName, void* data, size_t resourceSize) const override;
		virtual void* readResourceData(const FileName& resourceName, size_t& resourceSize) const override;
		virtual bool readResourceBinaryData(const FileName& resourceName, void* data, size_t resourceSize) const override;
		virtual void* readResourceBinaryData(const FileName& resourceName, size_t& resourceSize) const override;

		virtual bool writeResource(const FileName& resourceName, void* data, size_t resourceSize, WriteResourceFileFlag flag) override;

		virtual IWriteStream* openWriteStream(const FileName& resourceName) const override;
		virtual IWriteStream* openBinaryWriteStream(const FileName& resourceName) const override;
		virtual void closeWriteStream(IWriteStream* stream) const override;

		virtual IReadStream* openReadStream(const FileName& resourceName) const override;
		virtual IReadStream* openBinaryReadStream(const FileName& resourceName) const override;
		virtual void closeReadStream(IReadStream* stream) const override;

		virtual IReadWriteStream* openReadWriteStream(const FileName& resourceName) const override;
		virtual IReadWriteStream* openBinaryReadWriteStream(const FileName& resourceName) const override;
		virtual void closeReadWriteStream(IReadWriteStream* stream) const override;

		virtual void createDirectory(const FileName& path) override;

		virtual bool isDirectory(const FileName& path) const override;
		virtual bool exist(const FileName& resourceName) const override;

		virtual FileTime getModificationTime(const FileName& resourceName) const;
	};
}