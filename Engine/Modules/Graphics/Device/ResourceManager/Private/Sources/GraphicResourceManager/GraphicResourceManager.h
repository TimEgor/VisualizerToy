#pragma once

#include "GraphicResourceManager/IGraphicResourceManager.h"

namespace VT
{
	class GraphicResourceManager final : public IGraphicResourceManager
	{
	private:
		Texture2DContainer* m_texture2DContainer = nullptr;

	public:
		GraphicResourceManager() = default;
		~GraphicResourceManager() { release(); }

		virtual bool init(Texture2DContainer* texture2DContainer) override;
		virtual void release() override;

		virtual Texture2DContainer::NewResourceInfo createTexture2D(const Texture2DDesc& desc) override;
		virtual void deleteTexture2D(Texture2DHandle handle) override;
		virtual ITexture2D* getTexture2D(Texture2DHandle handle) override;
		virtual bool isValidTexture2D(Texture2DHandle handle) const override;
	};
}