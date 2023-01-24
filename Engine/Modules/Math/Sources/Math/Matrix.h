#pragma once

#include <cstdint>

namespace VT
{

	template <typename Type>
	struct Matrix22Base final
	{
		static constexpr Type InitialVal = Type();

		union
		{
			Type m_vals[2][2];

			struct
			{
				Type m_11, m_12,
					m_21, m_22;
			};
		};

		Matrix22Base() = default;
		Matrix22Base(
			Type v11, Type v12,
			Type v21, Type v22)
			: m_11(v11), m_12(v12),
			m_21(v21), m_22(v22)
		{}
		Matrix22Base(const Matrix22Base& matr) = default;

		inline Type& getElement(size_t n, size_t m) { return m_vals[n][m]; }
		inline const Type& getElement(size_t n, size_t m) const { return m_vals[n][m]; }

		Matrix22Base& operator=(const Matrix22Base& matr) = default;
	};

	template <typename Type>
	struct Matrix33Base final
	{
		static constexpr Type InitialVal = Type();

		union
		{
			Type m_vals[3][3];

			struct
			{
				Type m_11, m_12, m_13,
					m_21, m_22, m_23,
					m_31, m_32, m_33;
			};
		};

		Matrix33Base() = default;
		Matrix33Base(
			Type v11, Type v12, Type v13,
			Type v21, Type v22, Type v23,
			Type v31, Type v32, Type v33)
			: m_11(v11), m_12(v12), m_13(v13),
			m_21(v21), m_22(v22), m_23(v23),
			m_31(v31), m_32(v32), m_33(v33)
		{}
		Matrix33Base(const Matrix22Base<Type>& matr)
			: m_11(matr.m_11), m_12(matr.m_12), m_13(InitialVal),
			m_21(matr.m_21), m_22(matr.m_22), m_23(InitialVal),
			m_31(InitialVal), m_32(InitialVal), m_33(InitialVal)
		{}
		Matrix33Base(const Matrix33Base<Type>& matr) = default;

		inline Type& getElement(size_t n, size_t m) { return m_vals[n][m]; }
		inline const Type& getElement(size_t n, size_t m) const { return m_vals[n][m]; }

		Matrix33Base& operator=(const Matrix22Base<Type>& matr)
		{
			m_11 = matr.m_11;
			m_12 = matr.m_12;
			m_13 = InitialVal;

			m_21 = matr.m_21;
			m_22 = matr.m_22;
			m_23 = InitialVal;

			m_31 = InitialVal;
			m_32 = InitialVal;
			m_33 = InitialVal;
		}
		Matrix33Base& operator=(const Matrix33Base<Type>& matr) = default;
	};

	template <typename Type>
	struct Matrix44Base final
	{
		static constexpr Type InitialVal = Type();

		union
		{
			Type m_vals[4][4];

			struct
			{
				Type m_11, m_12, m_13, m_14,
					m_21, m_22, m_23, m_24,
					m_31, m_32, m_33, m_34,
					m_41, m_42, m_43, m_44;
			};
		};

		Matrix44Base() = default;
		Matrix44Base(
			Type v11, Type v12, Type v13, Type v14,
			Type v21, Type v22, Type v23, Type v24,
			Type v31, Type v32, Type v33, Type v34,
			Type v41, Type v42, Type v43, Type v44)
			: m_11(v11), m_12(v12), m_13(v13), m_14(v14),
			m_21(v21), m_22(v22), m_23(v23), m_24(v24),
			m_31(v31), m_32(v32), m_33(v33), m_34(v34),
			m_41(v41), m_42(v42), m_43(v43), m_44(v44)
		{}
		Matrix44Base(const Matrix22Base<Type>& matr)
			: m_11(matr.m_11), m_12(matr.m_12), m_13(InitialVal), m_14(InitialVal),
			m_21(matr.m_21), m_22(matr.m_22), m_23(InitialVal), m_24(InitialVal),
			m_31(InitialVal), m_32(InitialVal), m_33(InitialVal), m_34(InitialVal),
			m_41(InitialVal), m_42(InitialVal), m_43(InitialVal), m_44(InitialVal)
		{}
		Matrix44Base(const Matrix33Base<Type>& matr)
			: m_11(matr.m_11), m_12(matr.m_12), m_13(matr.m_13), m_14(InitialVal),
			m_21(matr.m_21), m_22(matr.m_22), m_23(matr.m_23), m_24(InitialVal),
			m_31(matr.m_31), m_32(matr.m_32), m_33(matr.m_33), m_34(InitialVal),
			m_41(InitialVal), m_42(InitialVal), m_43(InitialVal), m_44(InitialVal)
		{}
		Matrix44Base(const Matrix44Base<Type>& matr) = default;

		inline Type& getElement(size_t n, size_t m) { return m_vals[n][m]; }
		inline const Type& getElement(size_t n, size_t m) const { return m_vals[n][m]; }

		Matrix44Base& operator=(const Matrix22Base<Type>& matr)
		{
			m_11 = matr.m_11;
			m_12 = matr.m_12;
			m_13 = InitialVal;
			m_14 = InitialVal;

			m_21 = matr.m_21;
			m_22 = matr.m_22;
			m_23 = InitialVal;
			m_24 = InitialVal;

			m_31 = InitialVal;
			m_32 = InitialVal;
			m_33 = InitialVal;
			m_34 = InitialVal;

			m_41 = InitialVal;
			m_42 = InitialVal;
			m_43 = InitialVal;
			m_44 = InitialVal;
		}
		Matrix44Base& operator=(const Matrix33Base<Type>& matr)
		{
			m_11 = matr.m_11;
			m_12 = matr.m_12;
			m_13 = matr.m_13;
			m_14 = InitialVal;

			m_21 = matr.m_21;
			m_22 = matr.m_22;
			m_23 = matr.m_23;
			m_24 = InitialVal;

			m_31 = matr.m_31;
			m_32 = matr.m_32;
			m_33 = matr.m_33;
			m_34 = InitialVal;

			m_41 = InitialVal;
			m_42 = InitialVal;
			m_43 = InitialVal;
			m_44 = InitialVal;
		}
		Matrix44Base& operator=(const Matrix44Base<Type>& matr) = default;
	};

	using Matrix22 = Matrix22Base<float>;
	using Matrix33 = Matrix33Base<float>;
	using Matrix44 = Matrix44Base<float>;

	const Matrix44 Matrix44Identity = Matrix44(1.0f, 0.0f, 0.0f, 0.0f,
												0.0f, 1.0f, 0.0f, 0.0f,
												0.0f, 0.0f, 1.0f, 0.0f,
												0.0f, 0.0f, 0.0f, 1.0f);

}