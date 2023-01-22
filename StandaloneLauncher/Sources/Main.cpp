#include "LevelSystem/ILevelSystem.h"

#include "Core/Platform.h"

#include "InitParams.h"
#include "Engine/Engine.h"
#include "Engine/EngineInstance.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicPresenter/WindowGraphicPresenter.h"

#include "SwapChain/ISwapChain.h"
#include "WindowSystem/IWindow.h"

#include "ReturningCodes.h"

#include "RenderSystem/IRenderSystem.h"
#include "ResourceSystem/IResourceSystem.h"

#include <vector>

#include "GraphicDevice/IGraphicDevice.h"
#include "MeshSystem/MeshSystem.h"
#include "MeshSystem/MeshComponent.h"
#include "Scene/IScene.h"
#include "Scene/Node.h"
#include "Scene/SceneNodeIDComponent.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	VT::EngineInstance& engineInst = VT::EngineInstance::getInstance();

	VT::IEngine* engine = new VT::Engine();
	VT::WindowGraphicPresenter* graphicPresenter = new VT::WindowGraphicPresenter();

	{
		VT::EngineInitParam initParams;
		VT_Launcher::getPlatformPluginPath(initParams.m_platformPluginPath);
		VT_Launcher::getGraphicsPluginPath(initParams.m_graphicDevicePluginPath);
		VT_Launcher::getResourceSystemPluginPath(initParams.m_resourceSystenPluginPath);

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

	{
		VT::IRenderSystem* renderSystem = engineInst->getEnvironment()->m_renderSystem;

		VT::ILevel* level = engineInst->getEnvironment()->m_levelSystem->createLevel();
		level->init();
		VT::VT_Entity testEntity = level->createEntity();
		level->getEntityComponentSystem()->addComponent<VT::MeshComponent>(testEntity,
			engineInst->getEnvironment()->m_meshSystem->loadMesh("Cube.mesh"));
		VT::NodeID testNodeID = level->getEntityComponentSystem()->getComponent<VT::SceneNodeIDComponent>(testEntity).getNodeID();
		VT::NodeTransforms* transforms = level->getScene()->getNodeTransforms(testNodeID);
		transforms->m_globalTransform.m_41 = 2.0f;

		renderSystem->collectRenderingData(*level);

		engine->startTimer();

		while (!engine->isStoped())
		{
			engine->updateFrame();

			renderSystem->waitFrame();

			graphicPresenter->updateNextTargetTextureIndex();
			const uint32_t frameIndex = graphicPresenter->getCurrentTargetTextureIndex();

			renderSystem->render(
				graphicPresenter->getTargetTexture(frameIndex),
				graphicPresenter->getTargetTextureView(frameIndex)
			);

			graphicPresenter->present();
		}

		engine->getEnvironment()->m_graphicDevice->waitIdle();
	}

	VT_SAFE_DESTROY(graphicPresenter);
	VT_SAFE_DESTROY(engine);

	return VT_LAUNCHER_SUCCESS;
}
