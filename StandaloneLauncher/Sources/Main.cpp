#include "Core/Platform.h"
#include "Engine/Engine.h"
#include "Engine/EngineInstance.h"
#include "Engine/EngineEnvironment.h"
#include "PluginSystem/IPluginSystem.h"

#include "ReturningCodes.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	VT::EngineInstance& engineInst = VT::EngineInstance::getInstance();

	engineInst.setEngine(new VT::Engine());
	if (!engineInst->init())
	{
		return VT_LAUNCHER_ENGINE_INIT_ERROR;
	}

	//tmp functional for testing plugin system
	{
		VT::IPluginSystem* pluginSystem = engineInst->getEnvironment()->m_pluginSystem;
		pluginSystem->loadPlugin("D:/VisualizerToy/Plugins/Out/TestPlugin_Debug_Win32.dll");
	}

	return VT_LAUNCHER_SUCCESS;
}