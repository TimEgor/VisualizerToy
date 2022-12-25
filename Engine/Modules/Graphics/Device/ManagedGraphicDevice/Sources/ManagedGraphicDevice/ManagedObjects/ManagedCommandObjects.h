#pragma once

#include "GraphicCommandResources/ICommandList.h"
#include "ManagedGraphicDevice/ManagedGraphicObject.h"
#include "GraphicCommandResources/ICommandPool.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedCommandPoolHandle = ObjectPoolHandle8;
	using ManagedCommandPoolObject = ManagedGraphicObject<ManagedCommandPoolHandle>;

	class ManagedCommandPoolBase : public ICommandPool, public ManagedCommandPoolObject {};

	using ManagedCommandListHandle = ObjectPoolHandle16;
	using ManagedCommandListObject = ManagedGraphicObject<ManagedCommandListHandle>;

	class ManagedCommandListBase : public ICommandList, public ManagedCommandListObject {};
}