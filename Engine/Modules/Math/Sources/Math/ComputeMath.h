#pragma once

#ifdef SIMD_MATH_DX
#include <DirectXMath.h>
#endif

namespace VT
{
	namespace computeMath
	{
#ifdef SIMD_MATH_DX
		using ComputeVector = DirectX::XMVECTOR;
		using ComputeMatrix = DirectX::XMMATRIX;
		using ComputeQuaternion = DirectX::XMVECTOR;
#else
		static_assert(false, "Math SIMD realization is unspecified");
#endif
	}
}