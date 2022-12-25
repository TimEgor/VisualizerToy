#pragma once

namespace VT
{
	class ISemaphore;
	class ITexture2D;
	class ITexture2DView;

	class IRenderSystem
	{
	public:
		IRenderSystem() = default;
		virtual ~IRenderSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void render(ITexture2D* target, ITexture2DView* targetView,
			ISemaphore* textureAvailableSemaphore) = 0;

		virtual void waitFrame() = 0;

		virtual ISemaphore* getRenderCompletedSemaphore() = 0;
	};
}
