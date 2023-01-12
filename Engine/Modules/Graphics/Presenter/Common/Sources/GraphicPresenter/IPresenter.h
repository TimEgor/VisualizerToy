#pragma once

#include <cstdint>

namespace VT
{
	class ITexture2D;
	class IGraphicResourceDescriptor;

	class IPresenter
	{
	public:
		IPresenter() = default;
		virtual ~IPresenter() {}

		virtual void updateNextTargetTextureIndex() = 0;
		virtual uint32_t getCurrentTargetTextureIndex() const = 0;

		virtual uint32_t getFrameCount() const = 0;
		virtual ITexture2D* getTargetTexture(uint32_t frameIndex) = 0;
		virtual IGraphicResourceDescriptor* getTargetTextureView(uint32_t frameIndex) = 0;

		virtual void present() = 0;
	};
}