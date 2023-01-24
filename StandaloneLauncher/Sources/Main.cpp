#include "Core/Platform.h"

#include "InitParams.h"
#include "ReturningCodes.h"
#include "GameModuleLoader.h"

#include "Engine/Engine.h"
#include "Engine/EngineInstance.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"

#include "GraphicPresenter/WindowGraphicPresenter.h"
#include "WindowSystem/IWindow.h"
#include "SwapChain/ISwapChain.h"

#include "RenderSystem/IRenderSystem.h"
#include "ResourceSystem/IResourceSystem.h"

#include "MeshSystem/MeshSystem.h"

#include "GameSystem/IGameSystem.h"
#include "LevelSystem/ILevelSystem.h"

#include "ArgParser/Parser.h"

#include <vector>


int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	VT::FileName gameModulePath;

	VT::ArgParser mainArgParser;
	mainArgParser.addOptionValue<VT::FileName>("--module", gameModulePath);
	mainArgParser.parse(__argc, __argv);

	VT::EngineInstance& engineInst = VT::EngineInstance::getInstance();

	VT::IEngine* engine = new VT::Engine();
	VT::WindowGraphicPresenter* graphicPresenter = new VT::WindowGraphicPresenter();

	{
		VT::EngineInitParam initParams;
		VT_Launcher::getPlatformPluginPath(initParams.m_platformPluginPath);
		VT_Launcher::getGraphicsPluginPath(initParams.m_graphicDevicePluginPath);
		VT_Launcher::getResourceSystemPluginPath(initParams.m_resourceSystenPluginPath);

		VT_Launcher::getResourceSystemPath(initParams.m_resourceSystemPath);

		std::vector<VT::FileName> converters;

		VT::FileName& shaderConverter = converters.emplace_back();
		VT_Launcher::prepareConverterPath("ShaderConverterHLSL",
			"HLSL/" + VT::FileName(VT_PLATFORM_NAME), shaderConverter);

		VT::FileName& modelConverter = converters.emplace_back();
		VT_Launcher::prepareConverterPath("ModelConverterFBX",
			"", modelConverter);

		initParams.m_converterPaths = converters.data();
		initParams.m_converterPathsCount = static_cast<uint32_t>(converters.size());

		engineInst.setEngine(engine);
		if (!engineInst->init(initParams))
		{
			return VT_LAUNCHER_ENGINE_INIT_ERROR;
		}
	}

	const VT::WindowSize defaultWindowSize(500, 500);

	VT::SwapChainDesc swapChainDesc{};
	swapChainDesc.m_format = VT::Format::R8G8B8A8_UNORM;
	swapChainDesc.m_imageCount = 2;
	if (!graphicPresenter->init("VT LAUNCHER", defaultWindowSize, swapChainDesc))
	{
		return VT_LAUNCHER_WINDOW_PRESENTER_INIT_ERROR;
	}

	if (gameModulePath.empty())
	{
		VT_Launcher::getGameModulePath(gameModulePath);
	}

	VT::EngineEnvironment* engineEnvironment = engineInst->getEnvironment();

	if (gameModulePath.empty() || !engineEnvironment->m_gameSystem->loadGameModule(gameModulePath))
	{
		return VT_LAUNCHER_INVALID_GAME_MODULE;
	}

	{
		VT::IRenderSystem* renderSystem = engineEnvironment->m_renderSystem;
		VT::ILevelSystem* levelSystem = engineEnvironment->m_levelSystem;

		engine->startTimer();

		while (!engine->isStoped())
		{
			engine->beginFrame();

			engine->updateFrame();

			renderSystem->collectRenderingData(*levelSystem->getCurrentLevel());
			renderSystem->waitFrame();

			graphicPresenter->updateNextTargetTextureIndex();
			const uint32_t frameIndex = graphicPresenter->getCurrentTargetTextureIndex();

			renderSystem->render(
				graphicPresenter->getTargetTexture(frameIndex),
				graphicPresenter->getTargetTextureView(frameIndex)
			);

			graphicPresenter->present();

			engine->endFrame();
		}

		engine->getEnvironment()->m_graphicDevice->waitIdle();
	}

	VT_SAFE_DESTROY(graphicPresenter);
	VT_SAFE_DESTROY(engine);

	return VT_LAUNCHER_SUCCESS;
}
