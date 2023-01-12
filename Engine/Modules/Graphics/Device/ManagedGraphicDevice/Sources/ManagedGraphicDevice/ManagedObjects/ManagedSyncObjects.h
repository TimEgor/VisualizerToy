#pragma once

#include "GraphicSynchronization/IFence.h"
#include "ManagedGraphicDevice/ManagedGraphicObject.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedFenceHandle = ObjectPoolHandle8;
	using ManagedFenceObject = ManagedGraphicObject<ManagedFenceHandle>;

	class ManagedFenceBase : public IFence, public ManagedFenceObject
	{
	public:
		ManagedFenceBase() = default;
	};
}