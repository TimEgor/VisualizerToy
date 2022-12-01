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

		bool loadRawDataBase(const FileName& resourceName, void* data, size_t size, OpenMode mode);
		void prepareTargetName(const FileName& resourceName, FileName& targetName) const;

	public:
		NativeFileSystem() {}
		virtual ~NativeFileSystem() {}

		virtual bool init(const FileName& path = "") override;
		virtual void release() override;

		virtual bool getResourceSize(const FileName& resourceName, size_t& resourceSize) override;
		virtual bool readResourceData(const FileName& resourceName, void* data, size_t resourceSize) override;
		virtual bool readResourceBinaryData(const FileName& resourceName, void* data, size_t resourceSize) override;

		virtual bool writeResource(const FileName& resourceName, void* data, size_t resourceSize, WriteResourceFileFlag flag) override;

		virtual bool isDirectory(const FileName& resourceName) const override;
		virtual bool exist(const FileName& resourceName) const override;
	};
}