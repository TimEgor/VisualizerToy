#pragma once

#include "GraphicResourceManager/IGraphicResourceManager.h"

#include <vector>

namespace VT
{
	struct RemovingHandleLists final
	{
		std::vector<SwapChainHandle> m_swapChain;
		std::vector<Texture2DHandle> m_texture2D;
	};

	class GraphicResourceManager final : public IGraphicResourceManager
	{
	private:
		ResourceManagerContainerCollection m_containers;
		RemovingHandleLists m_removingLists;

		static bool isContainerCollectionEmpty(const ResourceManagerContainerCollection& collection);

	public:
		GraphicResourceManager() = default;
		~GraphicResourceManager() { release(); }

		virtual bool init(const ResourceManagerContainerCollection& containers, bool isSwapChainEnabled) override;
		virtual void release() override;

		virtual void updateRemovingLists() override;

		virtual SwapChainContainer::NewResourceInfo createSwapChain(const SwapChainDesc& desc, const IWindow* window) override;
		virtual void deleteSwapChain(SwapChainHandle handle) override;
		virtual ISwapChain* getSwapChain(SwapChainHandle handle) override;
		virtual bool isValidSwapChain(SwapChainHandle handle) const override;

		virtual Texture2DContainer::NewResourceInfo createTexture2D(const Texture2DDesc& desc) override;
		virtual void deleteTexture2D(Texture2DHandle handle) override;
		virtual ITexture2D* getTexture2D(Texture2DHandle handle) override;
		virtual bool isValidTexture2D(Texture2DHandle handle) const override;
	};
}