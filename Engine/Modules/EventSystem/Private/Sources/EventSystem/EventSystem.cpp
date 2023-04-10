#include "EventSystem.h"

bool VT::EventSystem::init()
{
	VT_CHECK_INITIALIZATION(m_dispatchers.init());

	VT_CHECK_INITIALIZATION(m_instancedEvents.init());
	VT_CHECK_INITIALIZATION(m_instancedDispatchers.init());
}

void VT::EventSystem::release()
{
	m_dispatchers.release();
	m_dispatcherOrders = DispatcherOrdersCollection();

	m_instancedEvents.release();
	m_instancedDispatchers.release();
	m_instancedDispatcherOrders = InstancedDispatcherOrdersCollection();
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

VT::InstancedEventID VT::EventSystem::registerInstancedEvent(InstancedEventType eventType)
{
	InstancedEventID eventID = m_instancedEvents.addElement(eventType).m_elementHandle.getKey();

	m_instancedDispatcherOrders.insert(std::make_pair(eventID, InvalidInstancedEventDispatcherID));

	return eventID;
}

bool VT::EventSystem::unregisterInstancedEvent(InstancedEventID eventID)
{
	auto findEventIter = m_instancedDispatcherOrders.find(eventID);
	if (findEventIter == m_instancedDispatcherOrders.end())
	{
		return false;
	}

	InstancedEventDispatcherID dispatcherID = findEventIter->second;
	while (dispatcherID != InvalidEventDispatcherID)
	{
		InstancedEventDispatcherID nextDispatcherID = m_instancedDispatchers.getElement(dispatcherID)->m_nextSiblingID;
		m_instancedDispatchers.removeElement(dispatcherID);

		dispatcherID = nextDispatcherID;
	}

	m_instancedEvents.removeElement(eventID);

	return true;
}

VT::InstancedEventDispatcherID VT::EventSystem::addInstanceEventDispatcher(InstancedEventID eventID,
	const InstancedEventDispatcher& dispatcher)
{
	auto findEventIter = m_instancedDispatcherOrders.find(eventID);
	if (findEventIter == m_instancedDispatcherOrders.end())
	{
		return InvalidEventDispatcherID;
	}

	InstancedEventDispatcherID prevOrderDispatcherId = findEventIter->second;
	InstancedEventDispatcherID dispatcherID = m_instancedDispatchers.addElement(
		InstancedDispatcherData{ dispatcher, eventID, prevOrderDispatcherId, InvalidInstancedEventDispatcherID }
	).m_elementHandle.getKey();

	if (prevOrderDispatcherId != InvalidInstancedEventDispatcherID)
	{
		m_instancedDispatchers.getElement(prevOrderDispatcherId)->m_nextSiblingID = dispatcherID;
	}

	findEventIter->second = dispatcherID;

	return dispatcherID;
}

void VT::EventSystem::removeInstancedEventDispatcher(InstancedEventDispatcherID dispatcherID)
{
	const InstancedDispatcherData* dispatcherData = m_instancedDispatchers.getElement(dispatcherID);
	if (!dispatcherData)
	{
		return;
	}

	if (dispatcherData->m_nextSiblingID != InvalidEventDispatcherID)
	{
		InstancedDispatcherData* nextSiblinngDispatcherData = m_instancedDispatchers.getElement(dispatcherData->m_nextSiblingID);

		assert(nextSiblinngDispatcherData);
		assert(nextSiblinngDispatcherData->m_prevSiblingID == dispatcherID);

		nextSiblinngDispatcherData->m_prevSiblingID = dispatcherData->m_prevSiblingID;
	}
	else
	{
		InstancedEventDispatcherID& firstOrderDispatcherID = m_instancedDispatcherOrders.at(dispatcherData->m_eventID);
		assert(firstOrderDispatcherID == dispatcherID);

		firstOrderDispatcherID = dispatcherData->m_prevSiblingID;
	}

	if (dispatcherData->m_prevSiblingID != InvalidEventDispatcherID)
	{
		InstancedDispatcherData* prevSiblinngDispatcherData = m_instancedDispatchers.getElement(dispatcherData->m_prevSiblingID);

		assert(prevSiblinngDispatcherData);
		assert(prevSiblinngDispatcherData->m_nextSiblingID == dispatcherID);

		prevSiblinngDispatcherData->m_nextSiblingID = dispatcherData->m_nextSiblingID;
	}

	m_instancedDispatchers.removeElement(dispatcherID);
}

bool VT::EventSystem::dispatchInstancedEvent(InstancedEventID eventID, const IInstancedEvent& event) const
{
	auto findEventIter = m_instancedDispatcherOrders.find(eventID);
	if (findEventIter == m_instancedDispatcherOrders.end())
	{
		return false;
	}

	InstancedEventDispatcherID dispatcherID = findEventIter->second;
	while (dispatcherID != InvalidEventDispatcherID)
	{
		const InstancedDispatcherData* dispatcherData = m_instancedDispatchers.getElement(dispatcherID);
		assert(dispatcherData);

		dispatcherData->m_dispatcher(event);

		dispatcherID = dispatcherData->m_nextSiblingID;
	}

	return true;
}
