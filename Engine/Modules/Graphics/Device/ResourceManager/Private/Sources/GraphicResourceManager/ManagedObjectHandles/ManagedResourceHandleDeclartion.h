#pragma once

#include "Core/FileName/FileName.h"

namespace VT
{
	template <typename BaseHandleType>
	class ManagedGraphicObjectHandle : public BaseHandleType
	{
	public:
		using ResourceHandleBaseType = BaseHandleType;

	private:
		virtual void selfDestroy() override;

	public:
		ManagedGraphicObjectHandle() = default;
		ManagedGraphicObjectHandle(typename BaseHandleType::HandleObjectType* object, typename BaseHandleType::HandleID handleID)
			: BaseHandleType(object, handleID) {}
		~ManagedGraphicObjectHandle();

		void setHandleObject(typename BaseHandleType::HandleObjectType* object) { m_objectPtr = object; }
	};

	template <typename ManagedBaseHandleType>
	class NamedManagedGraphicObjectHandle final : public ManagedBaseHandleType
	{
	private:
		FileNameID m_nameID;

		virtual void selfDestroy() override;

	public:
		NamedManagedGraphicObjectHandle() = default;
		NamedManagedGraphicObjectHandle(typename ManagedBaseHandleType::ResourceHandleBaseType::HandleObjectType* object,
			typename ManagedBaseHandleType::ResourceHandleBaseType::HandleID handleID, FileNameID nameID)
			: ManagedBaseHandleType(object, handleID), m_nameID(nameID) {}
		~NamedManagedGraphicObjectHandle() {}

		FileNameID getNameID() const { return m_nameID; }
	};

	template <typename BaseViewHandleType>
	class ManagedGraphicResourceViewHandle : public BaseViewHandleType
	{
	public:
		using ResourceViewHandleBaseType = BaseViewHandleType;

	private:
		virtual void selfDestroy() override;

	public:
		ManagedGraphicResourceViewHandle() = default;
		ManagedGraphicResourceViewHandle(IGraphicResourceDescriptor* descriptor, typename BaseViewHandleType::ViewHandleID viewHandleID,
			GraphicResourceHandleID resourceHandleID, GraphicResourceType resourceType)
			: BaseViewHandleType(descriptor, viewHandleID, resourceHandleID, resourceType) {}
		~ManagedGraphicResourceViewHandle();
	};

#define MANAGED_GRAPHIC_OBJECT(BASE_TYPE)	\
	using Managed##BASE_TYPE = ManagedGraphicObjectHandle<BASE_TYPE>;

#define NAMED_MANAGED_GRAPHIC_OBJECT(BASE_MANAGED_HANDLE_TYPE)	\
	using NamedManaged##BASE_MANAGED_HANDLE_TYPE = NamedManagedGraphicObjectHandle<ManagedGraphicObjectHandle<BASE_MANAGED_HANDLE_TYPE>>;

#define MANAGED_GRAPHIC_VIEW_OBJECT(BASE_VIEW_TYPE)	\
	using Managed##BASE_VIEW_TYPE = ManagedGraphicResourceViewHandle<BASE_VIEW_TYPE>;
}