#pragma once

#ifdef VT_SIMD_MATH_DX
#include <DirectXMath.h>
#endif

namespace VT
{
	namespace COMPUTE_MATH
	{
#ifdef VT_SIMD_MATH_DX
		using ComputeVector = DirectX::XMVECTOR;
		using ComputeMatrix = DirectX::XMMATRIX;
		using ComputeQuaternion = DirectX::XMVECTOR;
#else
		static_assert(false, "Math SIMD realization is unspecified");
#endif
	}
}