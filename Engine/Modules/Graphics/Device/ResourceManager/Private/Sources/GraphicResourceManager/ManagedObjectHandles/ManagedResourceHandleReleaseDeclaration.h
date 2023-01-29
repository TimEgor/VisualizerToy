#pragma once

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicResourceManager/GraphicResourceManager.h"
#include "GraphicResourceManager/NamedGraphicResourceSystem.h"

namespace VT
{
	inline IGraphicResourceManager* getEnvironmentGraphicResourceManager()
	{
		return EngineInstance::getInstance()->getEnvironment()->m_graphicResourceManager;
	}

	inline GraphicResourceManager* getResourceManager()
	{
		return reinterpret_cast<GraphicResourceManager*>(getEnvironmentGraphicResourceManager());
	}

	inline NamedGraphicResourceSystem* getNamedResourceManager()
	{
		return reinterpret_cast<NamedGraphicResourceSystem*>(getEnvironmentGraphicResourceManager());
	}

#define MANAGED_GRAPHIC_RESOURCE_DESTROY_RELEASE_IMPL(TYPE, FUNCTION_DSCTOR, FUNCTION_REF_RELEASE)				\
	void ManagedGraphicResourceHandle<##TYPE##>::selfDestroy()													\
	{																											\
		getResourceManager()->FUNCTION_REF_RELEASE(m_id);														\
	}																											\
	ManagedGraphicResourceHandle<##TYPE##>::~ManagedGraphicResourceHandle<##TYPE##>()							\
	{																											\
		if (m_objectPtr)																						\
		{																										\
			getResourceManager()->FUNCTION_DSCTOR(reinterpret_cast<TYPE::HandleObjectType*>(m_objectPtr));		\
		}																										\
	}

#define MANAGED_GRAPHIC_RESOURCE_RELEASE_IMPL(TYPE, FUNCTION_DSCTOR)											\
	void ManagedGraphicResourceHandle<##TYPE##>::selfDestroy()													\
	{																											\
	}																											\
	ManagedGraphicResourceHandle<##TYPE##>::~ManagedGraphicResourceHandle<##TYPE##>()							\
	{																											\
		if (m_objectPtr)																						\
		{																										\
			getResourceManager()->FUNCTION_DSCTOR(reinterpret_cast<TYPE::HandleObjectType*>(m_objectPtr));		\
		}																										\
	}

#define NAMED_MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(TYPE, FUNCTION_REF_RELEASE)								\
	void NamedManagedGraphicResourceHandle<ManagedGraphicResourceHandle<##TYPE##>>::selfDestroy()			\
	{																										\
		if (m_nameID != InvalidFileNameID)																	\
		{																									\
			getNamedResourceManager()->FUNCTION_REF_RELEASE(m_nameID);										\
		}																									\
	}
}
