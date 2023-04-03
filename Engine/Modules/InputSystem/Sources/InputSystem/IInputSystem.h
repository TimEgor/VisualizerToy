#pragma once

#include "Keys.h"
#include "Math/Vector.h"

namespace VT
{
	class IInputSystem
	{
	public:
		IInputSystem() = default;
		virtual ~IInputSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void update() = 0;

		virtual bool isKeyDown(Key key) const = 0;
		virtual bool isKeyUp(Key key) const = 0;

		virtual Vector2UInt16 getMousePosition() const = 0;
		virtual Vector2UInt16 getPrevMousePosition() const = 0;
		virtual Vector2Int16 getMouseMovementOffset() const = 0;
		virtual bool isMouseMoved() const = 0;
	};
}
