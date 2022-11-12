#pragma once

#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "ObjectPool/ObjectPool.h"

namespace VT
{
	class GraphicResourceManager final : public IGraphicResourceManager
	{
		using Texture2DPool = ObjectPool<ITexture2D*, ObjectPoolHandle32>;

	private:
		Texture2DPool m_textures2D;

	public:
		GraphicResourceManager() = default;
		~GraphicResourceManager() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual Texture2DResourceInfo createTexture2D(const Texture2DDesc& desc) override;
		virtual void deleteTexture2D(Texture2DHandle handle) override;
		virtual ITexture2D* getTexture2D(Texture2DHandle handle) override;
		virtual bool isValidTexture2D(Texture2DHandle handle) const override;
	};
}