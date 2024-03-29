#include "GameModuleLoader.h"

#include "Core/Platform.h"
#include "Core/FileName/FileName.h"

void VT_Launcher::getGamePath(VT::FileName& modulePath)
{
	modulePath.clear();

	char tmpPathBuffer[512] = "\0";

	OPENFILENAME openFileName;
	ZeroMemory(&openFileName, sizeof(openFileName));
	openFileName.lStructSize = sizeof(openFileName);
	openFileName.lpstrFile = tmpPathBuffer;
	openFileName.nMaxFile = sizeof(tmpPathBuffer);
	openFileName.lpstrFilter = "Game Module (.dll)\0*.dll\0Game Project (.vtproj)\0*.vtproj\0";
	openFileName.nFilterIndex = 1;
	openFileName.lpstrInitialDir = VT_ROOT_PATH;
	openFileName.lpstrTitle = "Game path";
	openFileName.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	bool gettingFileNameResult = GetOpenFileName(&openFileName);
	if (gettingFileNameResult)
	{
		modulePath = tmpPathBuffer;
	}
}
