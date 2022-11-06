#pragma once

#include "IGraphicResourceContainer.h"

#include "GraphicResourceHandles.h"

namespace VT
{
	class ISwapChain;
	class ITexture2D;

	using SwapChainContainer = IGraphicResourceContainer<ISwapChain, SwapChainHandle>;
	using Texture2DContainer = IGraphicResourceContainer<ITexture2D, Texture2DHandle>;
}