#pragma once

#include "Core/TypeHashMacros.h"

namespace VT
{
	class IGraphicDevice;
	class IRenderContext;

	class IImGuiRenderBackend;

	using GraphicPlatformType = HashTyped::Type;

	class IGraphicPlatform : public HashTyped
	{
	public:
		IGraphicPlatform() = default;

		virtual bool init(bool isSwapChainEnabled) = 0;
		virtual void release() = 0;

		virtual IGraphicDevice* createGraphicDevice() = 0;
		virtual IRenderContext* createRenderContext() = 0;

		virtual IImGuiRenderBackend* createImGuiRenderBackend() const = 0;

		virtual GraphicPlatformType getType() const = 0;
	};
}

#define VT_GRAPHIC_PLATFORM_TYPE(PLATFORM_TYPE) VT_HASH_TYPE(#PLATFORM_TYPE, VT::GraphicPlatformType, GraphicPlatform)