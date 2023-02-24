#pragma once

#include "GraphicPipeline/IPipelineBindingLayout.h"
#include "GraphicPipeline/IPipelineState.h"
#include "ManagedGraphicDevice/ManagedGraphicObject.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedPipelineStateHandle = ObjectPoolHandle32;
	using ManagedPipelineStateObject = ManagedGraphicObject<ManagedPipelineStateHandle>;

	class ManagedGraphicPipelineStateBase : public IGraphicPipelineState, public ManagedPipelineStateObject
	{
	public:
		ManagedGraphicPipelineStateBase() = default;
	};

	class ManagedComputePipelineStateBase : public IComputePipelineState, public ManagedPipelineStateObject
	{
	public:
		ManagedComputePipelineStateBase() = default;
	};

	using ManagedPipelineBindingLayoutHandle = ObjectPoolHandle16;
	using ManagedPipelineBindingLayoutObject = ManagedGraphicObject<ManagedPipelineBindingLayoutHandle>;

	class ManagedPipelineBindingLayoutBase : public IPipelineBindingLayout, public ManagedPipelineBindingLayoutObject
	{
	public:
		ManagedPipelineBindingLayoutBase() = default;
	};
}