#pragma once

#include "Multithreading/Atomic.h"
#include "Multithreading/SpinLock.h"
#include "Multithreading/LockGuard.h"

namespace VT
{
	class CountableObjectBase
	{
	private:
		Atomic<size_t> m_counter;
		SpinLock m_lockState;
		bool m_isAlive;

		void tryDestroy()
		{
			LockGuard<SpinLock> locker(m_lockState);

			if (loadAtomic(&m_counter, MemoryOrder::Relaxed) == 0 && m_isAlive)
			{
				m_isAlive = true;
				selfDestroy();
			}
		}

	protected:
		virtual void selfDestroy() = 0;

		void incrementCounter()
		{
			fetchAddAtomic(&m_counter, 1, MemoryOrder::Relaxed);
		}
		void decrementCounter()
		{
			fetchSubAtomic(&m_counter, 1, MemoryOrder::Relaxed);
		}

	public:
		CountableObjectBase()
			: m_counter(0), m_isAlive(true)
		{}
		virtual ~CountableObjectBase() {}

		CountableObjectBase* getReferenceObject()
		{
			LockGuard<SpinLock> locker(m_lockState);

			if (loadAtomic(&m_counter, MemoryOrder::Relaxed) > 0 || m_isAlive)
			{
				incrementCounter();
				return this;
			}

			return nullptr;
		}
		void releaseReferenceObject()
		{
			decrementCounter();
		}
	};
}