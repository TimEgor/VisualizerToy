#pragma once

#include "D3D12GraphicsPlugin/D3D12Core.h"
#include "GraphicPipeline/IPipelineState.h"

namespace VT_D3D12
{
	inline D3D12_COMPARISON_FUNC convertComparisonFuncVTtoD3D12(VT::GraphicComparisonFunc func)
	{
		switch (func)
		{
		case VT::GraphicComparisonFunc::NEVER:
			return D3D12_COMPARISON_FUNC_NEVER;
		case VT::GraphicComparisonFunc::EQUAL:
			return D3D12_COMPARISON_FUNC_EQUAL;
		case VT::GraphicComparisonFunc::NOT_EQUAL:
			return D3D12_COMPARISON_FUNC_NOT_EQUAL;
		case VT::GraphicComparisonFunc::LESS:
			return D3D12_COMPARISON_FUNC_LESS;
		case VT::GraphicComparisonFunc::LESS_EQUAL:
			return D3D12_COMPARISON_FUNC_LESS_EQUAL;
		case VT::GraphicComparisonFunc::GREATER:
			return D3D12_COMPARISON_FUNC_GREATER;
		case VT::GraphicComparisonFunc::GREATER_EQUAL:
			return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		case VT::GraphicComparisonFunc::ALWAYS:
			return D3D12_COMPARISON_FUNC_ALWAYS;
		default:
			return D3D12_COMPARISON_FUNC_NONE;
		}
	}
}
