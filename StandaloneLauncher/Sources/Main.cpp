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
#include "ResourceSystem/IResourceSystem.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	VT::EngineInstance& engineInst = VT::EngineInstance::getInstance();

	VT::EngineInitParam initParams;
	VT_Launcher::getPlatformPluginPath(initParams.m_platformPluginPath);
	VT_Launcher::getGraphicsPluginPath(initParams.m_graphicDevicePluginPath);
	VT_Launcher::getResourceSystemPluginPath(initParams.m_resourceSystenPluginPath);

	VT::IEngine* engine = new VT::Engine();
	VT::WindowGraphicPresenter* m_graphicPresenter = new VT::WindowGraphicPresenter();

	engineInst.setEngine(engine);
	if (!engineInst->init(initParams))
	{
		return VT_LAUNCHER_ENGINE_INIT_ERROR;
	}

	int counter = 0;
	{

		auto resourceLoadedCallback = [&counter]()
		{
			++counter;
		};

		auto dependencyLink = engineInst->getEnvironment()->m_resourceSystem->createResourceDependencyState(resourceLoadedCallback);

		engineInst->getEnvironment()->m_resourceSystem->getResourceAsync("Test.txt", [dependencyLink](VT::ResourceDataReference) {});
		engineInst->getEnvironment()->m_resourceSystem->getResourceAsync("Test1.txt", [dependencyLink](VT::ResourceDataReference) {});
	}

	const VT::WindowSize defaultWindowSize(500, 500);

	VT::SwapChainDesc swapChainDesc{};
	swapChainDesc.m_format = VT::Format::R8G8B8A8_UNORM;
	swapChainDesc.m_presentMode = VT::SwapChainPresentMode::FIFO;
	swapChainDesc.m_imageCount = 2;
	if (!m_graphicPresenter->init("VT LAUNCHER", defaultWindowSize, swapChainDesc))
	{
		return VT_LAUNCHER_WINDOW_PRESENTER_INIT_ERROR;
	}

	engine->startTimer();

	while (!engine->isStoped())
	{
		engine->updateFrame();
	}

	VT_SAFE_DESTROY(m_graphicPresenter);
	VT_SAFE_DESTROY(engine);

	return VT_LAUNCHER_SUCCESS;
}