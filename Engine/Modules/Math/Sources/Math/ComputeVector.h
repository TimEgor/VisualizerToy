#pragma once

#include "Vector.h"
#include "ComputeMath.h"

namespace VT
{
	namespace COMPUTE_MATH
	{
		//Loading
		ComputeVector loadComputeVectorFromVector2(const Vector2& vec);
		ComputeVector loadComputeVectorFromVector3(const Vector3& vec);
		ComputeVector loadComputeVectorFromVector4(const Vector4& vec);
		ComputeVector loadComputeVectorFromVector4(float x, float y, float z, float w);

		//Saving
		Vector2 saveComputeVectorToVector2(const ComputeVector& vec);
		Vector3 saveComputeVectorToVector3(const ComputeVector& vec);
		Vector4 saveComputeVectorToVector4(const ComputeVector& vec);

		//Arithmetic
		ComputeVector vectorAbs(const ComputeVector& vec);
		ComputeVector vectorAdd(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorAddAngles(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorCeiling(const ComputeVector& vec);
		ComputeVector vectorClamp(const ComputeVector& vec, const ComputeVector& minVec, const ComputeVector& maxVec);
		ComputeVector vectorDivide(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorFloor(const ComputeVector& vec);
		ComputeVector vectorIsInfinite(const ComputeVector& vec);
		ComputeVector vectorIsNaN(const ComputeVector& vec);
		ComputeVector vectorMax(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorMin(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorMod(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorModAngles(const ComputeVector& vec);
		ComputeVector vectorMultiply(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorMultiplyAdd(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& vec3);
		ComputeVector vectorNegate(const ComputeVector& vec);
		ComputeVector vectorNegativeMultiplySubtract(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& vec3);
		ComputeVector vectorPow(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorReciprocal(const ComputeVector& vec);
		ComputeVector vectorReciprocalEst(const ComputeVector& vec);
		ComputeVector vectorReciprocalSqrt(const ComputeVector& vec);
		ComputeVector vectorReciprocalSqrtEst(const ComputeVector& vec);
		ComputeVector vectorRound(const ComputeVector& vec);
		ComputeVector vectorSaturate(const ComputeVector& vec);
		ComputeVector vectorScale(const ComputeVector& vec, float factor);
		ComputeVector vectorSqrt(const ComputeVector& vec);
		ComputeVector vectorSqrtEst(const ComputeVector& vec);
		ComputeVector vectorSubtract(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorSubtractAngles(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorSum(const ComputeVector& vec);
		ComputeVector vectorTruncate(const ComputeVector& vec);

		//Comparison
		ComputeVector vectorEqual(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorGreater(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorGreaterOrEqual(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorLess(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorLessOrEqual(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorNearEqual(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& epsilon);

		//Geometric
		ComputeVector vectorLerp(const ComputeVector& vec1, const ComputeVector& vec2, float t);
		ComputeVector vectorLerpV(const ComputeVector& vec1, const ComputeVector& vec2, const ComputeVector& t);

		//Bit-Wise
		ComputeVector vectorAndCInt(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorAndInt(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorNorInt(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorNotEqual(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorNotEqualInt(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorOrInt(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorXorInt(const ComputeVector& vec1, const ComputeVector& vec2);

		//Transcendental
		ComputeVector vectorACos(const ComputeVector& vec);
		ComputeVector vectorACosEst(const ComputeVector& vec);
		ComputeVector vectorASin(const ComputeVector& vec);
		ComputeVector vectorASinEst(const ComputeVector& vec);
		ComputeVector vectorATan(const ComputeVector& vec);
		ComputeVector vectorATan2(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorATan2Est(const ComputeVector& vec1, const ComputeVector& vec2);
		ComputeVector vectorATanEst(const ComputeVector& vec);
		ComputeVector vectorCos(const ComputeVector& vec);
		ComputeVector vectorCosEst(const ComputeVector& vec);
		ComputeVector vectorCosH(const ComputeVector& vec);
		ComputeVector vectorExp(const ComputeVector& vec);
		ComputeVector vectorExp2(const ComputeVector& vec);
		ComputeVector vectorExpE(const ComputeVector& vec);
		ComputeVector vectorLog(const ComputeVector& vec);
		ComputeVector vectorLog2(const ComputeVector& vec);
		ComputeVector vectorLogE(const ComputeVector& vec);
		ComputeVector vectorSin(const ComputeVector& vec);
		void vectorSinCos(ComputeVector& sin, ComputeVector& cos, const ComputeVector& vec);
		void vectorSinEst(ComputeVector& sin, ComputeVector& cos, const ComputeVector& vec);
		ComputeVector vectorSinH(const ComputeVector& vec);
		ComputeVector vectorTan(const ComputeVector& vec);
		ComputeVector vectorTanEst(const ComputeVector& vec);
		ComputeVector vectorTanH(const ComputeVector& vec);

		//TODO: Complete function set
		//Quaternion
		ComputeQuaternion quaternionRotationRollPitchYaw(float pitch, float yaw, float roll);
		ComputeQuaternion quaternionRotationRollPitchYaw(const Vector3& angles);

		//TODO: Complete function set
		//Transforming
		ComputeVector vector3DRotate(const ComputeVector& vector, const ComputeQuaternion& quat);
	}
}