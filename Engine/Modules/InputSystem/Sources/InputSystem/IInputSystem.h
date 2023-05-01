#pragma once

#include "Core/TypeHashMacros.h"
#include "Keys.h"
#include "Math/Vector.h"

namespace VT
{
	using InputSystemType = HashTyped::Type;

	class IInputSystem : public HashTyped
	{
	public:
		IInputSystem() = default;
		virtual ~IInputSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void update() = 0;

		virtual bool isKeyDown(Key key) const = 0;
		virtual bool isKeyUp(Key key) const = 0;

		virtual const Vector2UInt16& getMousePosition() const = 0;
		virtual const Vector2UInt16& getPrevMousePosition() const = 0;
		virtual const Vector2Int16& getMouseMovementOffset() const = 0;
		virtual bool isMouseMoved() const = 0;

		virtual InputSystemType getType() const = 0;
	};
}

#define VT_INPUT_SYSTEM_TYPE(INPUT_SYSTEM_TYPE) VT_HASH_TYPE(#INPUT_SYSTEM_TYPE, VT::InputSystemType, InputSystem)
