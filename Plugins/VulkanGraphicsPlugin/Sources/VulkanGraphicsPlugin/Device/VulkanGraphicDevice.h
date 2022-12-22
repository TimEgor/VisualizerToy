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

		void getSwapChainCapabilitiesInfo(VkSurfaceKHR surface, VulkanSwapChainInfo& info);
		void createSwapChainInternal(const VT::SwapChainDesc& swapCahinDesc, const VT::IWindow* window,
			VkSurfaceKHR& surface, VkSwapchainKHR& swapChain, VT::Texture2DDesc& imageDesc);
		void initSwapChainImages(VulkanSwapChain* swapChain, const VT::Texture2DDesc& imageDesc);

		void createShaderInternal(const void* code, size_t codeSize, VkShaderModule& vkShaderModule);
		void destroyShaderInternal(VulkanShaderBase* shader);

		void destroyResources();

	protected:
		virtual bool initDevice(bool isSwapChainEnabled) override;
		virtual void releaseDevice() override;

		virtual bool createSwapChain(VT::ManagedGraphicDevice::ManagedSwapChainBase* swapChain, const VT::SwapChainDesc& desc, const VT::IWindow* window) override;
		virtual void destroySwapChain(VT::ManagedGraphicDevice::ManagedSwapChainBase* swapChain) override;

		virtual void destroyTexture2D(VT::ManagedGraphicDevice::ManagedTexture2DBase* texture) override;

		virtual bool createTexture2DView(VT::ManagedGraphicDevice::ManagedTexture2DViewBase* view, VT::ITexture2D* texture, const VT::TextureViewDesc& desc) override;
		virtual void destroyTexture2DView(VT::ManagedGraphicDevice::ManagedTexture2DViewBase* view) override;

		virtual bool createVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader, const void* code, size_t codeSize) override;
		virtual void destroyVertexShader(VT::ManagedGraphicDevice::ManagedVertexShaderBase* shader) override;

		virtual bool createPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader, const void* code, size_t codeSize) override;
		virtual void destroyPixelShader(VT::ManagedGraphicDevice::ManagedPixelShaderBase* shader) override;

		virtual bool createPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state,
			const VT::PipelineStateInfo& info, const VT::IRenderPass* renderPass) override;
		virtual void destroyPipelineState(VT::ManagedGraphicDevice::ManagedPipelineStateBase* state) override;

		virtual bool createRenderPass(VT::ManagedGraphicDevice::ManagedRenderPassBase* pass, const VT::RenderPassInfo& info) override;
		virtual void destroyRenderPass(VT::ManagedGraphicDevice::ManagedRenderPassBase* pass) override;

		virtual bool createCommandPool(VT::ManagedGraphicDevice::ManagedCommandPoolBase* commandPool) override;
		virtual void destroyCommandPool(VT::ManagedGraphicDevice::ManagedCommandPoolBase* commandPool) override;

		virtual SwapChainStorage* createSwapChainStorage() const override;

		virtual Texture2DStorage* createTexture2DStorage() const override;
		virtual Texture2DViewStorage* createTexture2DViewStorage() const override;

		virtual VertexShaderStorage* createVertexShaderStorage() const override;
		virtual PixelShaderStorage* createPixelShaderStorage() const override;

		virtual PipelineStateStorage* createPipelineStateStorage() const override;
		virtual RenderPassStorage* createRenderPassStorage() const override;

		virtual CommandPoolStorage* createCommandPoolStorage() const override;

	public:
		VulkanGraphicDevice() = default;

		virtual void update() override;

		virtual void wait() override;

		VT_GRAPHIC_DEVICE_TYPE(VT_GRAPHIC_DEVICE_VULKAN_TYPE)
	};
}
