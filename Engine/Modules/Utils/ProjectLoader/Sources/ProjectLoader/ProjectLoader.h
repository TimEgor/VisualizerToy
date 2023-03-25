#pragma once

#include <vector>

#include "Core/CommonMacros.h"
#include "Core/FileName/FileName.h"

#define VT_PROJECT_FILE_EXT vtproj
#define VT_PROJECT_FILE_EXT_STR VT_TO_STRING_EXPR(VT_PROJECT_FILE_EXT)
#define VT_PROJECT_FILE_FULL_EXT_STR VT_TO_STRING_EXPR(.VT_PROJECT_FILE_EXT)

namespace VT
{
	class FileName;

	class ProjectLoader final
	{
	private:
		static bool loadProjectInternal(const FileName& projectPath);
		static bool loadModuleInternal(const FileName& modulePath);

	public:
		ProjectLoader() = default;

		static bool load(const FileName& path); // auto selection between project and module loading by path extension
		static bool loadProject(const FileName& projectPath);
		static bool loadModule(const FileName& modulePath);
	};
}
