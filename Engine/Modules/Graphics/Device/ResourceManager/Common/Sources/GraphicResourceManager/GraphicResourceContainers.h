#pragma once

#include "IGraphicResourceContainer.h"

#include "GraphicResourceHandles.h"

#include "Textures/ITexture2D.h"

namespace VT
{
	using Texture2DContainer = IGraphicResourceContainer<ITexture2D, Texture2DHandle>;
}