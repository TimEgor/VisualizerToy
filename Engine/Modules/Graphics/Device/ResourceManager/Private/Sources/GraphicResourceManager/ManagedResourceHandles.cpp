#include "ManagedResourceHandles.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicResourceManager.h"

namespace VT
{
#define MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(TYPE, FUNCTION_DSCTOR, FUNCTION_REF_RELEASE)	\
	void TYPE##::selfDestroy()																\
	{																						\
		reinterpret_cast<GraphicResourceManager*>(											\
			EngineInstance::getInstance()->													\
			getEnvironment()->m_graphicResourceManager										\
		)->FUNCTION_REF_RELEASE(m_resourceHandle);											\
	}																						\
	TYPE##::~##TYPE##()																		\
	{																						\
		reinterpret_cast<GraphicResourceManager*>(											\
			EngineInstance::getInstance()->													\
			getEnvironment()->m_graphicResourceManager										\
		)->FUNCTION_DSCTOR(m_resourcePtr);													\
	}

	MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(ManagedTexture2DResourceHandle, deleteTexture2DInternal, deleteTexture2DReference)

	MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(ManagedVertexShaderResourceHandle, deleteVertexShaderInternal, deleteVertexShaderReference)
	MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(ManagedPixelShaderResourceHandle, deletePixelShaderInternal, deletePixelShaderReference)

}