#pragma once

#ifdef SIMD_MATH_DX
#include "Math/Types.h"

namespace VT
{
	Float16 convertFloat32To16(float f32)
	{
		return DirectX::PackedVector::XMConvertFloatToHalf(f32);
	}

	float convertFloat16To132(Float16 f16)
	{
		return DirectX::PackedVector::XMConvertHalfToFloat(f16);
	}
}
#endif // SIMD_MATH_DX