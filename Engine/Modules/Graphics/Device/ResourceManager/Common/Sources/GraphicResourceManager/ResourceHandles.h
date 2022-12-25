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
		using BaseResource = ResourceType;
		using HandleID = HandleIDType;

		static_assert(std::is_integral<HandleIDType>::value);

	protected:
		ResourceType* m_resourcePtr = nullptr;
		HandleIDType m_id = 0;

	public:
		GraphicResourceHandle() = default;
		GraphicResourceHandle(ResourceType* resource, HandleIDType handleID)
			: m_resourcePtr(resource), m_id(handleID) {}

		ResourceType* getResource() const { return m_resourcePtr; }
		HandleIDType getID() const { return m_id; }
	};

	template <typename ResourceViewType, typename ResourceHandleIDType, typename HandleIDType>
	class GraphicResourceViewHandle : public CountableObjectBase
	{
	public:
		using BaseResourceView = ResourceViewType;
		using HandleID = HandleIDType;
		using ResourceHandleID = ResourceHandleIDType;

	protected:
		ResourceViewType* m_resourceViewPtr = nullptr;
		HandleIDType m_id = 0;
		ResourceHandleIDType m_resourceHandleID = 0;

	public:
		GraphicResourceViewHandle() = default;
		GraphicResourceViewHandle(ResourceViewType* view, HandleIDType viewHandleID, ResourceHandleIDType resourceHandleID)
			: m_resourceViewPtr(view), m_id(viewHandleID), m_resourceHandleID(resourceHandleID) {}

		ResourceViewType* getResourceView() const { return m_resourceViewPtr; }
		HandleIDType getID() const { return m_id; }
		ResourceHandleIDType getResourceHandleID() const { return m_resourceHandleID; }
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
	GRAPHIC_RESOURCE_VIEW(Texture2DView, ITexture2DView, Texture2DHandleID, uint32_t);

	GRAPHIC_RESOURCE(VertexShader, IVertexShader, uint32_t);
	GRAPHIC_RESOURCE(PixelShader, IPixelShader, uint32_t);

	GRAPHIC_RESOURCE(PipelineState, IPipelineState, uint32_t);
}