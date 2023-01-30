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

#define MANAGED_GRAPHIC_OBJECT_DESTROY_RELEASE_IMPL(TYPE, FUNCTION_DSCTOR, FUNCTION_REF_RELEASE)				\
	void ManagedGraphicObjectHandle<##TYPE##>::selfDestroy()													\
	{																											\
		getResourceManager()->FUNCTION_REF_RELEASE(m_id);														\
	}																											\
	ManagedGraphicObjectHandle<##TYPE##>::~ManagedGraphicObjectHandle<##TYPE##>()								\
	{																											\
		if (m_objectPtr)																						\
		{																										\
			getResourceManager()->FUNCTION_DSCTOR(reinterpret_cast<TYPE::HandleObjectType*>(m_objectPtr));		\
		}																										\
	}

#define MANAGED_GRAPHIC_OBJECT_RELEASE_IMPL(TYPE, FUNCTION_DSCTOR)												\
	void ManagedGraphicObjectHandle<##TYPE##>::selfDestroy()													\
	{																											\
	}																											\
	ManagedGraphicObjectHandle<##TYPE##>::~ManagedGraphicObjectHandle<##TYPE##>()								\
	{																											\
		if (m_objectPtr)																						\
		{																										\
			getResourceManager()->FUNCTION_DSCTOR(reinterpret_cast<TYPE::HandleObjectType*>(m_objectPtr));		\
		}																										\
	}

#define NAMED_MANAGED_GRAPHIC_OBJECT_DESTROY_IMPL(TYPE, FUNCTION_REF_RELEASE)									\
	void NamedManagedGraphicObjectHandle<ManagedGraphicObjectHandle<##TYPE##>>::selfDestroy()					\
	{																											\
		if (m_nameID != InvalidFileNameID)																		\
		{																										\
			getNamedResourceManager()->FUNCTION_REF_RELEASE(m_nameID);											\
		}																										\
	}

#define MANAGED_GRAPHIC_OBJECT_VIEW_DESTROY_RELEASE_IMPL(VIEW_TYPE, FUNCTION_DSCTOR, FUNCTION_REF_RELEASE)		\
	void ManagedGraphicResourceViewHandle<##VIEW_TYPE##>::selfDestroy()											\
	{																											\
		getResourceManager()->FUNCTION_REF_RELEASE(m_id);														\
	}																											\
	ManagedGraphicResourceViewHandle<##VIEW_TYPE##>::~ManagedGraphicResourceViewHandle<##VIEW_TYPE##>()			\
	{																											\
		if (m_resourceDescriptor)																				\
		{																										\
			getResourceManager()->FUNCTION_DSCTOR(m_resourceDescriptor);										\
		}																										\
	}
}