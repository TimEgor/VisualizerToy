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
		ManagedGraphicResourceHandle(typename BaseType::BaseResourceType* resource, typename BaseType::HandleType id)
			: BaseType(resource, id) {}
		~ManagedGraphicResourceHandle();
	};

	template <typename ManagedBaseType>
	class NamedManagedGraphicResourceHandle final : public ManagedBaseType
	{
	private:
		FileNameID m_nameID;

		virtual void selfDestroy() override;

	public:
		NamedManagedGraphicResourceHandle() = default;
		NamedManagedGraphicResourceHandle(typename ManagedBaseType::ResourceHandleBaseType::BaseResourceType* resource, 
			typename ManagedBaseType::ResourceHandleBaseType::HandleType id, FileNameID nameID)
			: ManagedBaseType(resource, id), m_nameID(nameID) {}
		~NamedManagedGraphicResourceHandle() {}

		FileNameID getNameID() const { return m_nameID; }
	};

#define MANAGED_GRAPHIC_RESOURCE_TEST(BASE_TYPE)	\
	using Managed##BASE_TYPE = ManagedGraphicResourceHandle<BASE_TYPE>;

#define NAMED_MANAGED_GRAPHIC_RESOURCE_TEST(BASE_TYPE)	\
	using NamedManaged##BASE_TYPE = NamedManagedGraphicResourceHandle<ManagedGraphicResourceHandle<BASE_TYPE>>;

	MANAGED_GRAPHIC_RESOURCE_TEST(Texture2DResourceHandle)

	MANAGED_GRAPHIC_RESOURCE_TEST(VertexShaderResourceHandle)
	MANAGED_GRAPHIC_RESOURCE_TEST(PixelShaderResourceHandle)

	NAMED_MANAGED_GRAPHIC_RESOURCE_TEST(VertexShaderResourceHandle)
	NAMED_MANAGED_GRAPHIC_RESOURCE_TEST(PixelShaderResourceHandle)
}