#pragma once

namespace VT
{
	class IGraphicResourceDescriptor;
	class ITexture2D;

	class IRenderSystem
	{
	public:
		IRenderSystem() = default;
		virtual ~IRenderSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void render(ITexture2D* target, IGraphicResourceDescriptor* targetView) = 0;

		virtual void waitFrame() = 0;
	};
}
