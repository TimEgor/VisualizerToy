#pragma once

#include "GraphicResourceManager/ResourceHandles.h"

namespace VT
{
#define MANAGED_GRAPHIC_RESOURCE(BASE_TYPE)																			\
	class Managed##BASE_TYPE final : public BASE_TYPE																\
	{																												\
	private:																										\
		virtual void selfDestroy() override;																		\
	public:																											\
		Managed##BASE_TYPE##() = default;																			\
		Managed##BASE_TYPE##(typename BASE_TYPE::BaseResourceType* resource, typename BASE_TYPE::HandleType id)		\
			: BASE_TYPE(resource, id) {}																			\
		~Managed##BASE_TYPE##();																					\
	};

	MANAGED_GRAPHIC_RESOURCE(Texture2DResourceHandle)

	MANAGED_GRAPHIC_RESOURCE(VertexShaderResourceHandle)
	MANAGED_GRAPHIC_RESOURCE(PixelShaderResourceHandle)
}