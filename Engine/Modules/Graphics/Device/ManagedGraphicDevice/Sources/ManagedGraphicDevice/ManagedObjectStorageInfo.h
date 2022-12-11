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
}