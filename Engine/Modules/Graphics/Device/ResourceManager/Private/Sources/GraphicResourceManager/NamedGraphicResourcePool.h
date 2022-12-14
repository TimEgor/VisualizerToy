#pragma once

#include "ObjectPool/ObjectPool.h"
#include "Multithreading/Mutex.h"
#include "Multithreading/LockGuard.h"
#include "Core/FileName/FileName.h"

#include <unordered_map>

namespace VT
{
	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	class NamedGraphicResourcePool final
	{
		using ObjectPoolType  = ObjectPool<Resource, ResourcePoolHandle>;
		using NamesContainer = std::unordered_map<FileNameID, typename ResourcePoolHandle::KeyType>;

	public:
		using NewResourceInfo = typename ObjectPoolType::NewElementInfo;
		using ResourcePoolHandleType = ResourcePoolHandle;

	private:
		ObjectPoolType m_pool;
		NamesContainer m_names;
		mutable SharedMutex m_lockMutex;

	public:
		NamedGraphicResourcePool() = default;

		bool init(size_t pageSize = 128, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64);
		void release();

		void clear();

		bool isValid(ResourcePoolHandle handle) const;

		ResourceReference getResource(ResourcePoolHandle handle) const;
		ResourceReference getResource(ResourcePoolHandle handle);

		ResourceReference getResource(const FileName& name) const;
		ResourceReference getResource(FileNameID nameID) const;
		ResourceReference getResource(const FileName& name);
		ResourceReference getResource(FileNameID nameID);

		void addResource(NewResourceInfo& info, const FileName& name);
		void addResource(NewResourceInfo& info, FileNameID nameID);
		NewResourceInfo addResource(const FileName& name);
		NewResourceInfo addResource(FileNameID nameID);

		void removeResource(FileNameID nameID);
	};

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	bool NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::init(size_t pageSize,
		size_t maxFreePageCount, size_t minFreeIndexCount)
	{
		VT_CHECK_INITIALIZATION(m_pool.init(pageSize, maxFreePageCount, minFreeIndexCount));
		return true;
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	void NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::release()
	{
		UniqueLockGuard locker(m_lockMutex);

		m_pool.release();
		m_names = NamesContainer();
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	void NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::clear()
	{
		UniqueLockGuard locker(m_lockMutex);

		m_pool.clear();
		m_names = clear();
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	bool NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::isValid(
		ResourcePoolHandle handle) const
	{
		SharedLockGuard locker(m_lockMutex);

		return m_pool.isValid(handle);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		ResourcePoolHandle handle) const
	{
		SharedLockGuard locker(m_lockMutex);

		return m_pool.getElement(handle);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		ResourcePoolHandle handle)
	{
		SharedLockGuard locker(m_lockMutex);

		return m_pool.getElement(handle);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		const FileName& name) const
	{
		return getResource(name.hash());
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		FileNameID nameID) const
	{
		SharedLockGuard locker(m_lockMutex);

		auto findNameIter = m_names.find(nameID);
		if (findNameIter == m_names.end())
		{
			return nullptr;
		}

		return m_pool.getElement(findNameIter->second);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		const FileName& name)
	{
		return getResource(name.hash());
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		FileNameID nameID)
	{
		SharedLockGuard locker(m_lockMutex);

		auto findNameIter = m_names.find(nameID);
		if (findNameIter == m_names.end())
		{
			return nullptr;
		}

		return m_pool.getElement(findNameIter->second);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	void NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::addResource(NewResourceInfo& info,
		const FileName& name)
	{
		addResource(info, name.hash());
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	void NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::addResource(NewResourceInfo& info,
		FileNameID nameID)
	{
		UniqueLockGuard locker(m_lockMutex);

		auto findNameIter = m_names.find(nameID);
		if (findNameIter != m_names.end())
		{
			assert(false);

			info = NewResourceInfo{};
			return;
		}

		m_pool.addElementRaw(info);
		info.m_elementHandle.m_handle.setResourceType(NAMED_RESOURCE_TYPE);

		m_names[nameID] = info.m_elementHandle.getKey();
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	typename NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::NewResourceInfo
	NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::addResource(const FileName& name)
	{
		return addResource(name.hash());
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	typename NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::NewResourceInfo
	NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::addResource(FileNameID nameID)
	{
		UniqueLockGuard locker(m_lockMutex);

		auto findNameIter = m_names.find(nameID);
		if (findNameIter != m_names.end())
		{
			assert(false);

			return NewResourceInfo{};
		}

		NewResourceInfo info = m_pool.addElementRaw();
		info.m_elementHandle.m_handle.setResourceType(NAMED_RESOURCE_TYPE);

		m_names[nameID] = info.m_elementHandle.getKey();

		return info;
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	void NamedGraphicResourcePool<Resource, ResourceReference, ResourcePoolHandle>::removeResource(
		FileNameID nameID)
	{
		UniqueLockGuard locker(m_lockMutex);

		auto findNameIter = m_names.find(nameID);
		if (findNameIter == m_names.end())
		{
			assert(false);
			return;
		}

		m_pool.removeElement(findNameIter->second);
		m_names.erase(findNameIter);
	}
}
