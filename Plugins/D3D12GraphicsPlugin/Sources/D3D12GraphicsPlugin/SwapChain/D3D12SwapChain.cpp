#include "D3D12SwapChain.h"

#include "D3D12GraphicsPlugin/Device/D3D12GraphicDevice.h"

VT_D3D12::D3D12SwapChain::D3D12SwapChain(const VT::SwapChainDesc& desc, D3D12DeviceComPtr d3d12Device,
	DXGISwapChainComPtr dxgiSwapChain, D3D12Texture2D* textures, D3D12ResourceDescriptor* targetViews,
	uint32_t textureCount)
	: SwapChainBase(desc), D3D12DeviceObject(d3d12Device), m_dxgiSwapChain(dxgiSwapChain),
	m_textures(textures), m_targetViews(targetViews), m_textureCount(textureCount)
{
	m_currentTextureIndex = m_textureCount;
}

uint32_t VT_D3D12::D3D12SwapChain::getTextureCount() const
{
	return m_textureCount;
}

VT::ITexture2D* VT_D3D12::D3D12SwapChain::getTargetTexture(uint32_t index)
{
	assert(m_textures);
	return &m_textures[index];
}

VT::IGraphicResourceDescriptor* VT_D3D12::D3D12SwapChain::getTargetTextureView(uint32_t index)
{
	assert(m_targetViews);
	return &m_targetViews[index];
}

void VT_D3D12::D3D12SwapChain::updateCurrentTextureIndex()
{
	++m_currentTextureIndex;
	if (m_currentTextureIndex >= m_textureCount)
	{
		m_currentTextureIndex = 0;
	}
}

void VT_D3D12::D3D12SwapChain::present()
{
	m_dxgiSwapChain->Present(1, 0);
}
