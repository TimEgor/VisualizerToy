#pragma once

namespace VT
{
	class IPlatform;
	class IPluginSystem;
	class IWindowSystem;

	struct EngineEnvironment final
	{
		IPlatform* m_platform = nullptr;
		IPluginSystem* m_pluginSystem = nullptr;
		IWindowSystem* m_windowSystem = nullptr;
	};
}