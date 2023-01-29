#pragma once

#include "GraphicResourceManager/ObjectHandles.h"

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
