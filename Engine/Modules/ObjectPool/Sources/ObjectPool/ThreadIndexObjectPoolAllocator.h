#pragma once

#include "IndexObjectPoolAllocator.h"
#include "Multithreading/Mutex.h"
#include "Multithreading/LockGuard.h"

namespace VT
{
	template <typename IndexType>
	class ThreadIndexObjectPoolAllocator final
	{
	public:
		using ObjectPoolType = IndexObjectPoolAllocator<IndexType>;
		using NewElementInfo = typename ObjectPoolType::NewElementInfo;

	private:
		ObjectPoolType m_pool;
		mutable SharedMutex m_mutex;

	public:
		ThreadIndexObjectPoolAllocator() = default;

		bool init(size_t elementSize, size_t pageSize = 4096, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64)
		{
			UniqueLockGuard<SharedMutex> locker(m_mutex);
			return m_pool.init(elementSize, pageSize, maxFreePageCount, minFreeIndexCount);
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

		const void* getElement(IndexType index) const
		{
			SharedLockGuard<SharedMutex> locker(m_mutex);
			return m_pool.getElement(index);
		}

		void* getElement(IndexType index)
		{
			SharedLockGuard<SharedMutex> locker(m_mutex);
			return m_pool.getElement(index);
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

		void removeElement(IndexType index)
		{
			UniqueLockGuard<SharedMutex> locker(m_mutex);
			m_pool.removeElement(index);
		}
	};
}