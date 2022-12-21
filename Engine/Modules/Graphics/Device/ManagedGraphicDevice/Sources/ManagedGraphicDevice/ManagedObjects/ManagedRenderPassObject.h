#pragma once

#include "GraphicPipeline/IRenderPass.h"
#include "ManagedGraphicDevice/ManagedGraphicObject.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedRenderPassHandle = ObjectPoolHandle16;
	using ManagedRenderPassObject = ManagedGraphicObject<ManagedRenderPassHandle>;

	class ManagedRenderPassBase : public IRenderPass, public ManagedRenderPassObject
	{
	public:
		ManagedRenderPassBase() = default;
	};
}