#include "EventSystem.h"

bool VT::EventSystem::init()
{
	return m_dispatchers.init();
}

void VT::EventSystem::release()
{
	m_dispatchers.release();
	m_dispatcherOrders = DispatcherOrdersCollection();
}

bool VT::EventSystem::registerEvent(EventType type)
{
	auto findEventIter = m_dispatcherOrders.find(type);
	if (findEventIter != m_dispatcherOrders.end())
	{
		return false;
	}

	m_dispatcherOrders.insert(std::make_pair(type, DispatcherOrderCollection()));

	return true;
}

bool VT::EventSystem::unregisterEvent(EventType type)
{
	auto findEventIter = m_dispatcherOrders.find(type);
	if (findEventIter == m_dispatcherOrders.end())
	{
		return false;
	}

	m_dispatcherOrders.erase(findEventIter);

	return true;
}

VT::EventDispatcherID VT::EventSystem::addEventDispatcher(EventType type, const EventDispatcher& dispatcher)
{
	auto findEventIter = m_dispatcherOrders.find(type);
	if (findEventIter == m_dispatcherOrders.end())
	{
		return InvalidEventDispatcherID;
	}

	const DispatcherPoolElementInfo dispatcherInfo = m_dispatchers.addElement(DispatcherData{ dispatcher, type, static_cast<uint32_t>(-1) });
	const EventDispatcherID dispatcherID = dispatcherInfo.m_elementHandle.getKey();

	findEventIter->second.push_back(dispatcherInfo.m_elementPtr);
	dispatcherInfo.m_elementPtr->m_orderIndex = findEventIter->second.size() - 1;

	return dispatcherID;
}

void VT::EventSystem::removeEventDispatcher(EventDispatcherID dispatcherID)
{
	const DispatcherData* dispatcherData = m_dispatchers.getElement(dispatcherID);
	if (!dispatcherData)
	{
		return;
	}

	auto findEventIter = m_dispatcherOrders.find(dispatcherData->m_eventType);
	if (findEventIter == m_dispatcherOrders.end())
	{
		return;
	}

	DispatcherOrderCollection& order = findEventIter->second;
	if (dispatcherData->m_orderIndex != order.size())
	{
		DispatcherData* swapDispatcherData = order.back();
		assert(swapDispatcherData);

		order[dispatcherData->m_orderIndex] = swapDispatcherData;
		swapDispatcherData->m_orderIndex = dispatcherData->m_orderIndex;
	}

	m_dispatchers.removeElement(dispatcherID);
	order.pop_back();
}

bool VT::EventSystem::dispatchEvent(EventType type, const IEvent& event) const
{
	auto findEventIter = m_dispatcherOrders.find(type);
	if (findEventIter == m_dispatcherOrders.end())
	{
		return false;
	}

	for (const auto& dispatcherData : findEventIter->second)
	{
		dispatcherData->m_dispatcher(event);
	}

	return true;
}