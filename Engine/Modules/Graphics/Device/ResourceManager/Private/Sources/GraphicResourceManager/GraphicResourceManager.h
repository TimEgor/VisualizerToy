#pragma once

#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "ObjectPool/ThreadSafeObjectPool.h"
#include "ManagedResourceHandles.h"

namespace VT
{
	class GraphicResourceManager final : public IGraphicResourceManager
	{
		friend ManagedTexture2DResourceHandle;

		using Texture2DPool = ThreadSafeObjectPool<ManagedTexture2DResourceHandle, ObjectPoolHandle32>;

	private:
		Texture2DPool m_textures2D;

		void deleteTexture2DInternal(ITexture2D* texture);

	public:
		GraphicResourceManager() = default;
		~GraphicResourceManager() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual Texture2DResourceHandleReference createTexture2D(const Texture2DDesc& desc) override;
		virtual void deleteTexture2D(Texture2DResourceHandleConstReference textureReference) override;
		virtual void deleteTexture2D(Texture2DHandleID handle) override;
		virtual Texture2DResourceHandleReference getTexture2D(Texture2DHandleID handle) override;
		virtual bool isValidTexture2D(Texture2DHandleID handle) const override;
	};
}