#pragma once

#include <cstdint>

namespace VT
{
	class ISemaphore;
	class ITexture2D;
	class ITexture2DView;

	class IPresenter
	{
	public:
		IPresenter() = default;
		virtual ~IPresenter() {}

		virtual void updateNextTargetTextureIndex() = 0;
		virtual uint32_t getCurrentTargetTextureIndex() const = 0;
		virtual ISemaphore* getTargetTextureAvailable() = 0;

		virtual uint32_t getFrameCount() const = 0;
		virtual ITexture2D* getTargetTexture(uint32_t frameIndex) = 0;
		virtual ITexture2DView* getTargetTextureView(uint32_t frameIndex) = 0;

		virtual void present(ISemaphore* presentReadySemaphore) = 0;
	};
}