#pragma once

#include "IGameModule.h"

namespace VT
{
	class FileName;

	class IGameSystem
	{
	public:
		IGameSystem() = default;
		virtual ~IGameSystem() = default;

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void update(float deltaTime) = 0;

		virtual IGameModule* loadGameModule(const FileName& modulePath) = 0;
		virtual void unloadGameModule(const FileName& modulePath) = 0;
		virtual void unloadGameModule(GameModuleID moduleID) = 0;

		virtual IGameModule* getGameModule(const FileName& modulePath) = 0;
		virtual IGameModule* getGameModule(GameModuleID moduleID) = 0;
	};
}
