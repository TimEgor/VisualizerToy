#pragma once

#include "Core/TypeHashMacros.h"

namespace VT
{
	using GraphicPlatformType = HashTyped::Type;

	class IGraphicDevice;
	class IRenderContext;

	class IGraphicPlatform : public HashTyped
	{
	public:
		IGraphicPlatform() = default;

		virtual bool init(bool isSwapChainEnabled) = 0;
		virtual void release() = 0;

		virtual IGraphicDevice* createGraphicDevice() = 0;
		virtual VT::IRenderContext* createRenderContext() = 0;

		virtual GraphicPlatformType getType() const = 0;
	};
}

#define VT_GRAPHIC_PLATFORM_TYPE(PLATFORM_TYPE) VT_HASH_TYPE(#PLATFORM_TYPE, VT::GraphicPlatformType, GraphicPlatform)