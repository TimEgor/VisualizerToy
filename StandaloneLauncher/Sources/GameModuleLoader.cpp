#include "GameModuleLoader.h"

#include "Core/Platform.h"
#include "Core/FileName/FileName.h"

void VT_Launcher::getGameModulePath(VT::FileName& modulePath)
{
	modulePath.clear();

	char tmpPathBuffer[512] = "\0";

	OPENFILENAME openFileName;
	ZeroMemory(&openFileName, sizeof(openFileName));
	openFileName.lStructSize = sizeof(openFileName);
	openFileName.hwndOwner = NULL;
	openFileName.lpstrFile = tmpPathBuffer;
	openFileName.nMaxFile = sizeof(tmpPathBuffer);
	openFileName.lpstrFilter = "DLL\0*.dll";
	openFileName.nFilterIndex = 1;
	openFileName.lpstrFileTitle = NULL;
	openFileName.nMaxFileTitle = 0;
	openFileName.lpstrInitialDir = NULL;
	openFileName.lpstrInitialDir = VT_ROOT_PATH;
	openFileName.lpstrTitle = "Game module path";
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	bool gettingFileNameResult = GetOpenFileName(&openFileName);
	if (gettingFileNameResult)
	{
		modulePath = tmpPathBuffer;
	}
}
