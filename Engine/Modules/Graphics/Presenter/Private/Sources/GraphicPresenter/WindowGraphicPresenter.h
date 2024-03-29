#pragma once

#include "GraphicPresenter/IPresenter.h"
#include "EventSystem/IEventSystem.h"
#include "WindowSystem/IWindow.h"

namespace VT
{
	class IWindow;
	class ITeture2D;
	class ISwapChain;

	struct SwapChainDesc;

	class WindowGraphicPresenter final : public IPresenter
	{
	private:
		ISwapChain* m_swapChain = nullptr;
		InstancedEventDispatcherID m_windowSizeEventDispatcher = InvalidInstancedEventDispatcherID;

		void onWindowSizeEvent(const WindowSizeEvent& event);

	public:
		WindowGraphicPresenter() = default;
		~WindowGraphicPresenter() { release(); }

		bool init(const IWindow* window, const SwapChainDesc& swapChainDesc);
		void release();

		virtual void updateNextTargetTextureIndex() override;
		virtual uint32_t getCurrentTargetTextureIndex() const override;

		virtual uint32_t getFrameCount() const override;
		virtual ITexture2D* getTargetTexture(uint32_t frameIndex) override;
		virtual IGraphicResourceDescriptor* getTargetTextureView(uint32_t frameIndex) override;

		virtual void present() override;
	};
}
