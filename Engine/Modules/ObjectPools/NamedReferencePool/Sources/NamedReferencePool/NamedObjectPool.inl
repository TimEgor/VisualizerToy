#include "Multithreading/LockGuard.h"

#include <cassert>

namespace VT
{
	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	Resource* NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::addResourceInternal(
		FileNameID nameID)
	{
		auto findNameIter = m_names.find(nameID);
		if (findNameIter != m_names.end())
		{
			return nullptr;
		}

		ResourceInfo info = m_pool.addElementRaw();
		info.m_elementHandle.m_handle.setResourceType(NAMED_RESOURCE_TYPE);

		m_names[nameID] = info.m_elementHandle.getKey();

		Resource* handle
			= new (info.m_elementPtr) Resource(nullptr, info.m_elementHandle.getKey(), nameID);

		return handle;
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	Resource* NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::getResourceInternal(
		FileNameID nameID)
	{
		auto findNameIter = m_names.find(nameID);
		if (findNameIter == m_names.end())
		{
			return nullptr;
		}

		return m_pool.getElement(findNameIter->second);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	Resource* NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::getResourceInternal(
		ResourcePoolHandle handle)
	{
		return m_pool.getElement(handle);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	bool NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::init(size_t pageSize,
		size_t maxFreePageCount, size_t minFreeIndexCount)
	{
		UniqueLockGuard locker(m_lockMutex);

		VT_CHECK_INITIALIZATION(m_pool.init(pageSize, maxFreePageCount, minFreeIndexCount));
		return true;
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	void NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::release()
	{
		UniqueLockGuard locker(m_lockMutex);

		m_pool.release();
		m_names = NamesContainer();
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	void NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::clear()
	{
		UniqueLockGuard locker(m_lockMutex);

		m_pool.clear();
		m_names.clear();
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	bool NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::isValid(
		ResourcePoolHandle handle) const
	{
		SharedLockGuard locker(m_lockMutex);

		return m_pool.isValid(handle);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		ResourcePoolHandle handle) const
	{
		SharedLockGuard locker(m_lockMutex);
		return getResourceInternal(handle);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		ResourcePoolHandle handle)
	{
		SharedLockGuard locker(m_lockMutex);
		return getResourceInternal(handle);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		const FileName& name) const
	{
		return getResource(name.hash());
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		FileNameID nameID) const
	{
		SharedLockGuard locker(m_lockMutex);
		return getResourceInternal(nameID);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		const FileName& name)
	{
		return getResource(name.hash());
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::getResource(
		FileNameID nameID)
	{
		SharedLockGuard locker(m_lockMutex);
		return getResourceInternal(nameID);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::addResource(
		const FileName& name)
	{
		return addResource(name.hash());
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	ResourceReference NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::addResource(
		FileNameID nameID)
	{
		UniqueLockGuard locker(m_lockMutex);
		return addResourceInternal(nameID);
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	typename NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::ResourceAccessInfo
		NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::getOrAddResource(const FileName& name)
	{
		return getOrAddResource(name.hash());
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	typename NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::ResourceAccessInfo
		NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::getOrAddResource(FileNameID nameID)
	{
		UniqueLockGuard locker(m_lockMutex);

		Resource* resource = getResourceInternal(nameID);
		if (resource)
		{
			return ResourceAccessInfo{ resource, false };
		}

		return ResourceAccessInfo{ addResourceInternal(nameID), true };
	}

	template <typename Resource, typename ResourceReference, typename ResourcePoolHandle>
	void NamedObjectPool<Resource, ResourceReference, ResourcePoolHandle>::removeResource(
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