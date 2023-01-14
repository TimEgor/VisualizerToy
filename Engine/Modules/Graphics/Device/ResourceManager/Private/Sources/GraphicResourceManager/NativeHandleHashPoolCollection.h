#pragma once

#include "GraphicPipeline/IPipelineState.h"
#include "ObjectPool/ObjectPool.h"
#include "Multithreading/Mutex.h"
#include "Multithreading/LockGuard.h"

#include <unordered_map>

namespace VT
{
	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType,
	typename HashType, typename PoolHandleType>
	class NativeHandleHashPoolCollection final
	{
	public:
		struct AccessInfo final
		{
			HandleReferenceType m_handle = nullptr;
			bool m_isNew = false;
		};

	private:
		using PoolType = ObjectPool<HandleType, PoolHandleType>;
		using HandleInfo = typename PoolType::NewElementInfo;
		using HashCotainer = std::unordered_map<HashType, HandleReferenceType>;

		PoolType m_handles;
		HashCotainer m_handleIDs;
		mutable SharedMutex m_lockMutex;

		HandleReferenceType addHandleInternal(HashType hash);
		HandleReferenceType getHandleInternal(HashType hash);
		HandleConstReferenceType getHandleInternal(HashType hash) const;

	public:
		NativeHandleHashPoolCollection() = default;
		~NativeHandleHashPoolCollection() { release(); }

		bool init(size_t pageSize = 128, size_t maxFreePageCount = 2, size_t minFreeIndexCount = 64);
		void release();

		void clear();

		HandleConstReferenceType getHandle(HashType hash) const;
		HandleReferenceType getHandle(HashType hash);

		HandleReferenceType addHandle(HashType hash);

		AccessInfo getOrAddHandle(HashType hash);

		void removeHandle(HashType hash);
	};

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	HandleReferenceType NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType,
	HashType, PoolHandleType>::addHandleInternal(HashType hash)
	{
		auto findStateIDIter = m_handleIDs.find(hash);
		if (findStateIDIter != m_handleIDs.end())
		{
			return nullptr;
		}

		HandleInfo info = m_handles.addElementRaw();

		HandleType* handle
			= new (info.m_elementPtr) HandleType(nullptr, info.m_elementHandle.getKey());

		m_handleIDs[hash] = handle;

		return handle;
	}

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	HandleReferenceType NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType,
	HashType, PoolHandleType>::getHandleInternal(HashType hash)
	{
		auto findStateIDIter = m_handleIDs.find(hash);
		if (findStateIDIter == m_handleIDs.end())
		{
			return nullptr;
		}

		return findStateIDIter->second;
	}

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	HandleConstReferenceType NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType,
	HashType, PoolHandleType>::getHandleInternal(HashType hash) const
	{
		auto findStateIDIter = m_handleIDs.find(hash);
		if (findStateIDIter == m_handleIDs.end())
		{
			return nullptr;
		}

		return findStateIDIter->second;
	}

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	bool NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType, HashType,
	PoolHandleType>::init(size_t pageSize, size_t maxFreePageCount, size_t minFreeIndexCount)
	{
		UniqueLockGuard locker(m_lockMutex);

		VT_CHECK_INITIALIZATION(m_handles.init(pageSize, maxFreePageCount, minFreeIndexCount));
		return true;
	}

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	void NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType, HashType,
	PoolHandleType>::release()
	{
		UniqueLockGuard locker(m_lockMutex);

		m_handleIDs = HashCotainer();
		m_handles.release();
	}

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	void NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType, HashType,
	PoolHandleType>::clear()
	{
		UniqueLockGuard locker(m_lockMutex);

		m_handles.clear();
		m_handleIDs.clear();
	}

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	HandleConstReferenceType NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType,
	HashType, PoolHandleType>::getHandle(HashType hash) const
	{
		SharedLockGuard locker(m_lockMutex);
		return getHandleInternal(hash);
	}

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	HandleReferenceType NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType,
	HashType, PoolHandleType>::getHandle(HashType hash)
	{
		SharedLockGuard locker(m_lockMutex);
		return getHandleInternal(hash);
	}

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	HandleReferenceType NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType,
	HashType, PoolHandleType>::addHandle(HashType hash)
	{
		UniqueLockGuard locker(m_lockMutex);
		return addHandleInternal(hash);
	}

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	typename NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType, HashType,
	PoolHandleType>::AccessInfo NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType
	, HashType, PoolHandleType>::getOrAddHandle(HashType hash)
	{
		UniqueLockGuard locker(m_lockMutex);

		HandleReferenceType handle = getHandleInternal(hash);
		if (handle)
		{
			return AccessInfo{ handle, false };
		}

		return AccessInfo{ addHandleInternal(hash), true };
	}

	template <typename HandleType, typename HandleReferenceType, typename HandleConstReferenceType, typename HashType,
		typename PoolHandleType>
	void NativeHandleHashPoolCollection<HandleType, HandleReferenceType, HandleConstReferenceType, HashType,
	PoolHandleType>::removeHandle(HashType hash)
	{
		UniqueLockGuard locker(m_lockMutex);

		auto findHandleIDIter = m_handleIDs.find(hash);
		if (findHandleIDIter == m_handleIDs.end())
		{
			assert(false);
			return;
		}

		m_handleIDs.erase(findHandleIDIter);
	}
}
