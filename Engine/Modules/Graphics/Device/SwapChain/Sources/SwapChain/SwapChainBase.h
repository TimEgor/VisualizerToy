#pragma once

#include "ISwapChain.h"

namespace VT
{
	class SwapChainBase : public ISwapChain
	{
	protected:
		const SwapChainDesc m_desc;

	public:
		SwapChainBase(const SwapChainDesc& desc)
			: m_desc(desc) {}

		virtual const SwapChainDesc& getDesc() const override { return m_desc; }
	};
}