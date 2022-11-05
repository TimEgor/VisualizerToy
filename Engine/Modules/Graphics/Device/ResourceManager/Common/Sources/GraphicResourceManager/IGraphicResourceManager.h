#pragma once

#include "IGraphicResourceContainer.h"

#include "GraphicResourceContainers.h"

namespace VT
{
	class IGraphicResourceManager
	{
	public:
		IGraphicResourceManager() = default;
		virtual ~IGraphicResourceManager() {}

		virtual bool init(Texture2DContainer* texture2DContainer) = 0;
		virtual void release() = 0;

		virtual Texture2DContainer::NewResourceInfo createTexture2D(const Texture2DDesc& desc) = 0;
		virtual void deleteTexture2D(Texture2DHandle handle) = 0;
		virtual ITexture2D* getTexture2D(Texture2DHandle handle) = 0;
		virtual bool isValidTexture2D(Texture2DHandle handle) const = 0;
	};
}