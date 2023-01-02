#pragma once

#include "VulkanGraphicsPlugin/VulkanCore.h"
#include "ManagedGraphicDevice/ManagedGraphicDevice.h"
#include "VulkanDestroyingResourceCollection.h"

#define VT_GRAPHIC_DEVICE_VULKAN_TYPE

namespace VT_VK
{
	class VulkanShaderBase;
	struct VulkanSwapChainInfo;

	class VulkanSwapChain;

	class VulkanGraphicDevice final : public VT::ManagedGraphicDevice::ManagedGraphicDevice
	{
	private:
		VulkanDestroyingResourceCollection m_destroyingResources;

		VkDevice m_vkDevice = nullptr;
		VkPhysicalDevice m_vkPhysDevice = nullptr;

		VkPhysicalDeviceMemoryProperties m_physMemoryProps;

		VkQueue m_vkGraphicQueue = nullptr;
		VkQueue m_vkTransferQueue = nullptr;
		VkQueue m_vkComputeQueue = nullptr;

		VulkanQueueFamilyIndex m_graphicQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		VulkanQueueFamilyIndex m_transferQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		VulkanQueueFamilyIndex m_computeQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;

		bool initVkDevice(VkInstance vkInstance, bool isSwapChainEnabled);

		bool chooseVkPhysDevice(VkInstance vkInstance, const VulkanNameContainer& extensions);
		bool checkVkPhysDevice(VkPhysicalDevice device, const VulkanNameContainer& extensions);

		void findQueueFamiliesIndices();

		uint32_t chooseMemoryType(uint32_t filterBits, VkMemoryPropertyFlags properties);

		void getSwapChainCapabilitiesInfo(VkSurfaceKHR surface, VulkanSwapChainInfo& info);
		void createSwapChainInternal(const VT::SwapChainDesc& swapCahinDesc, const VT::IWindow* window,
			VkSurfaceKHR& surface, VkSwapchainKHR& swapChain, VT::Texture2DDesc& imageDesc);
		void initSwapChainImages(VulkanSwapChain* swapChain, const VT::Texture2DDesc& imageDesc);

		void createShaderInternal(const void* code, size_t codeSize, VkShaderModule& vkShaderModule);
		void destroyShaderInternal(VulkanShaderBase* shader);

		void createSemaphoreInternal(VkSemaphore& semaphore);

		void destroyResources();

	protected:
		virtual bool initDevice(bool isSwapChainEnabled) override;
		virtual void releaseDevice() override;

		//SwapChain
		virtual bool createSwapChain(VT::ManagedGraphicDevice::ManagedSwapChainBase* swapChain, const VT::SwapChainDesc& desc, const VT::IWindow* window) override;
		virtual void destroySwapChain(VT::ManagedGraphicDevice::ManagedSwapChainBase* swapChain) override;

		//Buffer
		virtual bool createBuffer(VT::ManagedGraphicDevice::ManagedGPUBufferBase* buffer, const VT::GPUBufferDesc& desc) override;
		virtual void destroyBuffer(VT::ManagedGraphicDevice::ManagedGPUBufferBase* buffer) override;

		//Textures
		virtual void destroyTexture2D(VT::ManagedGraphicDevice::ManagedTexture2DBase* texture) override;

		virtual bool createTexture2DView(VT::ManagedGraphicDevice::ManagedTexture2DViewBase* view, VT::ITexture2D* texture, const VT::TextureViewDesc& desc) override;
		virtual void destroyTexture2DView(VT::ManagedGraphicDevice::ManagedTexture2DViewBase* view) override;

		//Shaders
		virtual bool createVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader, const void* code, size_t codeSize) override;
		virtual void destroyVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader) override;

		virtual bool createPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader, const void* code, size_t codeSize) override;
		virtual void destroyPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader) override;

		//Pipeline
		virtual bool createPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state,
			const VT::PipelineStateInfo& info, const VT::InputLayoutDesc* inputLayoutDesc) override;
		virtual void destroyPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state) override;

		//Commands
		virtual bool createCommandPool(VT::ManagedGraphicDevice::ManagedCommandPoolBase* commandPool) override;
		virtual void destroyCommandPool(VT::ManagedGraphicDevice::ManagedCommandPoolBase* commandPool) override;

		virtual bool allocateCommandList(VT::ManagedGraphicDevice::ManagedCommandListBase* commandList, VT::ICommandPool* pool) override;

		virtual void submitCommandList(VT::ICommandList* list, const VT::CommandListSubmitInfo& info) override;

		//Sync
		virtual bool createFence(VT::ManagedGraphicDevice::ManagedFenceBase* fence, bool signaled) override;
		virtual void destroyFence(VT::ManagedGraphicDevice::ManagedFenceBase* fence) override;

		virtual void waitFences(uint32_t count, VT::IFence* fences) override;
		virtual void resetFences(uint32_t count, VT::IFence* fences) override;

		virtual bool createSemaphore(VT::ManagedGraphicDevice::ManagedSemaphoreBase* semaphore) override;
		virtual void destroySemaphore(VT::ManagedGraphicDevice::ManagedSemaphoreBase* semaphore) override;

		virtual SwapChainStorage* createSwapChainStorage() const override;

		virtual BufferStorage* createBufferStorage() const override;

		virtual Texture2DStorage* createTexture2DStorage() const override;
		virtual Texture2DViewStorage* createTexture2DViewStorage() const override;

		virtual VertexShaderStorage* createVertexShaderStorage() const override;
		virtual PixelShaderStorage* createPixelShaderStorage() const override;

		virtual PipelineStateStorage* createPipelineStateStorage() const override;

		virtual CommandPoolStorage* createCommandPoolStorage() const override;
		virtual CommandListStorage* createCommandListStorage() const override;

		virtual FenceStorage* createFenceStorage() const override;
		virtual SemaphoreStorage* createSemaphoreStorage() const override;

	public:
		VulkanGraphicDevice() = default;

		virtual void update() override;

		virtual void wait() override;

		VT_GRAPHIC_DEVICE_TYPE(VT_GRAPHIC_DEVICE_VULKAN_TYPE)
	};
}
