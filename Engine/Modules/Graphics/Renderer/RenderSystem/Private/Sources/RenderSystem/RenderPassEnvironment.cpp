#include "RenderPassEnvironment.h"

void VT::RenderPassEnvironment::addTexture(const RenderPassEnvironmentResourceName& name, TextureReference texture)
{
	m_textures.insert(std::make_pair(name.hash(), texture));
}

VT::TextureReference VT::RenderPassEnvironment::getTexture(const RenderPassEnvironmentResourceName& name) const
{
	auto findIter = m_textures.find(name.hash());
	if (findIter != m_textures.end())
	{
		return findIter->second;
	}

	return nullptr;
}

void VT::RenderPassEnvironment::addBuffer(const RenderPassEnvironmentResourceName& name, GPUBufferReference buffer)
{
	m_buffers.insert(std::make_pair(name.hash(), buffer));
}

VT::GPUBufferReference VT::RenderPassEnvironment::getBuffer(const RenderPassEnvironmentResourceName& name) const
{
	auto findIter = m_buffers.find(name.hash());
	if (findIter != m_buffers.end())
	{
		return findIter->second;
	}

	return nullptr;
}

void VT::RenderPassEnvironment::addShaderResourceView(const RenderPassEnvironmentResourceName& name, ShaderResourceViewReference srv)
{
	m_SRVs.insert(std::make_pair(name.hash(), srv));
}

VT::ShaderResourceViewReference VT::RenderPassEnvironment::getShaderResourceView(const RenderPassEnvironmentResourceName& name) const
{
	auto findIter = m_SRVs.find(name.hash());
	if (findIter != m_SRVs.end())
	{
		return findIter->second;
	}

	return nullptr;
}

void VT::RenderPassEnvironment::addRenderTargetView(const RenderPassEnvironmentResourceName& name, RenderTargetViewReference rtv)
{
	m_RTVs.insert(std::make_pair(name.hash(), rtv));
}

VT::RenderTargetViewReference VT::RenderPassEnvironment::getRenderTargetView(const RenderPassEnvironmentResourceName& name) const
{
	auto findIter = m_RTVs.find(name.hash());
	if (findIter != m_RTVs.end())
	{
		return findIter->second;
	}

	return nullptr;
}

void VT::RenderPassEnvironment::addDepthStencilView(const RenderPassEnvironmentResourceName& name,DepthStencilViewReference dsv)
{
	m_DSVs.insert(std::make_pair(name.hash(), dsv));
}

VT::DepthStencilViewReference VT::RenderPassEnvironment::getDepthStencilView(const RenderPassEnvironmentResourceName& name) const
{
	auto findIter = m_DSVs.find(name.hash());
	if (findIter != m_DSVs.end())
	{
		return findIter->second;
	}

	return nullptr;
}
