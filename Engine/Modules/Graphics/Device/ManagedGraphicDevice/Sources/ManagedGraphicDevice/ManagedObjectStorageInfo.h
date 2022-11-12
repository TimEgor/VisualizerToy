#pragma once

#include "ManagedGraphicObjects.h"

#include <type_traits>

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

	using ManagedSwapChainStorageInfoBase = ManagedStorageInfo<ManagedSwapChainBase>;
	using ManagedTexture2DStorageInfoBase = ManagedStorageInfo<ManagedTexture2DBase>;

	template <typename ManagedSwapChainType>
	class ManagedSwapChainStorageInfo : public ManagedSwapChainStorageInfoBase
	{
		static_assert(std::is_base_of<ManagedSwapChainBase, ManagedSwapChainType>::value);

	public:
		using ObjectType = ManagedSwapChainType;
		using BaseStorageInfo = ManagedSwapChainStorageInfoBase;
	};

	template <typename ManagedTexture2DType>
	class ManagedTexture2DStorageInfo : public ManagedTexture2DStorageInfoBase
	{
		static_assert(std::is_base_of<ManagedTexture2DBase, ManagedTexture2DType>::value);

	public:
		using ObjectType = ManagedTexture2DType;
		using BaseStorageInfo = ManagedTexture2DStorageInfoBase;
	};
}