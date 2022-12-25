#pragma once

#include "GraphicSynchronization/IFence.h"
#include "GraphicSynchronization/ISemaphore.h"
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


	using ManagedSemaphoreHandle = ObjectPoolHandle8;
	using ManagedSemaphoreObject = ManagedGraphicObject<ManagedSemaphoreHandle>;

	class ManagedSemaphoreBase : public ISemaphore, public ManagedSemaphoreObject
	{
	public:
		ManagedSemaphoreBase() = default;
	};
}