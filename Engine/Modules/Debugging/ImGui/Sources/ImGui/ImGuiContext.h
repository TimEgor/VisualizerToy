#pragma once

#include "IBackend.h"

namespace VT
{
	class ITexture2D;
	class IGraphicResourceDescriptor;

	class ImGuiContext final
	{
	private:
		IImGuiPlatformBackend* m_platformBackend = nullptr;
		IImGuiRenderBackend* m_renderBackend = nullptr;

	public:
		ImGuiContext() = default;
		~ImGuiContext() { release(); }

		bool init(IImGuiPlatformBackend* platformBackend, IImGuiRenderBackend* renderBackend);
		void release();

		void update();
		void render(ITexture2D* target, IGraphicResourceDescriptor* targetView);
	};
}
