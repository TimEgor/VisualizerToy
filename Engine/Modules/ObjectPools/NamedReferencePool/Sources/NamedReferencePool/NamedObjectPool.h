#pragma once

#include "ObjectPool/ObjectPool.h"
#include "Multithreading/Mutex.h"
#include "Core/FileName/FileName.h"

#include <unordered_map>

namespace VT
{
	template <typename ElementHandle, typename ElementHandleReference, typename ElementPoolHandle>
	class NamedObjectPool final
	{
		using ObjectPoolType  = ObjectPool<ElementHandle, ElementPoolHandle>;
		using ResourceInfo = typename ObjectPoolType::NewElementInfo;
		using NamesContainer = std::unordered_map<FileNameID, typename ElementPoolHandle::KeyType>;

	public:
		using ResourcePoolHandle = ElementPoolHandle;

		struct ResourceAccessInfo final
		{
			ElementHandleReference m_element = nullptr;
			bool m_isNew = false;
		};

	private:
		ObjectPoolType m_pool;
		NamesContainer m_names;
		mutable SharedMutex m_lockMutex;

		ElementHandle* addResourceInternal(FileNameID nameID);
		ElementHandle* getResourceInternal(FileNameID nameID);
		ElementHandle* getResourceInternal(ElementPoolHandle handle);

	public:
		NamedObjectPool() = default;

		bool init(size_t pageSize = 128, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64);
		void release();

		void clear();

		bool isValid(ElementPoolHandle handle) const;

		ElementHandleReference getResource(ElementPoolHandle handle) const;
		ElementHandleReference getResource(ElementPoolHandle handle);

		ElementHandleReference getResource(const FileName& name) const;
		ElementHandleReference getResource(FileNameID nameID) const;
		ElementHandleReference getResource(const FileName& name);
		ElementHandleReference getResource(FileNameID nameID);

		ElementHandleReference addResource(const FileName& name);
		ElementHandleReference addResource(FileNameID nameID);

		ResourceAccessInfo getOrAddResource(const FileName& name);
		ResourceAccessInfo getOrAddResource(FileNameID nameID);

		void removeResource(FileNameID nameID);
	};
}

#include "NamedObjectPool.inl"