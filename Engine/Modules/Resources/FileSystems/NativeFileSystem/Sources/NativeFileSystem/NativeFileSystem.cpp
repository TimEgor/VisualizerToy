#include "NativeFileSystem.h"

#include "Core/FileName/FileNameUtils.h"

#include <filesystem>
#include <fstream>
#include <cassert>

namespace VT
{
	bool NativeFileSystem::loadRawDataBase(const FileName& resourceName, void* data, size_t size, OpenMode mode) const
	{
		FileName targetResourceName;
		prepareTargetName(resourceName, targetResourceName);

		std::fstream fileStream(targetResourceName.c_str(), static_cast<std::ios_base::openmode>(mode));
		assert(fileStream.is_open());

		fileStream.read(reinterpret_cast<char*>(data), size);

		fileStream.close();

		return true;
	}

	void NativeFileSystem::prepareTargetName(const FileName& resourceName, FileName& targetName) const
	{
		targetName = m_rootPath + resourceName;
	}

	void NativeFileSystem::createDirectoryInternal(const FileName& path)
	{
		std::filesystem::create_directories(path.c_str());
	}

	bool NativeFileSystem::isDirectoryInternal(const FileName& path) const
	{
		return std::filesystem::is_directory(path.c_str());
	}

	bool NativeFileSystem::existInternal(const FileName& path) const
	{
		return std::filesystem::exists(path.c_str());
	}

	bool NativeFileSystem::init(const FileName& path)
	{
		m_rootPath = path + "/";
		return true;
	}

	void NativeFileSystem::release()
	{
		m_rootPath.release();
	}

	bool NativeFileSystem::getResourceSize(const FileName& resourceName, size_t& resourceSize) const
	{
		FileName targetResourceName;
		prepareTargetName(resourceName, targetResourceName);

		resourceSize = static_cast<size_t>(std::filesystem::file_size(targetResourceName.c_str()));
		return true;
	}

	bool NativeFileSystem::readResourceData(const FileName& resourceName, void* data, size_t resourceSize) const
	{
		return loadRawDataBase(resourceName, data, resourceSize, std::ios_base::in);
	}

	void* NativeFileSystem::readResourceData(const FileName& resourceName, size_t& resourceSize) const
	{
		if (!getResourceSize(resourceName, resourceSize) && resourceSize > 0)
		{
			return nullptr;
		}

		uint8_t* data = new uint8_t[resourceSize];

		return readResourceData(resourceName, data, resourceSize) ? data : nullptr;
	}

	bool NativeFileSystem::readResourceBinaryData(const FileName& resourceName, void* data, size_t resourceSize) const
	{
		return loadRawDataBase(resourceName, data, resourceSize, std::ios_base::in | std::ios_base::binary);
	}

	void* NativeFileSystem::readResourceBinaryData(const FileName& resourceName, size_t& resourceSize) const
	{
		if (!getResourceSize(resourceName, resourceSize) && resourceSize > 0)
		{
			return nullptr;
		}

		uint8_t* data = new uint8_t[resourceSize];

		return readResourceBinaryData(resourceName, data, resourceSize) ? data : nullptr;
	}

	bool NativeFileSystem::writeResource(const FileName& resourceName, void* data, size_t resourceSize, WriteResourceFileFlag flag)
	{
		FileName targetResourceName;
		prepareTargetName(resourceName, targetResourceName);

		const FileName parentPath = FileNameUtils::getFileDirPath(resourceName);
		if (!existInternal(parentPath))
		{
			createDirectoryInternal(parentPath);
		}

		std::fstream fileStream(targetResourceName.c_str(),
			std::ios_base::out | ((flag == WriteResourceFileFlag::APPEND ? std::ios_base::app : std::ios_base::trunc) | std::ios_base::binary));
		assert(fileStream.is_open());

		fileStream.write(reinterpret_cast<char*>(data), resourceSize);

		fileStream.close();

		return true;
	}

	void NativeFileSystem::createDirectory(const FileName& path)
	{
		FileName targetPath;
		prepareTargetName(path, targetPath);

		createDirectoryInternal(targetPath);
	}

	bool NativeFileSystem::isDirectory(const FileName& path) const
	{
		FileName targetResourceName;
		prepareTargetName(path, targetResourceName);

		return isDirectoryInternal(targetResourceName);
	}

	bool NativeFileSystem::exist(const FileName& resourceName) const
	{
		FileName targetResourceName;
		prepareTargetName(resourceName, targetResourceName);

		return existInternal(targetResourceName);
	}

	IFileSystem::FileTime NativeFileSystem::getModificationTime(const FileName& resourceName) const
	{
		FileName targetResourceName;
		prepareTargetName(resourceName, targetResourceName);

		return std::filesystem::last_write_time(targetResourceName.c_str()).time_since_epoch().count();
	}
}
