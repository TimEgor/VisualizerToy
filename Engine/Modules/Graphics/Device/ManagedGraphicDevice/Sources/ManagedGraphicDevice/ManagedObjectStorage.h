#pragma once

#include "ManagedObjectStorageInfo.h"
#include "ObjectPool/ObjectPool.h"

namespace VT::ManagedGraphicDevice
{
	template <typename BaseManagedStorageInfoType>
	class ManagedObjectStorageBase
	{
	protected:
		using HandleType = typename BaseManagedStorageInfoType::HandleType;
		using NewObjectInfo = typename BaseManagedStorageInfoType::NewObjectInfo;

		static_assert(std::is_base_of<ManagedStorageInfoBase, BaseManagedStorageInfoType>::value);

	public:
		ManagedObjectStorageBase() = default;
		virtual ~ManagedObjectStorageBase() {}

		virtual bool init(size_t pageSize, size_t maxFreePageCount, size_t minFreeIndexCount) = 0;
		virtual void release() = 0;

		virtual NewObjectInfo addNewObject() = 0;
		virtual void removeObject(HandleType handle) = 0;

		virtual bool isObjectValid(HandleType handle) const = 0;
	};

	template <typename ManagedStorageInfoType>
	class ManagedObjectStorage final : public ManagedObjectStorageBase<typename ManagedStorageInfoType::BaseStorageInfo>
	{
		using ObjectType = typename ManagedStorageInfoType::ObjectType;
		using HandleType = typename ManagedStorageInfoType::HandleType;
		using NewObjectInfo = typename ManagedStorageInfoType::NewObjectInfo;

		using ObjectPoolType = ObjectPool<ObjectType, HandleType>;
		using ObjectPoolElementInfo = typename ObjectPoolType::NewElementInfo;

	private:
		ObjectPoolType m_pool;

	public:
		ManagedObjectStorage() = default;
		~ManagedObjectStorage() { release(); }

		virtual bool init(size_t pageSize, size_t maxFreePageCount, size_t minFreeIndexCount) override
		{
			return m_pool.init(pageSize, maxFreePageCount, minFreeIndexCount);
		}

		virtual void release() override
		{
			m_pool.release();
		}

		virtual NewObjectInfo addNewObject() override
		{
			ObjectPoolElementInfo info = m_pool.addElementRaw();
			return NewObjectInfo{ info.m_elementPtr, info.m_elementHandle };
		}

		virtual void removeObject(HandleType handle) override
		{
			m_pool.removeElement(handle);
		}

		virtual bool isObjectValid(HandleType handle) const override
		{
			return m_pool.isValid(handle);
		}
	};
}