#include "Core/Platform.h"

#include "InitParams.h"
#include "Engine/Engine.h"
#include "Engine/EngineInstance.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicPresenter/WindowGraphicPresenter.h"

#include "SwapChain/ISwapChain.h"
#include "WindowSystem/IWindow.h"

#include "ReturningCodes.h"

// tmp
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "Platform/IPlatform.h"
#include "RenderSystem/IRenderSystem.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	VT::EngineInstance& engineInst = VT::EngineInstance::getInstance();

	VT::EngineInitParam initParams;
	VT_Launcher::getPlatformPluginPath(initParams.m_platformPluginPath);
	VT_Launcher::getGraphicsPluginPath(initParams.m_graphicDevicePluginPath);
	VT_Launcher::getResourceSystemPluginPath(initParams.m_resourceSystenPluginPath);

	VT_Launcher::prepareConverterPath("ShaderConverterHLSL",
		"HLSL/" + VT::FileName(VT_PLATFORM_NAME), initParams.m_shaderConverterPath);

	VT::IEngine* engine = new VT::Engine();
	VT::WindowGraphicPresenter* graphicPresenter = new VT::WindowGraphicPresenter();

	engineInst.setEngine(engine);
	if (!engineInst->init(initParams))
	{
		return VT_LAUNCHER_ENGINE_INIT_ERROR;
	}

	const VT::WindowSize defaultWindowSize(500, 500);

	VT::SwapChainDesc swapChainDesc{};
	swapChainDesc.m_format = VT::Format::B8G8R8A8_UNORM;
	swapChainDesc.m_presentMode = VT::SwapChainPresentMode::FIFO;
	swapChainDesc.m_imageCount = 2;
	if (!graphicPresenter->init("VT LAUNCHER", defaultWindowSize, swapChainDesc))
	{
		return VT_LAUNCHER_WINDOW_PRESENTER_INIT_ERROR;
	}

	{
		VT::IRenderSystem* renderSystem = engineInst->getEnvironment()->m_renderSystem;

		engine->startTimer();

		while (!engine->isStoped())
		{
			engine->updateFrame();

			renderSystem->waitFrame();

			graphicPresenter->updateNextTargetTextureIndex();
			const uint32_t frameIndex = graphicPresenter->getCurrentTargetTextureIndex();

			renderSystem->render(
				graphicPresenter->getTargetTexture(frameIndex),
				graphicPresenter->getTargetTextureView(frameIndex),
				graphicPresenter->getTargetTextureAvailable()
			);

			graphicPresenter->present(renderSystem->getRenderCompletedSemaphore());
		}
	}

	VT_SAFE_DESTROY(graphicPresenter);
	VT_SAFE_DESTROY(engine);

	return VT_LAUNCHER_SUCCESS;
}
