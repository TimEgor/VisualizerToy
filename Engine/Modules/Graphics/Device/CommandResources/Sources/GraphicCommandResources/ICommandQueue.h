#pragma once

#include "GraphicObject/IGraphicObject.h"

namespace VT
{
	class ICommandList;

	class ICommandQueue : public IGraphicObject
	{
	public:
		ICommandQueue() = default;

		virtual void submitCommandList(ICommandList* commandList) = 0;
	};
}