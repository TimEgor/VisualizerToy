#pragma once

#include "ManagedGraphicDevice/ManagedGraphicObject.h"
#include "GraphicCommandResources/ICommandPool.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedCommandPoolHandle = ObjectPoolHandle8;
	using ManagedCommandPoolObject = ManagedGraphicObject<ManagedCommandPoolHandle>;

	class ManagedCommandPoolBase : public ICommandPool, public ManagedCommandPoolObject {};
}