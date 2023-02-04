#pragma once

#include "D3D12GraphicsPlugin/D3D12Core.h"
#include "Textures/TextureCommon.h"

namespace VT_D3D12
{
	inline D3D12_RESOURCE_FLAGS convertTextureUsageVTtoD3D12(VT::TextureUsage usage)
	{
		D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE;

		if (usage & VT::TextureUsage::TEXTURE_USAGE_RENDER_TARGET)
		{
			flags |= D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;
		}

		if (usage & VT::TextureUsage::TEXTURE_USAGE_DEPTH_STENCIL)
		{
			flags |= D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
		}

		if (!(usage & VT::TextureUsage::TEXTURE_USAGE_DENY_SHADER_RESOURCE))
		{
			flags |= D3D12_RESOURCE_FLAG_DENY_SHADER_RESOURCE;
		}

		return flags;
	}
}
