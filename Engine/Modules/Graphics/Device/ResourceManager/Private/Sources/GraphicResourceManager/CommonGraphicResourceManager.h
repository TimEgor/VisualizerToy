#include "GraphicResourceManager/GraphicResourceManager.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "ResourceSystem/IResourceSystem.h"

#include <cassert>

namespace VT
{
	inline IGraphicDevice* getGraphicDevice()
	{
		return EngineInstance::getInstance()->getEnvironment()->m_graphicDevice;
	}

	inline IResourceSystem* getResourceSystem()
	{
		return EngineInstance::getInstance()->getEnvironment()->m_resourceSystem;
	}
}