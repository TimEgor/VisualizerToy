#pragma once

#include <atomic>

namespace VT
{
	enum class MemoryOrder
	{
		Relaxed,

		Acquire,
		Release,
		Acq_Rel,

		Seq_Cst
	};

	constexpr std::memory_order convertMemOrder_VTtoSTD(MemoryOrder order)
	{
		switch (order)
		{
		case VT::MemoryOrder::Relaxed:
			return std::memory_order_relaxed;
		case VT::MemoryOrder::Acquire:
			return std::memory_order_acquire;
		case VT::MemoryOrder::Release:
			return std::memory_order_release;
		case VT::MemoryOrder::Acq_Rel:
			return std::memory_order_acq_rel;
		case VT::MemoryOrder::Seq_Cst:
		default:
			return std::memory_order_seq_cst;
		}
	}

	using AtomicFlag = std::atomic_flag;
#define InitialAtomicFlag ATOMIC_FLAG_INIT;

	template <typename T>
	using Atomic = std::atomic<T>;

	template <typename T>
	using AtomicTypeVal = typename Atomic<T>::value_type;

	inline void clearAtomicFlag(AtomicFlag* flag) { std::atomic_flag_clear(flag); }
	inline void clearAtomicFlag(AtomicFlag* flag, MemoryOrder order) { std::atomic_flag_clear_explicit(flag, convertMemOrder_VTtoSTD(order)); }
	inline bool setAndTestAtomicFlag(AtomicFlag* flag) { return std::atomic_flag_test_and_set(flag); }
	inline bool setAndTestAtomicFlag(AtomicFlag* flag, MemoryOrder order) { return std::atomic_flag_test_and_set_explicit(flag, convertMemOrder_VTtoSTD(order)); }

	template <typename T>
	inline T loadAtomic(Atomic<T>* atomic) { return std::atomic_load(atomic); }
	template <typename T>
	inline T loadAtomic(Atomic<T>* atomic, MemoryOrder order) { return std::atomic_load_explicit(atomic, convertMemOrder_VTtoSTD(order)); }
	template <typename T>
	inline void storeAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val) { std::atomic_store(atomic, val); }
	template <typename T>
	inline void storeAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val, MemoryOrder order) { std::atomic_store_explicit(atomic, val, convertMemOrder_VTtoSTD(order)); }

	template <typename T>
	inline bool compareExchangeStrongAtomic(Atomic<T>* atomic, AtomicTypeVal<T>* expectedVal, AtomicTypeVal<T> desiredVal)
	{
		return std::atomic_compare_exchange_strong(atomic, expectedVal, desiredVal);
	}
	template <typename T>
	inline bool compareExchangeStrongAtomic(Atomic<T>* atomic, AtomicTypeVal<T>* expectedVal, AtomicTypeVal<T> desiredVal, MemoryOrder order)
	{
		return std::atomic_compare_exchange_strong_explicit(atomic, expectedVal, desiredVal, convertMemOrder_VTtoSTD(order));
	}
	template <typename T>
	inline bool compareExchangeWeakAtomic(Atomic<T>* atomic, AtomicTypeVal<T>* expectedVal, AtomicTypeVal<T> desiredVal)
	{
		return std::atomic_compare_exchange_weak(atomic, expectedVal, desiredVal);
	}
	template <typename T>
	inline bool compareExchangeWeakAtomic(Atomic<T>* atomic, AtomicTypeVal<T>* expectedVal, AtomicTypeVal<T> desiredVal, MemoryOrder order)
	{
		return std::atomic_compare_exchange_weak_explicit(atomic, expectedVal, desiredVal, convertMemOrder_VTtoSTD(order));
	}
	template <typename T>
	inline bool exchangeAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val) { return std::atomic_exchange(atomic, val); }
	template <typename T>
	inline bool exchangeAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val, MemoryOrder order) { return std::atomic_exchange_explicit(atomic, val, convertMemOrder_VTtoSTD(order)); }

	template <typename T>
	inline T fetchAddAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val) { return std::atomic_fetch_add(atomic, val); }
	template <typename T>
	inline T fetchAddAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val, MemoryOrder order) { return std::atomic_fetch_add_explicit(atomic, val, convertMemOrder_VTtoSTD(order)); }
	template <typename T>
	inline T fetchSubAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val) { return std::atomic_fetch_sub(atomic, val); }
	template <typename T>
	inline T fetchSubAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val, MemoryOrder order) { return std::atomic_fetch_sub_explicit(atomic, val, convertMemOrder_VTtoSTD(order)); }
	template <typename T>
	inline T fetchAndAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val) { return std::atomic_fetch_and(atomic, val); }
	template <typename T>
	inline T fetchAndAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val, MemoryOrder order) { return std::atomic_fetch_and_explicit(atomic, val, convertMemOrder_VTtoSTD(order)); }
	template <typename T>
	inline T fetchOrAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val) { return std::atomic_fetch_or(atomic, val); }
	template <typename T>
	inline T fetchOrAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val, MemoryOrder order) { return std::atomic_fetch_or_explicit(atomic, val, convertMemOrder_VTtoSTD(order)); }
	template <typename T>
	inline T fetchXorAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val) { return std::atomic_fetch_xor(atomic, val); }
	template <typename T>
	inline T fetchXorAtomic(Atomic<T>* atomic, AtomicTypeVal<T> val, MemoryOrder order) { return std::atomic_fetch_xor(atomic, val, convertMemOrder_VTtoSTD(order)); }
}