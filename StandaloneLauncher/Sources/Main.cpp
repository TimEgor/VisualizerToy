#include "Core/Platform.h"
#include "Engine/Engine.h"
#include "Engine/EngineInstance.h"
#include "Engine/EngineEnvironment.h"

#include "ReturningCodes.h"
#include "InitParams.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	VT::EngineInstance& engineInst = VT::EngineInstance::getInstance();

	std::string platformPluginPath = VT_Launcher::getPlatformPluginPath();
	std::string graphicsPluginPath = VT_Launcher::getGraphicsPluginPath();

	VT::EngineInitParam initParams;
	initParams.m_platformPluginPath = platformPluginPath.c_str();
	initParams.m_graphicDevicePluginPath = graphicsPluginPath.c_str();

	VT::IEngine* engine = new VT::Engine();

	engineInst.setEngine(engine);
	if (!engineInst->init(initParams))
	{
		return VT_LAUNCHER_ENGINE_INIT_ERROR;
	}

	engineInst->run();

	VT_SAFE_DESTROY_WITH_RELEASING(engine);

	return VT_LAUNCHER_SUCCESS;
}