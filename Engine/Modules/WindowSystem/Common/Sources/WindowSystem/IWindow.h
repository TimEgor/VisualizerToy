#pragma once

#include "Math/Vector.h"

namespace VT
{
	struct WindowSize final
	{
		using ValueType = uint16_t;
		using VectorValueType = Vector2Base<ValueType>;

		union
		{
			VectorValueType m_vector;

			struct
			{
				ValueType m_width;
				ValueType m_height;
			};
		};

		WindowSize() = default;
		WindowSize(const VectorValueType& vector) : m_vector(vector) {}
		WindowSize(ValueType width, ValueType height) : m_width(width), m_height(height) {}
	};

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

		WindowArea() = default;
		WindowArea(const VectorValueType& vector) : m_vector(vector) {}
		WindowArea(ValueType top, ValueType bottom, ValueType right, ValueType left)
			: m_top(top), m_bottom(bottom), m_right(right), m_left(left) {}
	};

	class IWindow
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
	};
}