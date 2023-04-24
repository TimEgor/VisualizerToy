#pragma once

namespace VT
{
	class IRenderContext;
	class ITexture2D;
	class IGraphicResourceDescriptor;

	struct RenderDrawingContext final
	{
		IRenderContext* m_context = nullptr;

		ITexture2D* m_target = nullptr;
		IGraphicResourceDescriptor* m_targetView = nullptr;
	};
}
