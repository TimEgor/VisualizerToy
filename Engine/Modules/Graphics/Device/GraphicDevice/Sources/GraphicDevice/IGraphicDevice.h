#pragma once

#include "Core/HashFunctions/CRC32.h"

namespace VT
{
	class IWindow;
	class ISwapChain;
	class ITexture2D;
	class IVertexShader;
	class IPixelShader;
	class ICommandPool;

	struct SwapChainDesc;

	using GraphicDeviceType = uint32_t;

	class IGraphicDevice
	{
	public:
		IGraphicDevice() = default;
		virtual ~IGraphicDevice() {}

		virtual bool init(bool isSwapChainEnabled) = 0;
		virtual void release() = 0;

		virtual void update() = 0;

		virtual void wait() = 0;

		virtual ISwapChain* createSwapChain(const SwapChainDesc& desc, const IWindow* window) = 0;
		virtual void destroySwapChain(ISwapChain* swapChain) = 0;

		virtual void destroyTexture2D(ITexture2D* texture) = 0;

		virtual IVertexShader* createVertexShader(const void* code, size_t codeSize) = 0;
		virtual void destroyVertexShader(IVertexShader* shader) = 0;

		virtual IPixelShader* createPixelShader(const void* code, size_t codeSize) = 0;
		virtual void destroyPixelShader(IPixelShader* shader) = 0;

		virtual ICommandPool* createCommandPool() = 0;
		virtual void destroyCommandPool(ICommandPool* commandPool) = 0;

		virtual GraphicDeviceType getType() const = 0;
	};
}

#define VT_GRAPHIC_DEVICE_TYPE(DEVICE_TYPE)																		\
	static inline constexpr VT::GraphicDeviceType getGraphicDeviceType() { return VT::crc32(#DEVICE_TYPE); }	\
	virtual VT::GraphicDeviceType getType() const override { return getGraphicDeviceType(); }