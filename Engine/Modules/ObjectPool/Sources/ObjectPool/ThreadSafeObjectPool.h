#pragma once

#include "ObjectPool.h"
#include "Multithreading/Mutex.h"
#include "Multithreading/LockGuard.h"

namespace VT
{
	template <typename ObjectType, typename HandleType>
	class ThreadSafeObjectPool
	{
	public:
		using ObjectPoolType = ObjectPool<ObjectType, HandleType>;
		using NewElementInfo = typename ObjectPoolType::NewElementInfo;
		using HandleElementType = HandleType;

	private:
		ObjectPoolType m_pool;
		mutable SharedMutex m_mutex;

	public:
		ThreadSafeObjectPool() = default;

		bool init(size_t pageSize = 4096, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64)
		{
			UniqueLockGuard locker(m_mutex);
			return m_pool.init(pageSize, maxFreePageCount, minFreeIndexCount);
		}

		void release()
		{
			UniqueLockGuard locker(m_mutex);
			m_pool.release();
		}

		void clear()
		{
			UniqueLockGuard locker(m_mutex);
			m_pool.clear();
		}

		bool isValid(HandleType handle) const
		{
			SharedLockGuard locker(m_mutex);
			return m_pool.isValid(handle);
		}

		const ObjectType* getElement(HandleType handle) const
		{
			SharedLockGuard locker(m_mutex);
			return m_pool.getElement(handle);
		}

		ObjectType* getElement(HandleType handle)
		{
			SharedLockGuard locker(m_mutex);
			return m_pool.getElement(handle);
		}

		template <typename T>
		T getElementCast(HandleType handle) const
		{
			SharedLockGuard locker(m_mutex);
			return m_pool.getElement(handle);
		}

		template <typename T>
		T getElementCast(HandleType handle)
		{
			SharedLockGuard locker(m_mutex);
			return m_pool.getElement(handle);
		}

		void addElementRaw(NewElementInfo& info)
		{
			UniqueLockGuard locker(m_mutex);
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
			new (info.m_elementPtr) ValType(args...);
		}

		template <typename... Args>
		NewElementInfo addElement(Args&&... args)
		{
			NewElementInfo info = addElementRaw();
			new (info.m_elementPtr) ValType(args...);

			return info;
		}

		void removeElement(HandleType handle)
		{
			UniqueLockGuard locker(m_mutex);
			m_pool.removeElement(handle);
		}
	};
}