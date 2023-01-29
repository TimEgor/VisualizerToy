#pragma once

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
		ManagedGraphicResourceHandle(typename BaseType::HandleObjectType* object, typename BaseType::HandleID handleID)
			: BaseType(object, handleID) {}
		~ManagedGraphicResourceHandle();

		void setHandleObject(typename BaseType::HandleObjectType* object) { m_objectPtr = object; }
	};

	template <typename ManagedBaseType>
	class NamedManagedGraphicResourceHandle final : public ManagedBaseType
	{
	private:
		FileNameID m_nameID;

		virtual void selfDestroy() override;

	public:
		NamedManagedGraphicResourceHandle() = default;
		NamedManagedGraphicResourceHandle(typename ManagedBaseType::ResourceHandleBaseType::HandleObjectType* object,
			typename ManagedBaseType::ResourceHandleBaseType::HandleID handleID, FileNameID nameID)
			: ManagedBaseType(object, handleID), m_nameID(nameID) {}
		~NamedManagedGraphicResourceHandle() {}

		FileNameID getNameID() const { return m_nameID; }
	};

#define MANAGED_GRAPHIC_RESOURCE(BASE_TYPE)			\
	using Managed##BASE_TYPE = ManagedGraphicResourceHandle<BASE_TYPE>;

#define NAMED_MANAGED_GRAPHIC_RESOURCE(BASE_TYPE)	\
	using NamedManaged##BASE_TYPE = NamedManagedGraphicResourceHandle<ManagedGraphicResourceHandle<BASE_TYPE>>;
}