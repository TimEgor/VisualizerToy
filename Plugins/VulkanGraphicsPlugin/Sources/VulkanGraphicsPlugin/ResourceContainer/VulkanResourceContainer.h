#pragma once

#include "GraphicResourceManager/GraphicResourceContainers.h"
#include "ObjectPool/ObjectPool.h"

#include "VulkanGraphicsPlugin/SwapChain/VulkanSwapChain.h"
#include "VulkanGraphicsPlugin/Textures/VulkanTexture2D.h"

#include <type_traits>

namespace VT_VK
{
	template <typename VulkanResourceType, typename BaseContainerType, typename HandleType,
		size_t PageSize = 4096, size_t MaxFreePageCount = 2, size_t MinFreeIndexCount = 64>
	class VulkanGraphicResourceContainer final : public BaseContainerType
	{
		using ResourceContainer = VT::ObjectPool<VulkanResourceType, HandleType>;

		using NewGraphicResourceInfo = typename BaseContainerType::NewResourceInfo;
		using GraphicResourceHandle = typename BaseContainerType::GraphicResourceHandle;
		using GraphicResource = typename BaseContainerType::GraphicResourceType;

	private:
		ResourceContainer m_pool;

	public:
		VulkanGraphicResourceContainer()
			: BaseContainerType() {}
		~VulkanGraphicResourceContainer() {}

		virtual bool init() override
		{
			static_assert(std::is_base_of<BaseContainerType::GraphicResourceType, VulkanResourceType>::value);
			return m_pool.init(PageSize, MaxFreePageCount, MinFreeIndexCount);
		}
		virtual void release() override
		{
			m_pool.release();
		}

		virtual void addResource(NewGraphicResourceInfo& info) override
		{
			ResourceContainer::NewElementInfo poolElementInfo;
			m_pool.addElementRaw(poolElementInfo);

			info.m_resourceHandle = poolElementInfo.m_elementHandle.getKey();
			info.m_resourcePtr = reinterpret_cast<VulkanResourceType*>(poolElementInfo.m_elementPtr);
		}
		virtual void removeResource(GraphicResourceHandle handle) override
		{
			m_pool.removeElement(handle);
		}

		virtual GraphicResource* getResource(GraphicResourceHandle handle, bool checkDisabling = true) override
		{
			return m_pool.getElement(handle, checkDisabling);
		}

		virtual void disableResource(GraphicResourceHandle handle)
		{
			return m_pool.setElementEnabledState(handle, false);
		}

		virtual bool isValidResourceHandle(GraphicResourceHandle handle) const override
		{
			return m_pool.isValid(handle);
		}
	};

	using VulkanSwapChainRescourceContainer = VulkanGraphicResourceContainer<VulkanSwapChain, VT::SwapChainContainer, VT::ObjectPoolHandle32, 2, 0, 2>;
	using VulkanTexture2DResourceContainer = VulkanGraphicResourceContainer<VulkanTexture2D, VT::Texture2DContainer, VT::ObjectPoolHandle32, 512, 0, 128>;
}