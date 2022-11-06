#pragma once

#include "IGraphicResourceContainer.h"

#include "GraphicResourceContainers.h"

namespace VT
{
	struct SwapChainDesc;
	struct Texture2DDesc;

	struct ResourceManagerContainerCollection final
	{
		SwapChainContainer* m_swapChainContainer = nullptr;
		Texture2DContainer* m_texture2DContainer = nullptr;
	};

	class IWindow;

	class IGraphicResourceManager
	{
	public:
		IGraphicResourceManager() = default;
		virtual ~IGraphicResourceManager() {}

		virtual bool init(const ResourceManagerContainerCollection& containers, bool isSwapChainEnabled) = 0;
		virtual void release() = 0;

		virtual void updateRemovingLists() = 0;

		virtual SwapChainContainer::NewResourceInfo createSwapChain(const SwapChainDesc& desc, const IWindow* window) = 0;
		virtual void deleteSwapChain(SwapChainHandle handle) = 0;
		virtual ISwapChain* getSwapChain(SwapChainHandle handle) = 0;
		virtual bool isValidSwapChain(SwapChainHandle handle) const = 0;

		virtual Texture2DContainer::NewResourceInfo createTexture2D(const Texture2DDesc& desc) = 0;
		virtual void deleteTexture2D(Texture2DHandle handle) = 0;
		virtual ITexture2D* getTexture2D(Texture2DHandle handle) = 0;
		virtual bool isValidTexture2D(Texture2DHandle handle) const = 0;
	};
}