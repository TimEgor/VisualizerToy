#pragma once

#ifdef SIMD_MATH_DX
#include "Math/ComputeVector.h"

namespace VT
{
	namespace COMPUTE_MATH
	{
		//
		// Loading
		//

#pragma region Loading
		ComputeVector loadComputeVectorFromVector2(const Vector2& vec)
		{
			return DirectX::XMVectorSet(vec.m_x, vec.m_y, 0.0f, 0.0f);
		}

		ComputeVector loadComputeVectorFromVector3(const Vector3& vec)
		{
			return DirectX::XMVectorSet(vec.m_x, vec.m_y, vec.m_z, 0.0f);
		}

		ComputeVector loadComputeVectorFromVector4(const Vector4& vec)
		{
			return DirectX::XMVectorSet(vec.m_x, vec.m_y, vec.m_z, vec.m_w);
		}

		ComputeVector loadComputeVectorFromVector4(float x, float y, float z, float w)
		{
			return DirectX::XMVectorSet(x, y, z, w);
		}
#pragma endregion

		//
		// Saving
		//

#pragma region Saving
		Vector2 saveComputeVectorToVector2(const ComputeVector& vec)
		{
			Vector2 result;
			DirectX::XMStoreFloat2(reinterpret_cast<DirectX::XMFLOAT2*>(&result), vec);

			return result;
		}

		Vector3 saveComputeVectorToVector3(const ComputeVector& vec)
		{
			Vector3 result;
			DirectX::XMStoreFloat3(reinterpret_cast<DirectX::XMFLOAT3*>(&result), vec);

			return result;
		}

		Vector4 saveComputeVectorToVector4(const ComputeVector& vec)
		{
			Vector4 result;
			DirectX::XMStoreFloat4(reinterpret_cast<DirectX::XMFLOAT4*>(&result), vec);

			return result;
		}
#pragma endregion

		//
		// Arithmetic
		//

#pragma region Arithmetic
		ComputeVector vectorAbs(const ComputeVector& vec)
		{
			return DirectX::XMVectorAbs(vec);
		}

		ComputeVector vectorAdd(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorAdd(vec1, vec2);
		}

		ComputeVector vectorAddAngles(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorAddAngles(vec1, vec2);
		}

		ComputeVector vectorCeiling(const ComputeVector& vec)
		{
			return DirectX::XMVectorCeiling(vec);
		}

		ComputeVector vectorClamp(const ComputeVector& vec, const ComputeVector& minVec, const ComputeVector& maxVec)
		{
			return DirectX::XMVectorClamp(vec, minVec, maxVec);
		}

		ComputeVector vectorDivide(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorDivide(vec1, vec2);
		}

		ComputeVector vectorFloor(const ComputeVector& vec)
		{
			return DirectX::XMVectorFloor(vec);
		}

		ComputeVector vectorIsInfinite(const ComputeVector& vec)
		{
			return DirectX::XMVectorIsInfinite(vec);
		}

		ComputeVector vectorIsNaN(const ComputeVector& vec)
		{
			return DirectX::XMVectorIsNaN(vec);
		}

		ComputeVector vectorMax(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorMax(vec1, vec2);
		}

		ComputeVector vectorMin(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorMin(vec1, vec2);
		}

		ComputeVector vectorMod(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorMod(vec1, vec2);
		}

		ComputeVector vectorModAngles(const ComputeVector& vec)
		{
			return DirectX::XMVectorModAngles(vec);
		}

		ComputeVector vectorMultiply(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorMultiply(vec1, vec2);
		}

		ComputeVector vectorMultiplyAdd(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& vec3)
		{
			return DirectX::XMVectorMultiplyAdd(vec1, vec2, vec3);
		}

		ComputeVector vectorNegate(const ComputeVector& vec)
		{
			return DirectX::XMVectorNegate(vec);
		}

		ComputeVector vectorNegativeMultiplySubtract(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& vec3)
		{
			return DirectX::XMVectorNegativeMultiplySubtract(vec1, vec2, vec3);
		}

		ComputeVector vectorPow(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorPow(vec1, vec2);
		}

		ComputeVector vectorReciprocal(const ComputeVector& vec)
		{
			return DirectX::XMVectorReciprocal(vec);
		}

		ComputeVector vectorReciprocalEst(const ComputeVector& vec)
		{
			return DirectX::XMVectorReciprocalEst(vec);
		}

		ComputeVector vectorReciprocalSqrt(const ComputeVector& vec)
		{
			return DirectX::XMVectorReciprocalSqrt(vec);
		}

		ComputeVector vectorReciprocalSqrtEst(const ComputeVector& vec)
		{
			return DirectX::XMVectorReciprocalSqrtEst(vec);
		}

		ComputeVector vectorRound(const ComputeVector& vec)
		{
			return DirectX::XMVectorRound(vec);
		}

		ComputeVector vectorSaturate(const ComputeVector& vec)
		{
			return DirectX::XMVectorSaturate(vec);
		}

		ComputeVector vectorScale(const ComputeVector& vec, float factor)
		{
			return DirectX::XMVectorScale(vec, factor);
		}

		ComputeVector vectorSqrt(const ComputeVector& vec)
		{
			return DirectX::XMVectorSqrt(vec);
		}

		ComputeVector vectorSqrtEst(const ComputeVector& vec)
		{
			return DirectX::XMVectorSqrtEst(vec);
		}

		ComputeVector vectorSubtract(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorSubtract(vec1, vec2);
		}

		ComputeVector vectorSubtractAngles(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorSubtractAngles(vec1, vec2);
		}

		ComputeVector vectorSum(const ComputeVector& vec)
		{
			return DirectX::XMVectorSum(vec);
		}

		ComputeVector vectorTruncate(const ComputeVector& vec)
		{
			return DirectX::XMVectorTruncate(vec);
		}
#pragma endregion

		//
		//Comparison
		//

#pragma region Comparison
		ComputeVector vectorEqual(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorEqual(vec1, vec2);
		}

		ComputeVector vectorGreater(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorGreater(vec1, vec2);
		}

		ComputeVector vectorGreaterOrEqual(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorGreaterOrEqual(vec1, vec2);
		}

		ComputeVector vectorLess(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorLess(vec1, vec2);
		}

		ComputeVector vectorLessOrEqual(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorLessOrEqual(vec1, vec2);
		}

		ComputeVector vectorNearEqual(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& epsilon)
		{
			return DirectX::XMVectorNearEqual(vec1, vec2, epsilon);
		}
#pragma endregion

		//
		//Geometric
		//

#pragma region Geometric
		ComputeVector vectorLerp(const ComputeVector& vec1, const ComputeVector& vec2, float t)
		{
			return DirectX::XMVectorLerp(vec1, vec2, t);
		}

		ComputeVector vectorLerpV(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& t)
		{
			return DirectX::XMVectorLerpV(vec1, vec2, t);
		}
#pragma endregion

		//
		//Bit-Wise
		//

#pragma region Bit-Wise
		ComputeVector vectorAndCInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorAndCInt(vec1, vec2);
		}

		ComputeVector vectorAndInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorAndInt(vec1, vec2);
		}

		ComputeVector vectorNorInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorNorInt(vec1, vec2);
		}

		ComputeVector vectorNotEqual(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorNotEqual(vec1, vec2);
		}

		ComputeVector vectorNotEqualInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorNotEqualInt(vec1, vec2);
		}

		ComputeVector vectorOrInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorOrInt(vec1, vec2);
		}

		ComputeVector vectorXorInt(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorXorInt(vec1, vec2);
		}
#pragma endregion

		//
		//Transcendental
		//

#pragma region Transcendental
		ComputeVector vectorACos(const ComputeVector& vec)
		{
			return DirectX::XMVectorACos(vec);
		}

		ComputeVector vectorACosEst(const ComputeVector& vec)
		{
			return DirectX::XMVectorACosEst(vec);
		}

		ComputeVector vectorASin(const ComputeVector& vec)
		{
			return DirectX::XMVectorASin(vec);
		}

		ComputeVector vectorASinEst(const ComputeVector& vec)
		{
			return DirectX::XMVectorASinEst(vec);
		}

		ComputeVector vectorATan(const ComputeVector& vec)
		{
			return DirectX::XMVectorATan(vec);
		}

		ComputeVector vectorATan2(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorATan2(vec1, vec2);
		}

		ComputeVector vectorATan2Est(const ComputeVector& vec1, const ComputeVector& vec2)
		{
			return DirectX::XMVectorATan2Est(vec1, vec2);
		}

		ComputeVector vectorATanEst(const ComputeVector& vec)
		{
			return DirectX::XMVectorATanEst(vec);
		}

		ComputeVector vectorCos(const ComputeVector& vec)
		{
			return DirectX::XMVectorCos(vec);
		}

		ComputeVector vectorCosEst(const ComputeVector& vec)
		{
			return DirectX::XMVectorCosEst(vec);
		}

		ComputeVector vectorCosH(const ComputeVector& vec)
		{
			return DirectX::XMVectorCosH(vec);
		}

		ComputeVector vectorExp(const ComputeVector& vec)
		{
			return DirectX::XMVectorExp(vec);
		}

		ComputeVector vectorExp2(const ComputeVector& vec)
		{
			return DirectX::XMVectorExp2(vec);
		}

		ComputeVector vectorExpE(const ComputeVector& vec)
		{
			return DirectX::XMVectorExpE(vec);
		}

		ComputeVector vectorLog(const ComputeVector& vec)
		{
			return DirectX::XMVectorLog(vec);
		}

		ComputeVector vectorLog2(const ComputeVector& vec)
		{
			return DirectX::XMVectorLog2(vec);
		}

		ComputeVector vectorLogE(const ComputeVector& vec)
		{
			return DirectX::XMVectorLogE(vec);
		}

		ComputeVector vectorSin(const ComputeVector& vec)
		{
			return DirectX::XMVectorSin(vec);
		}

		void vectorSinCos(ComputeVector& sin, ComputeVector& cos, const ComputeVector& vec)
		{
			return DirectX::XMVectorSinCos(&sin, &cos, vec);
		}

		void vectorSinEst(ComputeVector& sin, ComputeVector& cos, const ComputeVector& vec)
		{
			return DirectX::XMVectorSinCosEst(&sin, &cos, vec);
		}

		ComputeVector vectorSinH(const ComputeVector& vec)
		{
			return DirectX::XMVectorSinH(vec);
		}

		ComputeVector vectorTan(const ComputeVector& vec)
		{
			return DirectX::XMVectorTan(vec);
		}

		ComputeVector vectorTanEst(const ComputeVector& vec)
		{
			return DirectX::XMVectorTanEst(vec);
		}

		ComputeVector vectorTanH(const ComputeVector& vec)
		{
			return DirectX::XMVectorTanH(vec);
		}
#pragma endregion

		//
		//Quaternion
		//

		ComputeQuaternion quaternionRotationRollPitchYaw(float pitch, float yaw, float roll)
		{
			return DirectX::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
		}

		ComputeQuaternion quaternionRotationRollPitchYaw(const Vector3& angles)
		{
			return DirectX::XMQuaternionRotationRollPitchYaw(angles.m_x, angles.m_y, angles.m_z);
		}

		//
		//Transforming
		//

		ComputeVector vector3DRotate(const ComputeVector& vector, const ComputeVector& quat)
		{
			return DirectX::XMVector3Rotate(vector, quat);
		}
	}
}
#endif // SIMD_MATH_DX