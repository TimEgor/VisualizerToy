#pragma once

namespace VT
{
	class IPluginSystem;

	struct EngineEnvironment final
	{
		IPluginSystem* m_pluginSystem = nullptr;
	};
}