#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "Multithreading/Mutex.h"
#include "Multithreading/LockGuard.h"

#include <vector>

namespace VT_VK
{
	template <typename VkResourceType>
	class VulkanDestroyingResourceContainer final
	{
		using ContainerType = std::vector<VkResourceType>;
		using LockerType = VT::Mutex;

	private:
		ContainerType m_container;
		VT::Mutex m_mutex;

	public:
		VulkanDestroyingResourceContainer() = default;

		void addToContainer(VkResourceType val)
		{
			VT::LockGuard locker(m_mutex);
			m_container.push_back(val);
		}

		ContainerType& getContainer() { return m_container; }
		LockerType& getLocker() { return m_mutex; }
	};

	struct VulkanDestroyingResourceCollection final
	{
		VulkanDestroyingResourceContainer<VkSwapchainKHR> m_swapChains;
		VulkanDestroyingResourceContainer<VkSurfaceKHR> m_surfaces;
		VulkanDestroyingResourceContainer<VkImage> m_images;
		VulkanDestroyingResourceContainer<VkShaderModule> m_shaderModules;
	};
}