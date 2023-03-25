#pragma once

#include "Core/Platform.h"

#include <cstdint>

#define VT_GAME_MODULE_FILE_EXT VT_DYNAMIC_LIB_EXT
#define VT_GAME_MODULE_FILE_EXT_STR VT_TO_STRING_EXPR(VT_GAME_MODULE_FILE_EXT)
#define VT_GAME_MODULE_FILE_FULL_EXT_STR VT_TO_STRING_EXPR(.VT_GAME_MODULE_FILE_EXT)

namespace VT
{
	using GameModuleID = uint32_t;

	class IGameModule
	{
	public:
		IGameModule() = default;
		virtual ~IGameModule() = default;

		virtual void update(float deltaTime) = 0;

		virtual GameModuleID getModuleID() const = 0;
	};
}
