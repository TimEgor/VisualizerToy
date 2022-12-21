#pragma once

#include "GraphicResourceManager/ResourceHandles.h"

namespace VT
{
	class IMaterial
	{
	public:
		IMaterial() = default;
		virtual ~IMaterial() = default;

		virtual PipelineStateConstReference getPipelineState() const = 0;
	};
}
