#pragma once

#include "Atomic.h"

namespace VT
{
	class SpinLock final
	{
	private:
		AtomicFlag m_lockState = InitialAtomicFlag;

	public:
		SpinLock() = default;

		inline void lock()
		{
			while (setAndTestAtomicFlag(&m_lockState, MemoryOrder::Acq_Rel)) {}
		}

		inline bool tryLock()
		{
			return !setAndTestAtomicFlag(&m_lockState, MemoryOrder::Acq_Rel);
		}

		inline void unlock()
		{
			clearAtomicFlag(&m_lockState, MemoryOrder::Release);
		}

		inline bool try_lock() { return try_lock(); }
	};
}