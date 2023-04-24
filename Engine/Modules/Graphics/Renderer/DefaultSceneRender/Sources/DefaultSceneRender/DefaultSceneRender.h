#pragma once

#include "DefaultRender/DefaultRender.h"

namespace VT
{
	class ILevel;

	class DefaultSceneRender : public DefaultRender
	{
	public:
		DefaultSceneRender() = default;

		virtual void prepareRenderData(const ILevel& level);
	};
}
