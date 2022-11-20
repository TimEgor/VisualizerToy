#pragma once

#include <mutex>
#include <shared_mutex>

namespace VT
{
	template <typename MutexType>
	class LockGuard final
	{
	private:
		std::lock_guard<MutexType> m_locker;

	public:
		LockGuard(MutexType& mutex)
			: m_locker(mutex) {}
		LockGuard(const LockGuard& locker) = delete;
		LockGuard(LockGuard&& locker)
			: m_locker(std::move(locker)) {}
	};

	struct DeferLock final
	{
		explicit DeferLock() = default;
	};

	template <typename MutexType>
	class UniqueLockGuard final
	{
	private:
		std::unique_lock<MutexType> m_locker;

	public:
		UniqueLockGuard(MutexType& mutex)
			: m_locker(mutex) {}
		UniqueLockGuard(MutexType& mutex, DeferLock defLock)
			: m_locker(mutex, std::defer_lock) {}
		UniqueLockGuard(const UniqueLockGuard& locker) = delete;
		UniqueLockGuard(UniqueLockGuard&& locker)
			: m_locker(std::move(locker)) {}

		void lock() { m_locker.lock(); }
		void tryLock() { m_locker.try_lock(); }
		void unlock() { m_locker.unlock(); }
	};

	template <typename MutexType>
	class SharedLockGuard final
	{
	private:
		std::shared_lock<MutexType> m_locker;

	public:
		SharedLockGuard(MutexType& mutex)
			: m_locker(mutex) {}
		SharedLockGuard(MutexType& mutex, DeferLock defLock)
			: m_locker(mutex, std::defer_lock) {}
		SharedLockGuard(const SharedLockGuard& locker) = delete;
		SharedLockGuard(SharedLockGuard&& locker)
			: m_locker(std::move(locker)) {}

		void lock() { m_locker.lock(); }
		void tryLock() { m_locker.try_lock(); }
		void unlock() { m_locker.unlock(); }
	};
}