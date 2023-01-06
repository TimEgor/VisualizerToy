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
		using ElementInfo = typename ObjectPoolType::NewElementInfo;
		using NamesContainer = std::unordered_map<FileNameID, typename ElementPoolHandle::KeyType>;

	public:
		using ElementPoolHandleType = ElementPoolHandle;

		struct ElementAccessInfo final
		{
			ElementHandleReference m_element = nullptr;
			bool m_isNew = false;
		};

	private:
		ObjectPoolType m_pool;
		NamesContainer m_names;
		mutable SharedMutex m_lockMutex;

		ElementHandle* addElementInternal(FileNameID nameID);
		ElementHandle* getElementInternal(FileNameID nameID);
		ElementHandle* getElementInternal(ElementPoolHandleType handle);

	public:
		NamedObjectPool() = default;

		bool init(size_t pageSize = 128, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64);
		void release();

		void clear();

		bool isValid(ElementPoolHandleType handle) const;

		ElementHandleReference getElement(ElementPoolHandleType handle) const;
		ElementHandleReference getElement(ElementPoolHandleType handle);

		ElementHandleReference getElement(const FileName& name) const;
		ElementHandleReference getElement(FileNameID nameID) const;
		ElementHandleReference getElement(const FileName& name);
		ElementHandleReference getElement(FileNameID nameID);

		ElementHandleReference addElement(const FileName& name);
		ElementHandleReference addElement(FileNameID nameID);

		ElementAccessInfo getOrAddElement(const FileName& name);
		ElementAccessInfo getOrAddElement(FileNameID nameID);

		void removeElement(FileNameID nameID);
	};
}

#include "NamedObjectPool.inl"