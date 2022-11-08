#pragma once

#include "ObjectPool/ObjectPool.h"
#include "GraphicREsourceManager/GraphicResourceContainers.h"

namespace VT
{
	template <typename ResourceType, typename BaseContainerType, typename HandleType,
		size_t PageSize = 256, size_t MaxFreePageCount = 2, size_t MinFreeIndexCount = 64>
	class ObjectPoolGraphicResourceContainer final : public BaseContainerType
	{
		using ResourceContainer = VT::ObjectPool<ResourceType, HandleType>;

		using NewGraphicResourceInfo = typename BaseContainerType::NewResourceInfo;
		using GraphicResourceHandle = typename BaseContainerType::GraphicResourceHandle;
		using GraphicResource = typename BaseContainerType::GraphicResourceType;

	private:
		ResourceContainer m_pool;

	public:
		ObjectPoolGraphicResourceContainer()
			: BaseContainerType()
		{}
		~ObjectPoolGraphicResourceContainer() {}

		virtual bool init() override
		{
			static_assert(std::is_base_of<BaseContainerType::GraphicResourceType, ResourceType>::value);
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
			info.m_resourcePtr = reinterpret_cast<ResourceType*>(poolElementInfo.m_elementPtr);
		}
		virtual void removeResource(GraphicResourceHandle handle) override
		{
			m_pool.removeElement(handle);
		}

		virtual GraphicResource* getResource(GraphicResourceHandle handle) override
		{
			return m_pool.getElement(handle);
		}

		virtual bool isValidResourceHandle(GraphicResourceHandle handle) const override
		{
			return m_pool.isValid(handle);
		}
	};

	template <typename SwapChainType>
	using SwapChainObjectPoolContainer = ObjectPoolGraphicResourceContainer<SwapChainType, SwapChainContainer, ObjectPoolHandle32, 2, 0, 1>;
	template <typename Texture2DType>
	using Texture2DObjectPoolContainer = ObjectPoolGraphicResourceContainer<Texture2DType, Texture2DContainer, ObjectPoolHandle32, 128, 0, 32>;
}