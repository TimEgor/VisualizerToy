#pragma once

#include "Textures/ITexture2D.h"

namespace VT
{
	class IPresenter
	{
	public:
		IPresenter() = default;
		virtual ~IPresenter() {}

		virtual uint32_t getFrameCount() const = 0;
		virtual ITexture2D* getTargetTexture(uint32_t frameIndex) = 0;
	};
}