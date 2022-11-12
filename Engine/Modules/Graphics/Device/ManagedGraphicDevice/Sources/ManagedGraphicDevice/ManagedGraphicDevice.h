#pragma once

#include "GraphicDevice/IGraphicDevice.h"
#include "ManagedObjectStorage.h"

namespace VT::ManagedGraphicDevice
{
	class ManagedGraphicDevice : public IGraphicDevice
	{
	public:
		using SwapChainStorage = ManagedObjectStorageBase<ManagedSwapChainStorageInfoBase>;
		using Texture2DStorage = ManagedObjectStorageBase<ManagedTexture2DStorageInfoBase>;

	private:
		SwapChainStorage* m_swapChainStorage = nullptr;
		Texture2DStorage* m_texture2DStorage = nullptr;

	protected:
		virtual bool initDevice(bool isSwapChainEnabled) = 0;
		virtual void releaseDevice() = 0;

		virtual bool createSwapChain(ManagedSwapChainBase* swapChain, const SwapChainDesc& desc, const IWindow* window) = 0;
		virtual void destroySwapChain(ManagedSwapChainBase* swapChain) = 0;

		virtual void destroyTexture2D(ManagedTexture2DBase* texture) = 0;

		virtual SwapChainStorage* createSwapChainStorage() const = 0;
		virtual Texture2DStorage* createTexture2DStorage() const = 0;

	public:
		ManagedGraphicDevice() = default;
		virtual ~ManagedGraphicDevice() { release(); }

		virtual bool init(bool isSwapChainEnabled) override;
		virtual void release() override;

		virtual ISwapChain* createSwapChain(const SwapChainDesc& desc, const IWindow* window) override;
		virtual void destroySwapChain(ISwapChain* swapChain) override;

		virtual void destroyTexture2D(ITexture2D* texture) override;
	};
}

#define VT_GRAPHIC_DEVICE_TYPE(DEVICE_TYPE)																		\
	static inline constexpr VT::GraphicDeviceType getGraphicDeviceType() { return VT::crc32(#DEVICE_TYPE); }	\
	virtual VT::GraphicDeviceType getType() const override { return getGraphicDeviceType(); }