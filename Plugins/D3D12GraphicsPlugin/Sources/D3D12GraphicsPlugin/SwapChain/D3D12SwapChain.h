#pragma once

#include "D3D12GraphicsPlugin/Device/D3D12DeviceObject.h"
#include "D3D12GraphicsPlugin/Textures/D3D12Texture2D.h"
#include "D3D12GraphicsPlugin/Resource/D3D12ResourceDescriptor.h"
#include "SwapChain/SwapChainBase.h"

namespace VT_D3D12
{
	class D3D12SwapChain final : public VT::SwapChainBase, public D3D12DeviceObject
	{
	private:
		DXGISwapChainComPtr m_dxgiSwapChain = nullptr;

		D3D12Texture2D* m_textures = nullptr;
		D3D12ResourceDescriptor* m_targetViews = nullptr;
		const uint32_t m_textureCount = 0;

		uint32_t m_currentTextureIndex = 0;

	public:
		D3D12SwapChain(const VT::SwapChainDesc& desc, D3D12DeviceComPtr d3d12Device, DXGISwapChainComPtr dxgiSwapChain,
			D3D12Texture2D* textures, D3D12ResourceDescriptor* targetViews, uint32_t textureCount);

		virtual uint32_t getTextureCount() const override;
		virtual VT::ITexture2D* getTargetTexture(uint32_t index) override;
		virtual VT::IGraphicResourceDescriptor* getTargetTextureView(uint32_t index) override;

		virtual void updateCurrentTextureIndex() override;
		virtual uint32_t getCurrentTextureIndex() const override { return m_currentTextureIndex; }

		virtual void present() override;

		DXGISwapChainComPtr getDXGISwapChain() const { return m_dxgiSwapChain; }
	};
}
