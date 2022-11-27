#pragma once

#include "IndexObjectPool.h"
#include "Multithreading/Mutex.h"
#include "Multithreading/LockGuard.h"

namespace VT
{
	template <typename ObjectType, typename IndexType>
	class ThreadSafeIndexObjectPool
	{
	public:
		using ObjectPoolType = IndexObjectPool<ObjectType, IndexType>;
		using NewElementInfo = typename ObjectPoolType::NewElementInfo;

	private:
		ObjectPoolType m_pool;
		mutable SharedMutex m_mutex;

	public:
		ThreadSafeIndexObjectPool() = default;

		bool init(size_t pageSize = 4096, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64)
		{
			UniqueLockGuard<SharedMutex> locker(m_mutex);
			return m_pool.init(pageSize, maxFreePageCount, minFreeIndexCount);
		}

		void release()
		{
			UniqueLockGuard<SharedMutex> locker(m_mutex);
			m_pool.release();
		}

		void clear()
		{
			UniqueLockGuard<SharedMutex> locker(m_mutex);
			m_pool.clear();
		}

		bool isValid(IndexType index) const
		{
			SharedLockGuard<SharedMutex> locker(m_mutex);
			return m_pool.isValid(index);
		}

		const ObjectType* getElement(IndexType index) const
		{
			SharedLockGuard<SharedMutex> locker(m_mutex);
			return m_pool.getElement(index);
		}

		ObjectType* getElement(IndexType handle)
		{
			SharedLockGuard<SharedMutex> locker(m_mutex);
			return m_pool.getElement(handle);
		}

		void addElementRaw(NewElementInfo& info)
		{
			UniqueLockGuard<SharedMutex> locker(m_mutex);
			m_pool.addElementRaw(info);
		}

		NewElementInfo addElementRaw()
		{
			NewElementInfo info;
			addElementRaw(info);

			return info;
		}

		template <typename... Args>
		void addElement(NewElementInfo& info, Args&&... args)
		{
			addElementRaw(info);
			new (info.m_elementPtr) ObjectType(args...);
		}

		template <typename... Args>
		NewElementInfo addElement(Args&&... args)
		{
			NewElementInfo info = addElementRaw();
			new (info.m_elementPtr) ObjectType(args...);

			return info;
		}

		void removeElement(IndexType handle)
		{
			UniqueLockGuard<SharedMutex> locker(m_mutex);
			m_pool.removeElement(handle);
		}
	};
}