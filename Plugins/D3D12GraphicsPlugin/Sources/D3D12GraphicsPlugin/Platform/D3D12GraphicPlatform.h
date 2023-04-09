#pragma once

#include "GraphicPlatform/IGraphicPlatform.h"

#define VT_GRAPHIC_PLARFORM_D3D12_TYPE

namespace VT_D3D12
{
	class D3D12GraphicPlatform final : public VT::IGraphicPlatform
	{
	public:
		D3D12GraphicPlatform() = default;
		~D3D12GraphicPlatform() { release(); }

		virtual bool init(bool isSwapChainEnabled) override;
		virtual void release() override;

		virtual VT::IGraphicDevice* createGraphicDevice() override;
		virtual VT::IRenderContext* createRenderContext() override;

		virtual VT::IImGuiRenderBackend* createImGuiRenderBackend() const override;

		VT_GRAPHIC_PLATFORM_TYPE(VT_GRAPHIC_PLARFORM_D3D12_TYPE)
	};
}
