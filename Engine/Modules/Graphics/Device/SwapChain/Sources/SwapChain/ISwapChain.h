#pragma once

#include "GraphicResourceCommon/Format.h"

namespace VT
{
	struct SwapChainDesc final
	{
		Format m_format = Format::UNDEFINED;
		uint32_t m_imageCount = 0;
	};

	class ITexture2D;

	class ISwapChain
	{
	public:
		ISwapChain() = default;
		virtual ~ISwapChain() {}

		virtual const SwapChainDesc& getDesc() const = 0;

		virtual uint32_t getTextureCount() const = 0;
		virtual ITexture2D* getTargetTexture(uint32_t index) = 0;
		virtual IGraphicResourceDescriptor* getTargetTextureView(uint32_t index) = 0;

		virtual void updateCurrentTextureIndex() = 0;
		virtual uint32_t getCurrentTextureIndex() const = 0;

		virtual void present() = 0;
	};
}
