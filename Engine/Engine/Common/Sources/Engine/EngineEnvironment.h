#pragma once

namespace VT
{
	class IPlatform;
	class IPluginSystem;
	class IWindowSystem;

	class IGraphicPlatform;
	class IGraphicDevice;
	class IGraphicResourceManager;

	struct EngineEnvironment final
	{
		IPlatform* m_platform = nullptr;
		IPluginSystem* m_pluginSystem = nullptr;
		IWindowSystem* m_windowSystem = nullptr;

		IGraphicPlatform* m_graphicPlatform = nullptr;
		IGraphicDevice* m_graphicDevice = nullptr;
		IGraphicResourceManager* m_graphicResourceManager = nullptr;
	};
}