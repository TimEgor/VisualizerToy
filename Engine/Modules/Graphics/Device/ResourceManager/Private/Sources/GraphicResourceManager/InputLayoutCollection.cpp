#include "InputLayoutCollection.h"

#include "Multithreading/LockGuard.h"

#include <cassert>

void VT::InputLayoutCollection::addInputLayoutInternal(const InputLayoutDesc& desc)
{
	InputLayoutHash hash = desc.getHash();

	auto findStateIDIter = m_layoutIDs.find(hash);
	if (findStateIDIter != m_layoutIDs.end())
	{
		return;
	}

	InputLayoutHandleInfo layoutInfo = m_layouts.addElementRaw();

	ManagedInputLayoutHandle* handle
		= new (layoutInfo.m_elementPtr) ManagedInputLayoutHandle(desc, layoutInfo.m_elementHandle.getKey());

	m_layoutIDs[hash] = handle;
}

VT::InputLayoutReference VT::InputLayoutCollection::getInputLayoutInternal(InputLayoutHash hash)
{
	auto findStateIDIter = m_layoutIDs.find(hash);
	if (findStateIDIter == m_layoutIDs.end())
	{
		return nullptr;
	}

	return findStateIDIter->second;
}

VT::InputLayoutConstReference VT::InputLayoutCollection::getInputLayoutInternal(InputLayoutHash hash) const
{
	auto findStateIDIter = m_layoutIDs.find(hash);
	if (findStateIDIter == m_layoutIDs.end())
	{
		return nullptr;
	}

	return findStateIDIter->second;
}

void VT::InputLayoutCollection::removeInputLayoutHandle(InputLayoutHandleID handleID)
{
	m_layouts.removeElement(handleID);
}

bool VT::InputLayoutCollection::init(size_t pageSize, size_t maxFreePageCount, size_t minFreeIndexCount)
{
	UniqueLockGuard locker(m_lockMutex);

	VT_CHECK_INITIALIZATION(m_layouts.init(pageSize, maxFreePageCount, minFreeIndexCount));
	return true;
}

void VT::InputLayoutCollection::release()
{
	UniqueLockGuard locker(m_lockMutex);

	m_layoutIDs = HashCotainer();
	m_layouts.release();
}

void VT::InputLayoutCollection::clear()
{
	UniqueLockGuard locker(m_lockMutex);

	m_layouts.clear();
	m_layoutIDs.clear();
}

VT::InputLayoutConstReference VT::InputLayoutCollection::getInputLayout(InputLayoutHash hash) const
{
	SharedLockGuard locker(m_lockMutex);
	return getInputLayoutInternal(hash);
}

VT::InputLayoutReference VT::InputLayoutCollection::getInputLayout(InputLayoutHash hash)
{
	SharedLockGuard locker(m_lockMutex);
	return getInputLayoutInternal(hash);
}

void VT::InputLayoutCollection::addInputLayout(const InputLayoutDesc& desc)
{
	UniqueLockGuard locker(m_lockMutex);
	return addInputLayoutInternal(desc);
}

void VT::InputLayoutCollection::removeInputLayout(InputLayoutHash hash)
{
	UniqueLockGuard locker(m_lockMutex);

	auto findStateIDIter = m_layoutIDs.find(hash);
	if (findStateIDIter == m_layoutIDs.end())
	{
		assert(false);
		return;
	}

	m_layoutIDs.erase(findStateIDIter);
}
