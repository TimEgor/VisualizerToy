#pragma once

namespace VT
{
	class IPlatform;
	class IPluginSystem;
	class IWindowSystem;

	class IGraphicPlatform;
	class IGraphicDevice;
	class IGraphicResourceManager;

	class IRenderSystem;
	class IMaterialSystem;

	class IResourceSystem;

	struct EngineEnvironment final
	{
		IPlatform* m_platform = nullptr;
		IPluginSystem* m_pluginSystem = nullptr;
		IWindowSystem* m_windowSystem = nullptr;

		IGraphicPlatform* m_graphicPlatform = nullptr;
		IGraphicDevice* m_graphicDevice = nullptr;
		IGraphicResourceManager* m_graphicResourceManager = nullptr;

		IRenderSystem* m_renderSystem = nullptr;
		IMaterialSystem* m_materialSystem = nullptr;

		IResourceSystem* m_resourceSystem = nullptr;
	};
}