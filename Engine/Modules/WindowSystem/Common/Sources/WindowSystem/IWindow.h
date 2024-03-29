#pragma once

#include "Core/HashedType.h"
#include "Math/Vector.h"

#include "EventSystem/IEventSystem.h"

namespace VT
{
	using WindowSize = Vector2UInt16;
	constexpr WindowSize DefaultWindowSize = Vector2UInt16Zero;

	struct WindowArea final
	{
		using ValueType = uint16_t;
		using VectorValueType = Vector4Base<ValueType>;

		union
		{
			VectorValueType m_vector;

			struct
			{
				ValueType m_top;
				ValueType m_bottom;
				ValueType m_right;
				ValueType m_left;
			};
		};

		constexpr WindowArea() = default;
		constexpr WindowArea(const VectorValueType& vector) : m_vector(vector) {}
		constexpr WindowArea(ValueType top, ValueType bottom, ValueType right, ValueType left)
			: m_top(top), m_bottom(bottom), m_right(right), m_left(left) {}
	};

	constexpr WindowArea DefaultWindowArea = WindowArea{ 0, 0, 0, 0 };

	class IWindow;

	struct WindowEventBase : public IInstancedEvent
	{
		IWindow* m_window = nullptr;

		WindowEventBase(IWindow* window) : m_window(window) {}
	};

	struct WindowSizeEvent final : public WindowEventBase
	{
		WindowSizeEvent(IWindow* window)
			: WindowEventBase(window) {}

		VT_INSTANCED_EVENT_TYPE(WINDOW_SIZE_EVENT_TYPE);
	};

	using WindowType = HashedType::Type;

	class IWindow : public HashedType
	{
	public:
		IWindow() = default;
		virtual ~IWindow() {}

		virtual bool init(const char* title, const WindowSize& size) = 0;
		virtual void release() = 0;

		virtual void show() = 0;
		virtual void hide() = 0;
		virtual bool isShown() const = 0;

		virtual const WindowSize& getWindowSize() const = 0;
		virtual const WindowSize& getClientAreaSize() const = 0;
		virtual const WindowArea& getCutoutsArea() const = 0;

		virtual InstancedEventID getWindowSizeEventID() const = 0;

		virtual void* getNativeHandle() const = 0;

		virtual WindowType getType() const = 0;
	};
}

#define VT_WINDOW_TYPE(WINDOW_TYPE) VT_HASH_TYPE(#WINDOW_TYPE, VT::WindowType, Window)