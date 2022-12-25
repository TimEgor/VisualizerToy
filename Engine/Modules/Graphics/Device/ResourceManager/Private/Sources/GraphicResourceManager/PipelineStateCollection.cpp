#include "PipelineStateCollection.h"

#include "Multithreading/LockGuard.h"

#include <cassert>

#include "NamedReferencePool/NamedObjectPoolHandle.h"

VT::PipelineStateReference VT::PipelineStateCollection::addPipelineStateInternal(
	PipelineStateHash hash)
{
	auto findStateIDIter = m_stateIDs.find(hash);
	if (findStateIDIter != m_stateIDs.end())
	{
		return nullptr;
	}

	PipelineStateHandleInfo info = m_states.addElementRaw();

	ManagedPipelineStateResourceHandle* handle
		= new (info.m_elementPtr) ManagedPipelineStateResourceHandle(nullptr, info.m_elementHandle.getKey());

	m_stateIDs[hash] = HashStateData{ handle, info.m_elementHandle.getKey() };

	return handle;
}

VT::PipelineStateReference VT::PipelineStateCollection::getPipelineStateInternal(
	PipelineStateHash hash)
{
	auto findStateIDIter = m_stateIDs.find(hash);
	if (findStateIDIter == m_stateIDs.end())
	{
		return nullptr;
	}

	return findStateIDIter->second.m_state;
}

VT::PipelineStateConstReference VT::PipelineStateCollection::getPipelineStateInternal(
	PipelineStateHash hash) const
{
	auto findStateIDIter = m_stateIDs.find(hash);
	if (findStateIDIter == m_stateIDs.end())
	{
		return nullptr;
	}

	return findStateIDIter->second.m_state;
}

bool VT::PipelineStateCollection::init(size_t pageSize, size_t maxFreePageCount, size_t minFreeIndexCount)
{
	UniqueLockGuard locker(m_lockMutex);

	VT_CHECK_INITIALIZATION(m_states.init(pageSize, maxFreePageCount, minFreeIndexCount));
	return true;
}

void VT::PipelineStateCollection::release()
{
	UniqueLockGuard locker(m_lockMutex);

	m_states.release();
	m_stateIDs = HashCotainer();
}

void VT::PipelineStateCollection::clear()
{
	UniqueLockGuard locker(m_lockMutex);

	m_states.clear();
	m_stateIDs.clear();
}

bool VT::PipelineStateCollection::isValid(PipelineStatePoolHandle handle) const
{
	SharedLockGuard locker(m_lockMutex);

	return m_states.isValid(handle);
}

VT::PipelineStateConstReference VT::PipelineStateCollection::getPipelineState(PipelineStateHash hash) const
{
	SharedLockGuard locker(m_lockMutex);
	return getPipelineStateInternal(hash);
}

VT::PipelineStateReference VT::PipelineStateCollection::getPipelineState(PipelineStateHash hash)
{
	SharedLockGuard locker(m_lockMutex);
	return getPipelineStateInternal(hash);
}

VT::PipelineStateReference VT::PipelineStateCollection::addPipelineState(PipelineStateHash hash)
{
	UniqueLockGuard locker(m_lockMutex);
	return addPipelineStateInternal(hash);
}

VT::PipelineStateCollection::PipelineStateAccessInfo VT::PipelineStateCollection::getOrAddPipelineState(
	PipelineStateHash hash)
{
	UniqueLockGuard locker(m_lockMutex);

	PipelineStateReference handle = getPipelineStateInternal(hash);
	if (handle)
	{
		return PipelineStateAccessInfo{ handle, false };
	}

	return PipelineStateAccessInfo{ addPipelineStateInternal(hash), true };
}

void VT::PipelineStateCollection::removePipelineState(PipelineStateHash hash)
{
	UniqueLockGuard locker(m_lockMutex);

	auto findStateIDIter = m_stateIDs.find(hash);
	if (findStateIDIter == m_stateIDs.end())
	{
		assert(false);
		return;
	}

	findStateIDIter->second.m_state.release();

	m_states.removeElement(findStateIDIter->second.m_handleID);
	m_stateIDs.erase(findStateIDIter);
}
