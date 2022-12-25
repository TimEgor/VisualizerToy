#pragma once

#include "GraphicResourceManager/ResourceHandles.h"
#include "Core/FileName/FileName.h"

namespace VT
{
	template <typename BaseType>
	class ManagedGraphicResourceHandle : public BaseType
	{
	public:
		using ResourceHandleBaseType = BaseType;

	private:
		virtual void selfDestroy() override;

	public:
		ManagedGraphicResourceHandle() = default;
		ManagedGraphicResourceHandle(typename BaseType::BaseResource* resource, typename BaseType::HandleID handleID)
			: BaseType(resource, handleID) {}
		~ManagedGraphicResourceHandle();

		void setResource(typename BaseType::BaseResource* resource) { m_resourcePtr = resource; }
	};

	template <typename ManagedBaseType>
	class NamedManagedGraphicResourceHandle final : public ManagedBaseType
	{
	private:
		FileNameID m_nameID;

		virtual void selfDestroy() override;

	public:
		NamedManagedGraphicResourceHandle() = default;
		NamedManagedGraphicResourceHandle(typename ManagedBaseType::ResourceHandleBaseType::BaseResource* resource, 
			typename ManagedBaseType::ResourceHandleBaseType::HandleID handleID, FileNameID nameID)
			: ManagedBaseType(resource, handleID), m_nameID(nameID) {}
		~NamedManagedGraphicResourceHandle() {}

		FileNameID getNameID() const { return m_nameID; }
	};

	template <typename BaseType>
	class ManagedGraphicResourceViewHandle : public BaseType
	{
	public:
		using ResourceHandleBaseType = BaseType;

	private:
		virtual void selfDestroy() override;

	public:
		ManagedGraphicResourceViewHandle() = default;
		ManagedGraphicResourceViewHandle(typename BaseType::BaseResourceView* resourceView,
			typename BaseType::HandleID viewHandleID, typename BaseType::ResourceHandleID resourceHandleID)
			: BaseType(resourceView, viewHandleID, resourceHandleID) {}
		~ManagedGraphicResourceViewHandle();
	};

#define MANAGED_GRAPHIC_RESOURCE(BASE_TYPE)	\
	using Managed##BASE_TYPE = ManagedGraphicResourceHandle<BASE_TYPE>;

#define NAMED_MANAGED_GRAPHIC_RESOURCE(BASE_TYPE)	\
	using NamedManaged##BASE_TYPE = NamedManagedGraphicResourceHandle<ManagedGraphicResourceHandle<BASE_TYPE>>;

#define MANAGED_GRAPHIC_RESOURCE_VIEW(BASE_TYPE)	\
	using Managed##BASE_TYPE = ManagedGraphicResourceViewHandle<BASE_TYPE>;

	MANAGED_GRAPHIC_RESOURCE(Texture2DResourceHandle)
	MANAGED_GRAPHIC_RESOURCE_VIEW(Texture2DViewResourceHandle)

	MANAGED_GRAPHIC_RESOURCE(VertexShaderResourceHandle)
	MANAGED_GRAPHIC_RESOURCE(PixelShaderResourceHandle)

	MANAGED_GRAPHIC_RESOURCE(PipelineStateResourceHandle)

	NAMED_MANAGED_GRAPHIC_RESOURCE(VertexShaderResourceHandle)
	NAMED_MANAGED_GRAPHIC_RESOURCE(PixelShaderResourceHandle)
}