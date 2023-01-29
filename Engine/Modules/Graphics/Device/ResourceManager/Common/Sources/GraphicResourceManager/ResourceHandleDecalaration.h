#pragma once

#include "ObjectHandleDeclaration.h"

namespace VT
{
	class IGraphicResource;
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


	COUNTABLE_REFERENCES_DECLARATION(GraphicResourceHandle)

#define GRAPHIC_TYPED_RESOURCE(NAME, INTERFACE_TYPE)								\
	class INTERFACE_TYPE;															\
	using NAME##GraphicResourceHandle = GraphicTypedResourceHandle<INTERFACE_TYPE>;	\
	COUNTABLE_REFERENCES_DECLARATION_BY_NAME(NAME##GraphicResourceHandle, NAME)
}