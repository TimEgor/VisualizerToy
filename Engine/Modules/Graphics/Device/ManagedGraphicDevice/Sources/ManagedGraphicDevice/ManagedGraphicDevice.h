#pragma once

#include "GraphicDevice/IGraphicDevice.h"

#include "ManagedObjectStorage.h"
#include "ManagedStorageInfos.h"

namespace VT::ManagedGraphicDevice
{
	class ManagedGraphicDevice : public IGraphicDevice
	{
	public:
		using SwapChainStorage = ManagedObjectStorageBase<ManagedSwapChainStorageInfoBase>;
		using Texture2DStorage = ManagedObjectStorageBase<ManagedTexture2DStorageInfoBase>;
		using VertexShaderStorage = ManagedObjectStorageBase<ManagedVertexShaderStorageInfoBase>;
		using PixelShaderStorage = ManagedObjectStorageBase<ManagedPixelShaderStorageInfoBase>;
		using CommandPoolStorage = ManagedObjectStorageBase<ManagedCommandPoolStorageInfoBase>;

	private:
		SwapChainStorage* m_swapChainStorage = nullptr;
		Texture2DStorage* m_texture2DStorage = nullptr;
		VertexShaderStorage* m_vertexShaderStorage = nullptr;
		PixelShaderStorage* m_pixelShaderStorage = nullptr;
		CommandPoolStorage* m_commandPoolStorage = nullptr;

	protected:
		virtual bool initDevice(bool isSwapChainEnabled) = 0;
		virtual void releaseDevice() = 0;

		virtual bool createSwapChain(ManagedSwapChainBase* swapChain, const SwapChainDesc& desc, const IWindow* window) = 0;
		virtual void destroySwapChain(ManagedSwapChainBase* swapChain) = 0;

		virtual void destroyTexture2D(ManagedTexture2DBase* texture) = 0;

		virtual bool createVertexShader(ManagedVertexShaderBase* shader, const void* code, size_t codeSize) = 0;
		virtual void destroyVertexShader(ManagedVertexShaderBase* shader) = 0;

		virtual bool createPixelShader(ManagedPixelShaderBase* shader, const void* code, size_t codeSize) = 0;
		virtual void destroyPixelShader(ManagedPixelShaderBase* shader) = 0;

		virtual bool createCommandPool(ManagedCommandPoolBase* commandPool) = 0;
		virtual void destroyCommandPool(ManagedCommandPoolBase* commandPool) = 0;

		virtual SwapChainStorage* createSwapChainStorage() const = 0;
		virtual Texture2DStorage* createTexture2DStorage() const = 0;
		virtual VertexShaderStorage* createVertexShaderStorage() const = 0;
		virtual PixelShaderStorage* createPixelShaderStorage() const = 0;
		virtual CommandPoolStorage* createCommandPoolStorage() const = 0;

	public:
		ManagedGraphicDevice() = default;

		virtual bool init(bool isSwapChainEnabled) override;
		virtual void release() override;

		virtual ISwapChain* createSwapChain(const SwapChainDesc& desc, const IWindow* window) override;
		virtual void destroySwapChain(ISwapChain* swapChain) override;

		virtual void destroyTexture2D(ITexture2D* texture) override;

		virtual IVertexShader* createVertexShader(const void* code, size_t codeSize) override;
		virtual void destroyVertexShader(IVertexShader* shader) override;

		virtual IPixelShader* createPixelShader(const void* code, size_t codeSize) override;
		virtual void destroyPixelShader(IPixelShader* shader) override;

		virtual ICommandPool* createCommandPool() override;
		virtual void destroyCommandPool(ICommandPool* commandPool) override;
	};
}

#define VT_GRAPHIC_DEVICE_TYPE(DEVICE_TYPE)																		\
	static inline constexpr VT::GraphicDeviceType getGraphicDeviceType() { return VT::crc32(#DEVICE_TYPE); }	\
	virtual VT::GraphicDeviceType getType() const override { return getGraphicDeviceType(); }