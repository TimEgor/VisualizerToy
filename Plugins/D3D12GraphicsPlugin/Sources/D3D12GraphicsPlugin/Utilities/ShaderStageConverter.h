#pragma once

#include "D3D12GraphicsPlugin/D3D12Core.h"

namespace VT_D3D12
{
	inline D3D12_SHADER_VISIBILITY convertShaderStageVisibilityVTtoD3D12(VT::ShaderStageVisibility stage)
	{
		switch (stage)
		{
		case VT::VERTEX_STAGE:
			return D3D12_SHADER_VISIBILITY_VERTEX;
		case VT::HULL_STAGE:
			return D3D12_SHADER_VISIBILITY_HULL;
		case VT::DOMAIN_STAGE:
			return D3D12_SHADER_VISIBILITY_DOMAIN;
		case VT::GEOMETRY_STAGE:
			return D3D12_SHADER_VISIBILITY_GEOMETRY;
		case VT::PIXEL_STAGE:
			return D3D12_SHADER_VISIBILITY_PIXEL;
		case VT::ALL_STAGES:
		default: ;
			return D3D12_SHADER_VISIBILITY_ALL;
		}
	}
}
