#pragma once

#include "GraphicPresenter/IPresenter.h"

namespace VT
{
	class ITeture2D;
	class ISwapChain;
	class IWindow;

	struct SwapChainDesc;
	struct WindowSize;

	class WindowGraphicPresenter final : public IPresenter
	{
	private:
		IWindow* m_window = nullptr;
		ISwapChain* m_swapChain = nullptr;

	public:
		WindowGraphicPresenter() = default;
		~WindowGraphicPresenter() { release(); }

		bool init(const char* windowTitle, const WindowSize& windowSize, const SwapChainDesc& swapChainDesc);
		void release();

		virtual void updateNextTargetTextureIndex() override;
		virtual uint32_t getCurrentTargetTextureIndex() const override;

		virtual uint32_t getFrameCount() const override;
		virtual ITexture2D* getTargetTexture(uint32_t frameIndex) override;
		virtual IGraphicResourceDescriptor* getTargetTextureView(uint32_t frameIndex) override;

		virtual void present() override;
	};
}