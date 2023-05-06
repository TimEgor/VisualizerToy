#pragma once

#include "IRenderContext.h"

#include <cassert>

namespace VT
{
	class RenderContextEvent final
	{
	private:
		IRenderContext* m_context = nullptr;

	public:
		RenderContextEvent(IRenderContext* context, const char* name)
			: m_context(context)
		{
			assert(m_context);
			m_context->beginEvent(name);
		}

		~RenderContextEvent()
		{
			m_context->endEvent();
		}
	};
}
