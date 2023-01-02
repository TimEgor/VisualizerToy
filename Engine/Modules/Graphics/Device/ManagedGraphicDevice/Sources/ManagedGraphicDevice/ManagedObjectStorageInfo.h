#pragma once

namespace VT::ManagedGraphicDevice
{
	class ManagedStorageInfoBase {};

	template <typename BaseManagedObjectType>
	class ManagedStorageInfo : ManagedStorageInfoBase
	{
	public:
		using BaseObjectType = BaseManagedObjectType;
		using HandleType = typename BaseManagedObjectType::HandleType;

		struct NewObjectInfo final
		{
			BaseObjectType* m_objectPtr = nullptr;
			HandleType m_objectHandle = HandleType();
		};
	};

#define MANAGED_OBJECT_STORAGE_INFO(NAME_INFO, BASE_OBJECT_TYPE)					\
	using NAME_INFO##StorageInfoBase = ManagedStorageInfo<BASE_OBJECT_TYPE>;		\
																					\
	template <typename NAME_INFO##Type>												\
	class NAME_INFO##StorageInfo : public NAME_INFO##StorageInfoBase				\
	{																				\
		static_assert(std::is_base_of<BASE_OBJECT_TYPE, NAME_INFO##Type>::value);	\
																					\
	public:																			\
		using ObjectType = NAME_INFO##Type;											\
		using BaseStorageInfo = NAME_INFO##StorageInfoBase;							\
	};
}