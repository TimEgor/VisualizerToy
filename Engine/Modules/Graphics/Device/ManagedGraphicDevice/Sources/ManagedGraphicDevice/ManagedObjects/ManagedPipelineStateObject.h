#pragma once

#include "GraphicPipeline/IPipelineState.h"
#include "ManagedGraphicDevice/ManagedGraphicObject.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedPipelineStateHandle = ObjectPoolHandle32;
	using ManagedPipelineStateObject = ManagedGraphicObject<ManagedPipelineStateHandle>;

	class ManagedPipelineStateBase : public IPipelineState, public ManagedPipelineStateObject
	{
	public:
		ManagedPipelineStateBase() = default;
	};
}