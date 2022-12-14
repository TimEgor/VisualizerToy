#include "ManagedResourceHandles.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicResourceManager.h"
#include "NamedGraphicResourceSystem.h"

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

#define MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(TYPE, FUNCTION_DSCTOR, FUNCTION_REF_RELEASE)	\
	void ManagedGraphicResourceHandle<##TYPE##>::selfDestroy()								\
	{																						\
		getResourceManager()->FUNCTION_REF_RELEASE(m_resourceHandle);						\
	}																						\
	ManagedGraphicResourceHandle<##TYPE##>::~ManagedGraphicResourceHandle<##TYPE##>()		\
	{																						\
		if (m_resourcePtr)																	\
		{																					\
			getResourceManager()->FUNCTION_DSCTOR(m_resourcePtr);							\
		}																					\
	}

#define NAMED_MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(TYPE, FUNCTION_REF_RELEASE)						\
	void NamedManagedGraphicResourceHandle<ManagedGraphicResourceHandle<##TYPE##>>::selfDestroy()	\
	{																								\
		if (m_nameID != InvalidFileNameID)															\
		{																							\
			getNamedResourceManager()->FUNCTION_REF_RELEASE(m_nameID);								\
		}																							\
	}

	MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(Texture2DResourceHandle, deleteTexture2DInternal, deleteTexture2DReference)

	MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(VertexShaderResourceHandle, deleteVertexShaderInternal, deleteVertexShaderReference)
	MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(PixelShaderResourceHandle, deletePixelShaderInternal, deletePixelShaderReference)

	NAMED_MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(VertexShaderResourceHandle, deleteVertexShaderReference)
	NAMED_MANAGED_GRAPHIC_RESOURCE_DESTROY_IMPL(PixelShaderResourceHandle, deletePixelShaderReference)
}