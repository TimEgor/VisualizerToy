#pragma once

#include "ObjectHandleDeclaration.h"
#include "GraphicResourceCommon/IGraphicResource.h"

namespace VT
{
	class IGraphicResourceDescriptor;

	using GraphicResourceHandleID = uint32_t;

	class GraphicResourceHandle : public GraphicTypedObjectHandle<IGraphicResource, GraphicResourceHandleID>
	{
	public:
		GraphicResourceHandle() = default;
		GraphicResourceHandle(IGraphicResource* resource, GraphicResourceHandleID handleID)
			: GraphicTypedObjectHandle(resource, handleID) {}

		IGraphicResource* getResource() const { return reinterpret_cast<IGraphicResource*>(m_objectPtr); }

		template <typename Type>
		Type* getResourceCast() const
		{
			static_assert(std::is_base_of_v<IGraphicResource, Type>);

			return reinterpret_cast<Type*>(getResource());
		}
	};

	template <typename ResourceType>
	class GraphicTypedResourceHandle : public GraphicResourceHandle
	{
	public:
		using HandleObjectType = ResourceType;

	public:
		GraphicTypedResourceHandle() = default;
		GraphicTypedResourceHandle(ResourceType* resource, GraphicResourceHandleID handleID)
			: GraphicResourceHandle(resource, handleID) {}

		ResourceType* getTypedResource() const { return getResourceCast<ResourceType>(); }
	};

	template <typename ViewHandleIDType>
	class GraphicResourceViewHandle : public CountableObjectBase
	{
	public:
		using ViewHandleID = ViewHandleIDType;

	protected:
		IGraphicResourceDescriptor* m_resourceDescriptor = nullptr;
		ViewHandleID m_id = 0;

		GraphicResourceHandleID m_resourceHandleID = 0;
		GraphicResourceType m_resourceType = 0; 

	public:
		GraphicResourceViewHandle() = default;
		GraphicResourceViewHandle(IGraphicResourceDescriptor* descriptor, ViewHandleID viewHandleID,
			GraphicResourceHandleID resourceHandleID, GraphicResourceType resourceType)
			: m_resourceDescriptor(descriptor), m_id(viewHandleID),
			m_resourceHandleID(resourceHandleID), m_resourceType(resourceType) {}

		IGraphicResourceDescriptor* getResourceView() const { return m_resourceDescriptor; }
		ViewHandleID getID() const { return m_id; }

		GraphicResourceHandleID getResourceHandleID() const { return m_resourceHandleID; }
		GraphicResourceType getResourceType() const { return m_resourceType; }
	};

	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(GraphicResourceHandle, GraphicResource)

#define GRAPHIC_TYPED_RESOURCE(NAME, INTERFACE_TYPE)								\
	class INTERFACE_TYPE;															\
	using NAME##GraphicResourceHandle = GraphicTypedResourceHandle<INTERFACE_TYPE>;	\
	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(NAME##GraphicResourceHandle, NAME)

#define GRAPHIC_RESOURCE_VIEW(NAME, BASE_NAME_REF, VIEW_HANDLE_ID_TYPE)						\
	using NAME##GraphicResourceViewHandle = GraphicResourceViewHandle<VIEW_HANDLE_ID_TYPE>;	\
	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(NAME##GraphicResourceViewHandle, BASE_NAME_REF)
}