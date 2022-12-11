#pragma once

#include "ManagedGraphicDevice/ManagedObjectStorageInfo.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedShaderObjects.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedTexture2DObject.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedSwapChainObject.h"
#include "ManagedGraphicDevice/ManagedObjects/ManagedCommandPoolObject.h"

namespace VT::ManagedGraphicDevice
{
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

	MANAGED_OBJECT_STORAGE_INFO(ManagedSwapChain, ManagedSwapChainBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedTexture2D, ManagedTexture2DBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedVertexShader, ManagedVertexShaderBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedPixelShader, ManagedPixelShaderBase)
	MANAGED_OBJECT_STORAGE_INFO(ManagedCommandPool, ManagedCommandPoolBase)
}