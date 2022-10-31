#pragma once

#include "IGraphicResourceContainer.h"

#include "Textures/ITexture2D.h"

namespace VT
{
	class IGraphicResourceManager
	{
	public:
		using Texture2DContainer = IGraphicResourceContainer<ITexture2D>;

	public:
		IGraphicResourceManager() = default;
		virtual ~IGraphicResourceManager() {}

		virtual bool init(Texture2DContainer* texture2DContainer) = 0;
		virtual void release() = 0;

		virtual Texture2DContainer::NewResourceInfo createTexture2D(const Texture2DDesc& desc) = 0;
		virtual void deleteTexture2D(GraphicResourceHandle handle) = 0;
		virtual ITexture2D* getTexture2D(GraphicResourceHandle handle) = 0;
		virtual bool isValidTexture2D(GraphicResourceHandle handle) const = 0;
	};
}