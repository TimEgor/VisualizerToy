#pragma once

#include "ManagedGraphicDevice/ManagedGraphicObject.h"
#include "SwapChain/SwapChainBase.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedSwapChainHandle = ObjectPoolHandle8;
	using ManagedSwapChainObject = ManagedGraphicObject<ManagedSwapChainHandle>;

	class ManagedSwapChainBase : public SwapChainBase, public ManagedSwapChainObject
	{
	public:
		ManagedSwapChainBase(const SwapChainDesc& desc) : SwapChainBase(desc) {}
	};
}