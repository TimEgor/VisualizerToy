#pragma once

#include <mutex>
#include <shared_mutex>

namespace VT
{
	class Mutex final
	{
	private:
		std::mutex m_mutex;

	public:
		Mutex() = default;

		Mutex(const Mutex&) = delete;
		Mutex& operator=(const Mutex&) = delete;

		void lock() { m_mutex.lock(); }
		bool tryLock() { return m_mutex.try_lock(); }
		void unlock() { m_mutex.unlock(); }

		bool try_lock() { return tryLock(); }
	};

	class SharedMutex final
	{
	private:
		std::shared_mutex m_mutex;

	public:
		SharedMutex() = default;

		SharedMutex(const SharedMutex&) = delete;
		SharedMutex& operator=(const SharedMutex&) = delete;

		void lock() { m_mutex.lock(); }
		bool tryLock() { return m_mutex.try_lock(); }
		void unlock() { m_mutex.unlock(); }

		void sharedLock() { m_mutex.lock_shared(); }
		bool trySharedLock() { return m_mutex.try_lock_shared(); }
		void sharedUnlock() { m_mutex.unlock_shared(); }

		bool try_lock() { return tryLock(); }

		void lock_shared() { sharedLock(); }
		bool try_lock_shared() { return trySharedLock(); }
		void unlock_shared() { sharedUnlock(); }
	};
}