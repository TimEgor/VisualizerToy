#pragma once

#include "GraphicResourceCommon/Format.h"

namespace VT
{

	enum class SwapChainPresentMode
	{
		IMMIDIATE,
		FIFO,
		FIFO_RELAX,
		MAILBOX
	};

	struct SwapChainDesc final
	{
		Format m_format = Format::UNDEFINED;
		SwapChainPresentMode m_presentMode = SwapChainPresentMode::IMMIDIATE;
		uint32_t m_imageCount = 0;
	};

	class ITexture2D;
	class ITexture2DView;

	class ISwapChain
	{
	public:
		ISwapChain() = default;
		virtual ~ISwapChain() {}

		virtual const SwapChainDesc& getDesc() const = 0;

		virtual uint32_t getTextureCount() const = 0;
		virtual const ITexture2D* getTexture(uint32_t index) const = 0;
		virtual const ITexture2DView* getTargetView(uint32_t index) const = 0;
	};
}
