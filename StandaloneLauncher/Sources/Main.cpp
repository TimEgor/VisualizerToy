#include "Core/Platform.h"
#include "Engine/Engine.h"
#include "Engine/EngineInstance.h"
#include "Engine/EngineEnvironment.h"

#include "PluginSystem/IPluginSystem.h"

#include "ObjectPool/ObjectPool.h"

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

	//tmp functional for testing object pool
	{
		using PoolHandle = VT::ObjectPoolHandle32;

		VT::ObjectPool<int, PoolHandle> pool;
		pool.init(4, 1, 4);

		PoolHandle handle0 = pool.addElement(0);
		PoolHandle handle1 = pool.addElement(1);
		PoolHandle handle2 = pool.addElement(2);
		PoolHandle handle3 = pool.addElement(3);

		PoolHandle handle4 = pool.addElement(4);
		PoolHandle handle5 = pool.addElement(5);
		PoolHandle handle6 = pool.addElement(6);
		PoolHandle handle7 = pool.addElement(7);

		pool.removeElement(handle0);
		pool.removeElement(handle1);
		pool.removeElement(handle2);
		pool.removeElement(handle3);

		PoolHandle handle8 = pool.addElement(8);
		PoolHandle handle9 = pool.addElement(9);
		PoolHandle handle10 = pool.addElement(10);
		PoolHandle handle11 = pool.addElement(11);

		pool.removeElement(handle4);
		pool.removeElement(handle5);
		pool.removeElement(handle6);
		pool.removeElement(handle7);

		PoolHandle handle12 = pool.addElement(12);
		PoolHandle handle13 = pool.addElement(13);
		PoolHandle handle14 = pool.addElement(14);
		PoolHandle handle15 = pool.addElement(15);

		pool.removeElement(PoolHandle{});
		pool.removeElement(handle0);
	}

	return VT_LAUNCHER_SUCCESS;
}