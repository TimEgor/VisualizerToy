#pragma once

#include "Multithreading/CountableObject/CountableReference.h"
#include "Core/FileName/FileName.h"

#include <cstdint>
#include <type_traits>

namespace VT
{
	using GraphicResourceName = FileName;
	using GraphicResourceNameID = FileNameID;

	template <typename ResourceType, typename HandleIDType>
	class GraphicResourceHandle : public CountableObjectBase
	{
	public:
		using BaseResourceType = ResourceType;
		using HandleType = HandleIDType;

		static_assert(std::is_integral<HandleIDType>::value);

	protected:
		ResourceType* m_resourcePtr = nullptr;
		HandleIDType m_resourceHandle = 0;

	public:
		GraphicResourceHandle() = default;
		GraphicResourceHandle(ResourceType* resource, HandleIDType handle)
			: m_resourcePtr(resource), m_resourceHandle(handle) {}

		ResourceType* getResource() const { return m_resourcePtr; }
		HandleIDType getID() const { return m_resourceHandle; }
	};

	template <typename ResourceViewType, typename ResourceType, typename HandleIDType>
	class GraphicResourceViewHandle : public GraphicResourceHandle<ResourceType, HandleIDType>
	{
	public:
		using BaseResourceViewType = ResourceViewType;

	protected:
		ResourceViewType* m_resourceViewPtr = nullptr;

	public:
		GraphicResourceViewHandle() = default;
		GraphicResourceViewHandle(ResourceViewType* resourceView, ResourceType* resource, HandleIDType handle)
			: GraphicResourceHandle(resource, handle), m_resourceViewPtr(resourceView) {}

		ResourceType* getResourceView() const { return m_resourceViewPtr; }
	};

#define GRAPHIC_RESOURCE(NAME, INTERFACE_TYPE, HANDLE_ID_TYPE)							\
	class INTERFACE_TYPE;																\
	using NAME##HandleID = HANDLE_ID_TYPE;												\
	using NAME##ResourceHandle = GraphicResourceHandle<INTERFACE_TYPE, NAME##HandleID>;	\
	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(NAME##ResourceHandle, NAME)

#define GRAPHIC_RESOURCE_VIEW(NAME, INTERFACE_VIEW_TYPE, INTERFACE_TYPE, HANDLE_ID_TYPE)							\
	class INTERFACE_VIEW_TYPE;																						\
	using NAME##HandleID = HANDLE_ID_TYPE;																			\
	using NAME##ResourceHandle = GraphicResourceViewHandle<INTERFACE_VIEW_TYPE, INTERFACE_TYPE, NAME##HandleID>;	\
	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(NAME##ResourceHandle, NAME)

	GRAPHIC_RESOURCE(Texture2D, ITexture2D, uint32_t);
	GRAPHIC_RESOURCE_VIEW(Texture2DView, ITexture2DView, ITexture2D, uint32_t);

	GRAPHIC_RESOURCE(VertexShader, IVertexShader, uint32_t);
	GRAPHIC_RESOURCE(PixelShader, IPixelShader, uint32_t);

	GRAPHIC_RESOURCE(PipelineState, IPipelineState, uint32_t);
}