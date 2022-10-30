#pragma once

namespace VT
{
	class ISwapChain
	{
	public:
		ISwapChain() = default;
		virtual ~ISwapChain() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void present() = 0;
	};
}