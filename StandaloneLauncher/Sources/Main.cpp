#include "Core/Platform.h"

#include "InitParams.h"
#include "Engine/Engine.h"
#include "Engine/EngineInstance.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicPresenter/WindowGraphicPresenter.h"

#include "ReturningCodes.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	VT::EngineInstance& engineInst = VT::EngineInstance::getInstance();

	std::string platformPluginPath = VT_Launcher::getPlatformPluginPath();
	std::string graphicsPluginPath = VT_Launcher::getGraphicsPluginPath();

	VT::EngineInitParam initParams;
	initParams.m_platformPluginPath = platformPluginPath.c_str();
	initParams.m_graphicDevicePluginPath = graphicsPluginPath.c_str();

	VT::IEngine* engine = new VT::Engine();
	VT::WindowGraphicPresenter* m_graphicPresenter = new VT::WindowGraphicPresenter();

	engineInst.setEngine(engine);
	if (!engineInst->init(initParams))
	{
		return VT_LAUNCHER_ENGINE_INIT_ERROR;
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

	VT_SAFE_DESTROY_WITH_RELEASING(engine);

	return VT_LAUNCHER_SUCCESS;
}