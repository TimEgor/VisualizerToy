#pragma once

#include "Matrix.h"
#include "Vector.h"
#include "ComputeMath.h"

namespace VT
{
	namespace COMPUTE_MATH
	{
		//Loading
		ComputeMatrix loadComputeMatrixFromMatrix2x2(const Matrix22& matrix);
		ComputeMatrix loadComputeMatrixFromMatrix3x3(const Matrix33& matrix);
		ComputeMatrix loadComputeMatrixFromMatrix4x4(const Matrix44& matrix);
		ComputeMatrix loadComputeMatrixFromMatrix4x4(
			float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44);

		//Saving
		Matrix22 saveComputeMatrixToMatrix2x2(const ComputeMatrix& matrix);
		Matrix33 saveComputeMatrixToMatrix3x3(const ComputeMatrix& matrix);
		Matrix44 saveComputeMatrixToMatrix4x4(const ComputeMatrix& matrix);

		//Arithmetic
		ComputeVector matrixDeterminant(const ComputeMatrix& matrix);
		ComputeMatrix matrixIdentity();
		ComputeMatrix matrixInverse(const ComputeMatrix& matrix);
		bool matrixIsIdentity(const ComputeMatrix& matrix);
		bool matrixIsInfinite(const ComputeMatrix& matrix);
		bool matrixIsNaN(const ComputeMatrix& matrix);
		ComputeMatrix matrixMultiply(const ComputeMatrix& matrix1, const ComputeMatrix& matrix2);
		ComputeMatrix matrixMultiplyTranspose(const ComputeMatrix& matrix1, const ComputeMatrix& matrix2);
		ComputeMatrix matrixTranspose(const ComputeMatrix& matrix);

		//Transformation
		ComputeMatrix matrixAffineTransformation(const ComputeVector& scaling, const ComputeQuaternion& rotationOrigin,
			const ComputeQuaternion& rotationQuaternion, const ComputeVector& translation);
		ComputeMatrix matrixAffineTransformation2D(const ComputeVector& scaling, const ComputeQuaternion& rotationOrigin,
			float rotation, const ComputeVector& translation);
		bool matrixDecompose(ComputeVector& outScale, ComputeQuaternion& outRotation, ComputeVector& outTranslation,
			const ComputeMatrix& matrix);
		ComputeMatrix matrixLookAtLH(const ComputeVector& eyePos, const ComputeVector& focusPos, const ComputeVector& upDir);
		ComputeMatrix matrixLookAtRH(const ComputeVector& eyePos, const ComputeVector& focusPos, const ComputeVector& upDir);
		ComputeMatrix matrixLookToLH(const ComputeVector& eyePos, const ComputeVector& eyeDir, const ComputeVector& upDir);
		ComputeMatrix matrixLookToRH(const ComputeVector& eyePos, const ComputeVector& eyeDir, const ComputeVector& upDir);
		ComputeMatrix matrixOrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ);
		ComputeMatrix matrixOrthographicRH(float viewWidth, float viewHeight, float nearZ, float farZ);
		ComputeMatrix matrixOrthographicOffCenterLH(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);
		ComputeMatrix matrixOrthographicOffCenterRH(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);
		ComputeMatrix matrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
		ComputeMatrix matrixPerspectiveFovRH(float fovAngleY, float aspectRatio, float nearZ, float farZ);
		ComputeMatrix matrixPerspectiveLH(float viewWidth, float viewHeight, float nearZ, float farZ);
		ComputeMatrix matrixPerspectiveRH(float viewWidth, float viewHeight, float nearZ, float farZ);
		ComputeMatrix matrixPerspectiveOffCenterLH(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);
		ComputeMatrix matrixPerspectiveOffCenterRH(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ);
		ComputeMatrix matrixReflect(const ComputeVector& reflectionPlane);
		ComputeMatrix matrixRotationAxis(const ComputeVector& axis, float angle);
		ComputeMatrix matrixRotationNormal(const ComputeVector& normalAxis, float angle);
		ComputeMatrix matrixRotationQuaternion(const ComputeVector& quaternion);
		ComputeMatrix matrixRotationRollPithYaw(float pitch, float yaw, float roll);
		ComputeMatrix matrixRotationRollPithYaw(const Vector3& angles);
		ComputeMatrix matrixRotationRollPithYaw(const ComputeVector& angles);
		ComputeMatrix matrixRotationX(float angle);
		ComputeMatrix matrixRotationY(float angle);
		ComputeMatrix matrixRotationZ(float angle);
		ComputeMatrix matrixScaling(float scaleX, float scaleY, float scaleZ);
		ComputeMatrix matrixScaling(const Vector3& scale);
		ComputeMatrix matrixScaling(const ComputeVector& scale);
		ComputeMatrix matrixShadow(const ComputeVector& shadowPlane, const ComputeVector& lightPosition);
		ComputeMatrix matrixTransformation(const ComputeVector& scalingOrigin, const ComputeQuaternion& scalingOrientationQuaternion, const ComputeVector& scaling,
			const ComputeQuaternion& rotationOrigin, const ComputeQuaternion& rotationQuaternion, const ComputeVector& translation);
		ComputeMatrix matrixTransformation2D(const ComputeVector& scalingOrigin, float scalingOrientationQuaternion, const ComputeVector& scaling,
			const ComputeQuaternion& rotationOrigin, float rotationQuaternion, const ComputeVector& translation);
		ComputeMatrix matrixTranslation(float offsetX, float offsetY, float offsetZ);
		ComputeMatrix matrixTranslation(const Vector3& pos);
		ComputeMatrix matrixTranslation(const ComputeVector& offsets);
	}
}