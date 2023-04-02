#pragma once

#include <cstdint>

namespace VT
{

	template <typename Type>
	struct Vector2Base final
	{
		static constexpr Type InitialVal = Type();

		union
		{
			Type m_vals[2];

			struct
			{
				Type m_x, m_y;
			};
		};

		constexpr Vector2Base() = default;
		constexpr Vector2Base(Type x, Type y) : m_x(x), m_y(y) {}
		constexpr Vector2Base(const Type* vals) { memcpy_s(m_vals, sizeof(m_vals), vals, sizeof(Type) * 2); }
		constexpr Vector2Base(const Vector2Base<Type>& vec) = default;

		inline Type& operator[](size_t i) { return getElement(i); }
		inline const Type& operator[](size_t i) const { return getElement(i); }

		inline Type& getElement(size_t i) { return m_vals[i]; }
		inline const Type& getElement(size_t i) const { return m_vals[i]; }

		Vector2Base& operator=(const Vector2Base<Type>& vec) = default;
	};

	template <typename Type>
	struct Vector3Base final
	{
		static constexpr Type InitialVal = Type();

		union
		{
			Type m_vals[3];

			Vector2Base<Type> m_xy;

			struct
			{
				Type m_x, m_y, m_z;
			};
		};

		constexpr Vector3Base() = default;
		constexpr Vector3Base(Type x, Type y, Type z) : m_x(x), m_y(y), m_z(z) {}
		constexpr Vector3Base(const Type* vals) { memcpy_s(m_vals, sizeof(m_vals), vals, sizeof(Type) * 3); }
		constexpr Vector3Base(const Vector3Base<Type>& vec) = default;
		constexpr Vector3Base(const Vector2Base<Type>& vec) : m_x(vec.m_x), m_y(vec.m_y), m_z(InitialVal) {}

		inline Type& operator[](size_t i) { return getElement(i); }
		inline const Type& operator[](size_t i) const { return getElement(i); }

		inline Type& getElement(size_t i) { return m_vals[i]; }
		inline const Type& getElement(size_t i) const { return m_vals[i]; }

		Vector3Base& operator=(const Vector2Base<Type>& vec)
		{
			m_x = vec.m_x;
			m_y = vec.m_y;
			m_z = InitialVal;

			return *this;
		}
		Vector3Base& operator=(const Vector3Base<Type>& vec) = default;
	};

	template <typename Type>
	struct Vector4Base final
	{
		static constexpr Type InitialVal = Type();

		union
		{
			Type m_vals[4];

			struct
			{
				Vector2Base<Type> m_xy;
				Vector2Base<Type> m_zw;
			};

			Vector3Base<Type> m_xyz;

			struct
			{
				Type m_x, m_y, m_z, m_w;
			};
		};

		constexpr Vector4Base() = default;
		constexpr Vector4Base(Type x, Type y, Type z, Type w) : m_x(x), m_y(y), m_z(z), m_w(w) {}
		constexpr Vector4Base(const Type* vals) { memcpy_s(m_vals, sizeof(m_vals), vals, sizeof(Type) * 4); }
		constexpr Vector4Base(const Vector4Base<Type>& vec) = default;
		constexpr Vector4Base(const Vector3Base<Type>& vec) : m_x(vec.m_x), m_y(vec.m_y), m_z(vec.z), m_w(InitialVal) {}
		constexpr Vector4Base(const Vector2Base<Type>& vec) : m_x(vec.m_x), m_y(vec.m_y), m_z(InitialVal), m_w(InitialVal) {}

		inline Type& operator[](size_t i) { return getElement(i); }
		inline const Type& operator[](size_t i) const { return getElement(i); }

		inline Type& getElement(size_t i) { return m_vals[i]; }
		inline const Type& getElement(size_t i) const { return m_vals[i]; }

		Vector4Base& operator=(const Vector2Base<Type>& vec)
		{
			m_x = vec.m_x;
			m_y = vec.m_y;
			m_z = InitialVal;
			m_w = InitialVal;

			return *this;
		}
		Vector4Base& operator=(const Vector3Base<Type>& vec)
		{
			m_x = vec.m_x;
			m_y = vec.m_y;
			m_z = vec.m_z;
			m_w = InitialVal;

			return *this;
		}
		Vector4Base& operator=(const Vector4Base<Type>& vec) = default;
	};

	using Vector2 = Vector2Base<float>;
	using Vector3 = Vector3Base<float>;
	using Vector4 = Vector4Base<float>;

	constexpr Vector3 Vector3Zero = Vector3(0.0f, 0.0f, 0.0f);
	constexpr Vector3 Vector3Identity = Vector3(1.0f, 1.0f, 1.0f);
	constexpr Vector3 Vector3NegativeIdentity = Vector3(-1.0f, -1.0f, -1.0f);
	constexpr Vector3 Vector3UnitX = Vector3(1.0f, 0.0f, 0.0f);
	constexpr Vector3 Vector3UnitY = Vector3(0.0f, 1.0f, 0.0f);
	constexpr Vector3 Vector3UnitZ = Vector3(0.0f, 0.0f, 1.0f);
	constexpr Vector3 Vector3NegativeUnitX = Vector3(-1.0f, 0.0f, 0.0f);
	constexpr Vector3 Vector3NegativeUnitY = Vector3(0.0f, -1.0f, 0.0f);
	constexpr Vector3 Vector3NegativeUnitZ = Vector3(0.0f, 0.0f, -1.0f);

	using Vector2UInt16 = Vector2Base<uint16_t>;
	using Vector3UInt16 = Vector3Base<uint16_t>;
	using Vector4UInt16 = Vector4Base<uint16_t>;

	constexpr Vector2UInt16 Vector2UInt16Zero = Vector2UInt16(0, 0);
	constexpr Vector3UInt16 Vector3UInt16Zero = Vector3UInt16(0, 0, 0);
	constexpr Vector4UInt16 Vector4UInt16Zero = Vector4UInt16(0, 0, 0, 0);
}