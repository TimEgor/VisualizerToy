#include "Multithreading/LockGuard.h"

#include <cassert>

namespace VT
{
	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	Element* NamedObjectPool<Element, ElementReference, ElementPoolHandle>::addElementInternal(
		FileNameID nameID)
	{
		auto findNameIter = m_names.find(nameID);
		if (findNameIter != m_names.end())
		{
			return nullptr;
		}

		ElementInfo info = m_pool.addElementRaw();
		info.m_elementHandle.m_handle.setElementType(NAMED_ELEMENT_TYPE);

		m_names[nameID] = info.m_elementHandle.getKey();

		Element* handle
			= new (info.m_elementPtr) Element(nullptr, info.m_elementHandle.getKey(), nameID);

		return handle;
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	Element* NamedObjectPool<Element, ElementReference, ElementPoolHandle>::getElementInternal(
		FileNameID nameID)
	{
		auto findNameIter = m_names.find(nameID);
		if (findNameIter == m_names.end())
		{
			return nullptr;
		}

		return m_pool.getElement(findNameIter->second);
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	Element* NamedObjectPool<Element, ElementReference, ElementPoolHandle>::getElementInternal(
		ElementPoolHandleType handle)
	{
		return m_pool.getElement(handle);
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	bool NamedObjectPool<Element, ElementReference, ElementPoolHandle>::init(size_t pageSize,
		size_t maxFreePageCount, size_t minFreeIndexCount)
	{
		UniqueLockGuard locker(m_lockMutex);

		VT_CHECK_INITIALIZATION(m_pool.init(pageSize, maxFreePageCount, minFreeIndexCount));
		return true;
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	void NamedObjectPool<Element, ElementReference, ElementPoolHandle>::release()
	{
		UniqueLockGuard locker(m_lockMutex);

		m_pool.release();
		m_names = NamesContainer();
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	void NamedObjectPool<Element, ElementReference, ElementPoolHandle>::clear()
	{
		UniqueLockGuard locker(m_lockMutex);

		m_pool.clear();
		m_names.clear();
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	bool NamedObjectPool<Element, ElementReference, ElementPoolHandle>::isValid(
		ElementPoolHandleType handle) const
	{
		SharedLockGuard locker(m_lockMutex);

		return m_pool.isValid(handle);
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	ElementReference NamedObjectPool<Element, ElementReference, ElementPoolHandle>::getElement(
		ElementPoolHandleType handle) const
	{
		SharedLockGuard locker(m_lockMutex);
		return getElementInternal(handle);
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	ElementReference NamedObjectPool<Element, ElementReference, ElementPoolHandle>::getElement(
		ElementPoolHandleType handle)
	{
		SharedLockGuard locker(m_lockMutex);
		return getElementInternal(handle);
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	ElementReference NamedObjectPool<Element, ElementReference, ElementPoolHandle>::getElement(
		const FileName& name) const
	{
		return getElement(name.hash());
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	ElementReference NamedObjectPool<Element, ElementReference, ElementPoolHandle>::getElement(
		FileNameID nameID) const
	{
		SharedLockGuard locker(m_lockMutex);
		return getElementInternal(nameID);
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	ElementReference NamedObjectPool<Element, ElementReference, ElementPoolHandle>::getElement(
		const FileName& name)
	{
		return getElement(name.hash());
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	ElementReference NamedObjectPool<Element, ElementReference, ElementPoolHandle>::getElement(
		FileNameID nameID)
	{
		SharedLockGuard locker(m_lockMutex);
		return getElementInternal(nameID);
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	ElementReference NamedObjectPool<Element, ElementReference, ElementPoolHandle>::addElement(
		const FileName& name)
	{
		return addElement(name.hash());
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	ElementReference NamedObjectPool<Element, ElementReference, ElementPoolHandle>::addElement(
		FileNameID nameID)
	{
		UniqueLockGuard locker(m_lockMutex);
		return addElementInternal(nameID);
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	typename NamedObjectPool<Element, ElementReference, ElementPoolHandle>::ElementAccessInfo
		NamedObjectPool<Element, ElementReference, ElementPoolHandle>::getOrAddElement(const FileName& name)
	{
		return getOrAddElement(name.hash());
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	typename NamedObjectPool<Element, ElementReference, ElementPoolHandle>::ElementAccessInfo
		NamedObjectPool<Element, ElementReference, ElementPoolHandle>::getOrAddElement(FileNameID nameID)
	{
		UniqueLockGuard locker(m_lockMutex);

		Element* resource = getElementInternal(nameID);
		if (resource)
		{
			return ElementAccessInfo{ resource, false };
		}

		return ElementAccessInfo{ addElementInternal(nameID), true };
	}

	template <typename Element, typename ElementReference, typename ElementPoolHandle>
	void NamedObjectPool<Element, ElementReference, ElementPoolHandle>::removeElement(
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