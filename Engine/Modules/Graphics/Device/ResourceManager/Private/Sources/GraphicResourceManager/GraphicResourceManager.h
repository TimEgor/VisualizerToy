#pragma once

#include "GraphicResourceManager/IGraphicResourceManager.h"

namespace VT
{
	class GraphicResourceManager final : public IGraphicResourceManager
	{
	private:
		ResourceManagerContainerCollection m_containers;

		static bool isContainerCollectionEmpty(const ResourceManagerContainerCollection& collection);
		static bool isContainerCollectionFull(const ResourceManagerContainerCollection& collection);

	public:
		GraphicResourceManager() = default;
		~GraphicResourceManager() { release(); }

		virtual bool init(const ResourceManagerContainerCollection& containers, bool isSwapChainEnabled) override;
		virtual void release() override;

		virtual SwapChainContainer::NewResourceInfo createSwapChain(const SwapChainDesc& desc, const IWindow* window) override;
		virtual void deleteSwapChain(SwapChainHandle handle) override;
		virtual ITexture2D* getSwapChain(SwapChainHandle handle) override;
		virtual bool isValidSwapChain(SwapChainHandle handle) const override;

		virtual Texture2DContainer::NewResourceInfo createTexture2D(const Texture2DDesc& desc) override;
		virtual void deleteTexture2D(Texture2DHandle handle) override;
		virtual ITexture2D* getTexture2D(Texture2DHandle handle) override;
		virtual bool isValidTexture2D(Texture2DHandle handle) const override;
	};
}