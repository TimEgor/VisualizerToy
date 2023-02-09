#pragma once

#include "D3D12GraphicsPlugin/D3D12Core.h"
#include "Textures/TextureCommon.h"

namespace VT_D3D12
{
	inline D3D12_RESOURCE_STATES convertBufferStateVTtoD3D12(VT::GraphicStateValueType state)
	{
		switch (state)
		{
		case VT::CommonGraphicState::SHADER_RESOURCE:
			return D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
		case VT::CommonGraphicState::SHADER_RESOURCE_COMPUTE:
			return D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
		case VT::CommonGraphicState::UNORDERED_ACCESS:
			return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		case VT::CommonGraphicState::COMMON_READ:
			return D3D12_RESOURCE_STATE_GENERIC_READ;
		case VT::CommonGraphicState::COPY_SRC:
			return D3D12_RESOURCE_STATE_COPY_SOURCE;
		case VT::CommonGraphicState::COPY_DEST:
			return D3D12_RESOURCE_STATE_COPY_DEST;

		case VT::GPUBufferState::VERTEX_BUFFER:
		case VT::GPUBufferState::CONSTANT_BUFFER:
			return D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
		case VT::GPUBufferState::INDEX_BUFFER:
			return D3D12_RESOURCE_STATE_INDEX_BUFFER;
		case VT::GPUBufferState::INDIRECT_ARGUMENT:
			return D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT;

		case VT::CommonGraphicState::UNDEFINED:
		default:
			return D3D12_RESOURCE_STATE_COMMON;
		}
	}


	inline D3D12_RESOURCE_STATES convertTextureStateVTtoD3D12(VT::GraphicStateValueType state)
	{
		switch (state)
		{
		case VT::CommonGraphicState::SHADER_RESOURCE:
			return D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE | D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
		case VT::CommonGraphicState::SHADER_RESOURCE_COMPUTE:
			return D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
		case VT::CommonGraphicState::UNORDERED_ACCESS:
			return D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		case VT::CommonGraphicState::COMMON_READ:
			return D3D12_RESOURCE_STATE_GENERIC_READ;
		case VT::CommonGraphicState::COPY_SRC:
			return D3D12_RESOURCE_STATE_COPY_SOURCE;
		case VT::CommonGraphicState::COPY_DEST:
			return D3D12_RESOURCE_STATE_COPY_DEST;

		case VT::TextureState::RENDER_TARGET:
			return D3D12_RESOURCE_STATE_RENDER_TARGET;
		case VT::TextureState::DEPTH_STENCIL:
			return D3D12_RESOURCE_STATE_DEPTH_WRITE;
		case VT::TextureState::DEPTH_STENCIL_READONLY:
			return D3D12_RESOURCE_STATE_DEPTH_READ;
		case VT::TextureState::PRESENTING:
			return D3D12_RESOURCE_STATE_PRESENT;

		case VT::CommonGraphicState::UNDEFINED:
		default:
			return D3D12_RESOURCE_STATE_COMMON;
		}
	}
}
