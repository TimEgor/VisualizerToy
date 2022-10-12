#include "Core/Platform.h"
#include "Engine/Engine.h"
#include "Engine/EngineInstance.h"

#include "ReturningCodes.h"

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	VT::EngineInstance& engineInst = VT::EngineInstance::getInstance();

	engineInst.setEngine(new VT::Engine());
	if (!engineInst->init())
	{
		return VT_LAUNCHER_ENGINE_INIT_ERROR;
	}

	return VT_LAUNCHER_SUCCESS;
}