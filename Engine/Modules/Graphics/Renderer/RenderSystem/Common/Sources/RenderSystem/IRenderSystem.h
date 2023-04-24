#pragma once

namespace VT
{
	class IRender;
	class IGraphicResourceDescriptor;
	class ITexture2D;

	class IRenderSystem
	{
	public:
		IRenderSystem() = default;
		virtual ~IRenderSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void setRender(IRender* render) = 0;
		virtual void draw(ITexture2D* target, IGraphicResourceDescriptor* targetView) = 0;

		virtual void waitFrame() = 0;
	};
}
