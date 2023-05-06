#pragma once

#ifdef SIMD_MATH_DX
#include <DirectXPackedVector.h>
#endif

namespace VT
{
#ifdef SIMD_MATH_DX
	using Float16 = DirectX::PackedVector::HALF;

	Float16 convertFloat32To16(float f32);
	float convertFloat16To132(Float16 f16);
#else
	static_assert(false, "Math SIMD realization is unspecified");
#endif
}