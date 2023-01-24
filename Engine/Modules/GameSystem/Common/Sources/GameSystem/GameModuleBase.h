#pragma once

#include "IGameModule.h"
#include "PluginSystem/PluginBase.h"

namespace VT
{
	class GameModuleBase : public PluginBase, public IGameModule
	{
	protected:
		virtual PluginID getID() const override { return PluginBase::getID(); }

	public:
		GameModuleBase(PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: PluginBase(id, nativeHandle) {}

		virtual GameModuleID getModuleID() const { return getID(); }
	};
}
