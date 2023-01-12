#pragma once

#include "GraphicCommandResources/ICommandList.h"
#include "ManagedGraphicDevice/ManagedGraphicObject.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedCommandListHandle = ObjectPoolHandle16;
	using ManagedCommandListObject = ManagedGraphicObject<ManagedCommandListHandle>;

	class ManagedCommandListBase : public ICommandList, public ManagedCommandListObject {};
}