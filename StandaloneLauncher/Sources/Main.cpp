#include "Core/Platform.h"

#include "InitParams.h"
#include "ReturningCodes.h"
#include "GameModuleLoader.h"

#include "Engine/Engine.h"
#include "Engine/EngineInstance.h"
#include "Engine/EngineEnvironment.h"

#include "WindowSystem/IWindowSystem.h"

#include "DebugUiSystem/IDebugUiSystem.h"

#include "GraphicDevice/IGraphicDevice.h"

#include "GraphicPresenter/WindowGraphicPresenter.h"
#include "WindowSystem/IWindow.h"
#include "SwapChain/ISwapChain.h"

#include "RenderSystem/IRenderSystem.h"
#include "ResourceSystem/IResourceSystem.h"

#include "MeshSystem/MeshSystem.h"

#include "ProjectLoader/ProjectLoader.h"
#include "LevelSystem/ILevelSystem.h"

#include "ArgParser/Parser.h"

#include <vector>


int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	VT::EngineInstance& engineInst = VT::EngineInstance::getInstance();
	VT::IEngine* engine = new VT::Engine();

	{
		VT::FileName gameProjectPath;

		{
			VT::FileName gameModulePath;

			VT::ArgParser mainArgParser;
			mainArgParser.addOptionValue<VT::FileName>("--module", gameModulePath);
			mainArgParser.addOptionValue<VT::FileName>("--project", gameProjectPath);
			mainArgParser.parse(__argc, __argv);

			if (gameProjectPath && gameModulePath)
			{
				return VT_LAUNCHER_GAME_ARGS_ERROR;
			}

			if (gameProjectPath.empty())
			{
				gameProjectPath = gameModulePath;
			}
		}

		engineInst.setEngine(engine);

		{
			VT::EngineInitParam initParams;
			initParams.m_platformPluginPath = VT_Launcher::getPlatformPluginPath();
			initParams.m_graphicDevicePluginPath = VT_Launcher::getGraphicsPluginPath();
			initParams.m_resourceSystenPluginPath = VT_Launcher::getResourceSystemPluginPath();

			initParams.m_resourceSystemPath = VT_Launcher::getResourceSystemPath();

			std::vector<VT::FileName> converters;

			converters.emplace_back(VT_Launcher::prepareConverterPath("ShaderConverterHLSL",
				"HLSL/" + VT::FileName(VT_PLATFORM_NAME)));
			converters.emplace_back(VT_Launcher::prepareConverterPath("ModelConverterFBX", ""));

			initParams.m_converterPaths = converters.data();
			initParams.m_converterPathsCount = static_cast<uint32_t>(converters.size());

			if (!engineInst->init(initParams))
			{
				return VT_LAUNCHER_ENGINE_INIT_ERROR;
			}

		}

		{
			if (gameProjectPath.empty())
			{
				VT_Launcher::getGamePath(gameProjectPath);
			}

			if (gameProjectPath.empty() || !VT::ProjectLoader::load(gameProjectPath))
			{
				return VT_LAUNCHER_GAME_INIT_ERROR;
			}
		}
	}

	VT::EngineEnvironment* engineEnvironment = engine->getEnvironment();
	VT::IWindow* window = nullptr;

	{
		const VT::WindowSize windowSize(500, 500);
		window = engineEnvironment->m_windowSystem->createWindow("VT LAUNCHER", windowSize);

		if (!window)
		{
			return VT_LAUNCHER_WINDOW_CREATION_ERROR;
		}

		if (!engineEnvironment->m_debugUiSystem->setWindow(window))
		{
			return VT_LAUNCHER_IMGUI_WINDOW_BACKEND_INIT_ERROR;
		}
	}

	VT::WindowGraphicPresenter* graphicPresenter = new VT::WindowGraphicPresenter();

	{
		VT::SwapChainDesc swapChainDesc{};
		swapChainDesc.m_format = VT::Format::R8G8B8A8_UNORM;
		swapChainDesc.m_imageCount = 2;
		if (!graphicPresenter->init(window, swapChainDesc))
		{
			return VT_LAUNCHER_WINDOW_PRESENTER_INIT_ERROR;
		}
	}

	{
		VT::IRenderSystem* renderSystem = engineEnvironment->m_renderSystem;
		VT::ILevelSystem* levelSystem = engineEnvironment->m_levelSystem;
		VT::IGraphicDevice* graphicDevice = engineEnvironment->m_graphicDevice;
		VT::IDebugUiSystem* debugUi = engineEnvironment->m_debugUiSystem;

		graphicDevice->submitContexts();

		engine->startTimer();

		while (!engine->isStoped())
		{
			engine->beginFrame();

			graphicDevice->waitContexts();
			graphicDevice->resetContexts();

			debugUi->update();

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
			graphicDevice->submitContexts();

			engine->endFrame();
		}

		engine->getEnvironment()->m_graphicDevice->waitIdle();
	}

	VT_SAFE_DESTROY(graphicPresenter);

	engineEnvironment->m_windowSystem->destroyWindow(window);

	VT_SAFE_DESTROY(engine);

	return VT_LAUNCHER_SUCCESS;
}
