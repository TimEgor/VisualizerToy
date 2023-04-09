#pragma once

#include "IImGuiBackend.h"

namespace VT
{
	class ITexture2D;
	class IGraphicResourceDescriptor;

	class IImGuiContext
	{
	public:
		IImGuiContext() = default;
		virtual ~IImGuiContext() {}

		virtual bool init(IWindow* window) = 0;
		virtual void release() = 0;

		virtual void update() = 0;

		virtual void render(IRenderContext* context) = 0;
	};
}
