#include "WindowBase.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

bool VT::WindowBase::initEvents()
{
	IEventSystem* eventSystem = EngineInstance::getInstance()->getEnvironment()->m_eventSystem;

	m_sizeEventID = eventSystem->registerInstancedEvent(WindowSizeEvent::getInstancedEventsType());
	VT_CHECK_RETURN_FALSE(m_sizeEventID != InvalidInstancedEventID);

	return true;
}

void VT::WindowBase::releaseEvents()
{
	IEventSystem* eventSystem = EngineInstance::getInstance()->getEnvironment()->m_eventSystem;

	if (m_sizeEventID != InvalidInstancedEventID)
	{
		eventSystem->unregisterInstancedEvent(m_sizeEventID);
		m_sizeEventID = InvalidInstancedEventID;
	}
}
