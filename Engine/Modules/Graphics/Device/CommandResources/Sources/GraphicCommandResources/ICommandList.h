#pragma once

#include "GraphicObject/IGraphicObject.h"

namespace VT
{
	class ICommandList : public IGraphicObject
	{
	public:
		ICommandList() = default;

		virtual void close() = 0;
		virtual void reset() = 0;
	};
}