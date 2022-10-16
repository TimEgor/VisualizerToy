#pragma once

#ifdef SIMD_MATH_GLM
#include "Math/ComputeVector.h"

#include "glm/gtc/reciprocal.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/compatibility.hpp"

namespace VT
{
	namespace computeMath
	{
		//
		// Loading
		//

#pragma region Loading
		ComputeVector loadComputeVectorFromVector2(const Vector2& vec)
		{
			return glm::vec4(vec.m_x, vec.m_y, 0.0f, 0.0f);
		}

		ComputeVector loadComputeVectorFromVector3(const Vector3& vec)
		{
			return glm::vec4(vec.m_x, vec.m_y, vec.m_z, 0.0f);
		}

		ComputeVector loadComputeVectorFromVector4(const Vector4& vec)
		{
			return glm::vec4(vec.m_x, vec.m_y, vec.m_z, vec.m_w);
		}

		ComputeVector loadComputeVectorFromVector4(float x, float y, float z, float w)
		{
			return glm::vec4(x, y, z, w);
		}
#pragma endregion

		//
		// Saving
		//

#pragma region Saving
		Vector2 saveComputeVectorToVector2(const ComputeVector& vec)
		{
			return vec.data.m128_f32;
		}

		Vector3 saveComputeVectorToVector3(const ComputeVector& vec)
		{
			return vec.data.m128_f32;
		}

		Vector4 saveComputeVectorToVector4(const ComputeVector& vec)
		{
			return vec.data.m128_f32;
		}
#pragma endregion

		//
		// Arithmetic
		//

#pragma region Arithmetic
		ComputeVector vectorAbs(const ComputeVector& vec)
		{
			return glm::abs(vec);
		}

		ComputeVector vectorAdd(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return vec1 + vec2;
		}

		ComputeVector vectorAddAngles(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorCeiling(const ComputeVector& vec)
		{
			return glm::ceil(vec);
		}

		ComputeVector vectorClamp(const ComputeVector& vec, const ComputeVector& minVec, const ComputeVector& maxVec)
		{
			return glm::clamp(vec, minVec, maxVec);
		}

		ComputeVector vectorDivide(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return vec1 / vec2;
		}

		ComputeVector vectorFloor(const ComputeVector& vec)
		{
			return glm::floor(vec);
		}

		ComputeVector vectorIsInfinite(const ComputeVector& vec)
		{
			return glm::isinf(vec);
		}

		ComputeVector vectorIsNaN(const ComputeVector& vec)
		{
			return glm::isnan(vec);
		}

		ComputeVector vectorMax(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::max(vec1, vec2);
		}

		ComputeVector vectorMin(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::min(vec1, vec2);
		}

		ComputeVector vectorMod(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::mod(vec1, vec2);
		}

		ComputeVector vectorModAngles(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorMultiply(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return vec1 * vec2;
		}

		ComputeVector vectorMultiplyAdd(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& vec3)
		{
			return vec1 * vec2 + vec3;
		}

		ComputeVector vectorNegate(const ComputeVector& vec)
		{
			return -vec;
		}

		ComputeVector vectorNegativeMultiplySubtract(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& vec3)
		{
			return vec3 - vec1 * vec2;
		}

		ComputeVector vectorPow(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::pow(vec1, vec2);
		}

		ComputeVector vectorReciprocal(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorReciprocalEst(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorReciprocalSqrt(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorReciprocalSqrtEst(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorRound(const ComputeVector& vec)
		{
			return glm::round(vec);
		}

		ComputeVector vectorSaturate(const ComputeVector& vec)
		{
			return glm::saturate(vec);
		}

		ComputeVector vectorScale(const ComputeVector& vec, float factor)
		{
			return vec * factor;
		}

		ComputeVector vectorSqrt(const ComputeVector& vec)
		{
			return glm::sqrt(vec);
		}

		ComputeVector vectorSqrtEst(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorSubtract(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return vec1 - vec2;
		}

		ComputeVector vectorSubtractAngles(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorSum(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorTruncate(const ComputeVector& vec)
		{
			return glm::trunc(vec);
		}
#pragma endregion

		//
		//Comparison
		//

#pragma region Comparison
		ComputeVector vectorEqual(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::equal(vec1, vec2);
		}

		ComputeVector vectorGreater(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::greaterThan(vec1, vec2);
		}

		ComputeVector vectorGreaterOrEqual(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::greaterThanEqual(vec1, vec2);
		}

		ComputeVector vectorLess(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::lessThan(vec1, vec2);
		}

		ComputeVector vectorLessOrEqual(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::lessThanEqual(vec1, vec2);
		}

		ComputeVector vectorNearEqual(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& epsilon)
		{
			return glm::epsilonEqual(vec1, vec2, epsilon);
		}
#pragma endregion

		//
		//Geometric
		//

#pragma region Geometric
		ComputeVector vectorLerp(const ComputeVector& vec1, const ComputeVector& vec2, float t)
		{
			return glm::lerp(vec1, vec2, t);
		}

		ComputeVector vectorLerpV(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& t)
		{
			return glm::lerp(vec1, vec2, t);
		}
#pragma endregion

		//
		//Bit-Wise
		//

#pragma region Bit-Wise
		ComputeVector vectorAndCInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return vec1 & ~vec2;
		}

		ComputeVector vectorAndInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return vec1 & vec2;
		}

		ComputeVector vectorNorInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return ~(vec1 | vec2);
		}

		ComputeVector vectorNotEqual(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::notEqual(vec1, vec2);
		}

		ComputeVector vectorNotEqualInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::notEqual(vec1, vec2);
		}

		ComputeVector vectorOrInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return vec1 | vec2;
		}

		ComputeVector vectorXorInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return vec1 ^ vec2;
		}
#pragma endregion

		//
		//Transcendental
		//

#pragma region Transcendental
		ComputeVector vectorACos(const ComputeVector& vec)
		{
			return glm::acos(vec);
		}

		ComputeVector vectorACosEst(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorASin(const ComputeVector& vec)
		{
			return glm::sin(vec);
		}

		ComputeVector vectorASinEst(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorATan(const ComputeVector& vec)
		{
			return glm::atan(vec);
		}

		ComputeVector vectorATan2(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return glm::atan2(vec1, vec2);
		}

		ComputeVector vectorATan2Est(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorATanEst(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorCos(const ComputeVector& vec)
		{
			return glm::cos(vec);
		}

		ComputeVector vectorCosEst(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorCosH(const ComputeVector& vec)
		{
			return glm::cosh(vec);
		}

		ComputeVector vectorExp(const ComputeVector& vec)
		{
			return glm::exp(vec);
		}

		ComputeVector vectorExp2(const ComputeVector& vec)
		{
			return glm::exp2(vec);
		}

		ComputeVector vectorExpE(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorLog(const ComputeVector& vec)
		{
			return glm::log(vec);
		}

		ComputeVector vectorLog2(const ComputeVector& vec)
		{
			return glm::log2(vec);
		}

		ComputeVector vectorLogE(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorSin(const ComputeVector& vec)
		{
			return glm::sin(vec);
		}

		void vectorSinCos(ComputeVector& sin, ComputeVector& cos, const ComputeVector& vec)
		{
			sin = glm::sin(vec);
			cos = glm::cos(vec);
		}

		void vectorSinEst(ComputeVector& sin, ComputeVector& cos, const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
		}

		ComputeVector vectorSinH(const ComputeVector& vec)
		{
			return glm::sinh(vec);
		}

		ComputeVector vectorTan(const ComputeVector& vec)
		{
			return glm::tan(vec);
		}

		ComputeVector vectorTanEst(const ComputeVector& vec)
		{
			assert(false && "There isn't a realization.");
			return glm::vec4();
		}

		ComputeVector vectorTanH(const ComputeVector& vec)
		{
			return glm::tanh(vec);
		}
#pragma endregion

		//
		//Quaternion
		//

		ComputeQuaternion quaternionRotationRollPitchYaw(float pitch, float yaw, float roll)
		{
			return glm::quat(glm::vec3(pitch, yaw, roll));
		}

		ComputeQuaternion quaternionRotationRollPitchYaw(const Vector3& angles)
		{
			return quaternionRotationRollPitchYaw(angles.m_x, angles.m_y, angles.m_z);
		}

		//
		//Transforming
		//

		ComputeVector vector3DRotate(const ComputeVector& vector, const ComputeQuaternion& quat)
		{
			return quat * vector;
		}
	}
}
#endif // SIMD_MATH_DX