#pragma once

#include "ResourceHandles.h"

namespace VT
{
	class IWindow;

	struct SwapChainDesc;
	struct Texture2DDesc;

	class IGraphicResourceManager
	{
	public:
		IGraphicResourceManager() = default;
		virtual ~IGraphicResourceManager() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual Texture2DResourceInfo createTexture2D(const Texture2DDesc& desc) = 0;
		virtual void deleteTexture2D(Texture2DHandle handle) = 0;
		virtual ITexture2D* getTexture2D(Texture2DHandle handle) = 0;
		virtual bool isValidTexture2D(Texture2DHandle handle) const = 0;
	};
}