#pragma once

#include "Core/FileName/FileName.h"
#include "GraphicResourceManager/ObjectHandles.h"

#include <unordered_map>

namespace VT
{
	using RenderPassEnvironmentResourceName = FileName;
	using RenderPassEnvironmentResourceID = FileNameID;

	enum class RenderPassEnvironmentResourceType
	{
		UNKNOWN,

		TEXTURE,
		BUFFER,

		SRV,
		RTV,
		DSV
	};

	class RenderPassEnvironment final
	{
		using TextureCollection = std::unordered_map<RenderPassEnvironmentResourceID, TextureReference>;
		using BufferCollection = std::unordered_map<RenderPassEnvironmentResourceID, GPUBufferReference>;
		using ShaderResourceViewCollection = std::unordered_map<RenderPassEnvironmentResourceID, ShaderResourceViewReference>;
		using RenderTargetViewCollection = std::unordered_map<RenderPassEnvironmentResourceID, RenderTargetViewReference>;
		using DepthStencilViewCollection = std::unordered_map<RenderPassEnvironmentResourceID, DepthStencilViewReference>;

	private:
		TextureCollection m_textures;
		BufferCollection m_buffers;
		ShaderResourceViewCollection m_SRVs;
		RenderTargetViewCollection m_RTVs;
		DepthStencilViewCollection m_DSVs;

	public:
		RenderPassEnvironment() = default;

		void addTexture(const RenderPassEnvironmentResourceName& name, TextureReference texture);
		TextureReference getTexture(const RenderPassEnvironmentResourceName& name) const;

		void addBuffer(const RenderPassEnvironmentResourceName& name, GPUBufferReference buffer);
		GPUBufferReference getBuffer(const RenderPassEnvironmentResourceName& name) const;

		void addShaderResourceView(const RenderPassEnvironmentResourceName& name, ShaderResourceViewReference srv);
		ShaderResourceViewReference getShaderResourceView(const RenderPassEnvironmentResourceName& name) const;

		void addRenderTargetView(const RenderPassEnvironmentResourceName& name, RenderTargetViewReference rtv);
		RenderTargetViewReference getRenderTargetView(const RenderPassEnvironmentResourceName& name) const;

		void addDepthStencilView(const RenderPassEnvironmentResourceName& name, DepthStencilViewReference dsv);
		DepthStencilViewReference getDepthStencilView(const RenderPassEnvironmentResourceName& name) const;
	};
}
