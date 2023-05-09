#pragma once

#ifdef VT_SIMD_MATH_DX
#include "Math/ComputeMatrix.h"

namespace VT
{
	namespace COMPUTE_MATH
	{
		//
		//Loading
		//

#pragma region Loading
		ComputeMatrix loadComputeMatrixFromMatrix2x2(const Matrix22& matrix)
		{
			return DirectX::XMMatrixSet(
				matrix.m_11, matrix.m_12, 0.0f, 0.0f,
				matrix.m_21, matrix.m_22, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f);
		}

		ComputeMatrix loadComputeMatrixFromMatrix3x3(const Matrix33& matrix)
		{
			return DirectX::XMLoadFloat3x3(reinterpret_cast<const DirectX::XMFLOAT3X3*>(&matrix));
		}

		ComputeMatrix loadComputeMatrixFromMatrix4x4(const Matrix44& matrix)
		{
			return DirectX::XMLoadFloat4x4(reinterpret_cast<const DirectX::XMFLOAT4X4*>(&matrix));
		}

		ComputeMatrix loadComputeMatrixFromMatrix4x4(
			float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44)
		{
			return DirectX::XMMatrixSet(
				m11, m12, m13, m14,
				m21, m22, m23, m24,
				m31, m32, m33, m34,
				m41, m42, m43, m44);
		}
#pragma endregion

		//
		//Saving
		//

#pragma region Saving
		Matrix22 saveComputeMatrixToMatrix2x2(const ComputeMatrix& matrix)
		{
			// quick realization
			//TODO: need to rework this method

			Matrix44 tmp;
			DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&tmp), matrix);

			Matrix22 result(tmp.m_11, tmp.m_12, tmp.m_21, tmp.m_22);
			return result;

		}

		Matrix33 saveComputeMatrixToMatrix3x3(const ComputeMatrix& matrix)
		{
			Matrix33 result;
			DirectX::XMStoreFloat3x3(reinterpret_cast<DirectX::XMFLOAT3X3*>(&result), matrix);

			return result;
		}

		Matrix44 saveComputeMatrixToMatrix4x4(const ComputeMatrix& matrix)
		{
			Matrix44 result;
			DirectX::XMStoreFloat4x4(reinterpret_cast<DirectX::XMFLOAT4X4*>(&result), matrix);

			return result;
		}
#pragma endregion

		//
		//Arithmetic
		//

#pragma region Arithmetic
		ComputeVector matrixDeterminant(const ComputeMatrix& matrix)
		{
			return DirectX::XMMatrixDeterminant(matrix);
		}

		ComputeMatrix matrixIdentity()
		{
			return DirectX::XMMatrixIdentity();
		}

		ComputeMatrix matrixInverse(const ComputeMatrix& matrix)
		{
			return DirectX::XMMatrixInverse(nullptr, matrix);
		}

		bool matrixIsIdentity(const ComputeMatrix& matrix)
		{
			return DirectX::XMMatrixIsIdentity(matrix);
		}

		bool matrixIsInfinite(const ComputeMatrix& matrix)
		{
			return DirectX::XMMatrixIsInfinite(matrix);
		}

		bool matrixIsNaN(const ComputeMatrix& matrix)
		{
			return DirectX::XMMatrixIsNaN(matrix);
		}

		ComputeMatrix matrixMultiply(const ComputeMatrix& matrix1, const ComputeMatrix& matrix2)
		{
			return DirectX::XMMatrixMultiply(matrix1, matrix2);
		}

		ComputeMatrix matrixMultiplyTranspose(const ComputeMatrix& matrix1, const ComputeMatrix& matrix2)
		{
			return DirectX::XMMatrixMultiplyTranspose(matrix1, matrix2);
		}

		ComputeMatrix matrixTranspose(const ComputeMatrix& matrix)
		{
			return DirectX::XMMatrixTranspose(matrix);
		}
#pragma endregion

		//
		//Transformation
		//

#pragma region Transforming
		ComputeMatrix matrixAffineTransformation(const ComputeVector& scaling, const ComputeQuaternion& rotationOrigin,
			const ComputeQuaternion& rotationQuaternion, const ComputeVector& translation)
		{
			return DirectX::XMMatrixAffineTransformation(scaling, rotationOrigin, rotationQuaternion, translation);
		}

		ComputeMatrix matrixAffineTransformation2D(const ComputeVector& scaling, const ComputeQuaternion& rotationOrigin,
			float rotation, const ComputeVector& translation)
		{
			return DirectX::XMMatrixAffineTransformation2D(scaling, rotationOrigin, rotation, translation);
		}

		bool matrixDecompose(ComputeVector& outScale, ComputeVector& outRotation, ComputeVector& outTranslation,
			const ComputeMatrix& matrix)
		{
			return DirectX::XMMatrixDecompose(&outScale, &outRotation, &outTranslation, matrix);
		}

		ComputeMatrix matrixLookAtLH(const ComputeVector& eyePos, const ComputeVector& focusPos, const ComputeVector& upDir)
		{
			return DirectX::XMMatrixLookAtLH(eyePos, focusPos, upDir);
		}

		ComputeMatrix matrixLookAtRH(const ComputeVector& eyePos, const ComputeVector& focusPos, const ComputeVector& upDir)
		{
			return DirectX::XMMatrixLookAtRH(eyePos, focusPos, upDir);
		}

		ComputeMatrix matrixLookToLH(const ComputeVector& eyePos, const ComputeVector& eyeDir, const ComputeVector& upDir)
		{
			return DirectX::XMMatrixLookToLH(eyePos, eyeDir, upDir);
		}

		ComputeMatrix matrixLookToRH(const ComputeVector& eyePos, const ComputeVector& eyeDir, const ComputeVector& upDir)
		{
			return DirectX::XMMatrixLookToRH(eyePos, eyeDir, upDir);
		}

		ComputeMatrix matrixOrthographicLH(float viewWidth, float viewHeight, float nearZ, float farZ)
		{
			return DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, nearZ, farZ);
		}

		ComputeMatrix matrixOrthographicRH(float viewWidth, float viewHeight, float nearZ, float farZ)
		{
			return DirectX::XMMatrixOrthographicRH(viewWidth, viewHeight, nearZ, farZ);
		}

		ComputeMatrix matrixOrthographicOffCenterLH(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ)
		{
			return DirectX::XMMatrixOrthographicOffCenterLH(viewLeft, viewRight, viewBottom, viewTop, nearZ, farZ);
		}

		ComputeMatrix matrixOrthographicOffCenterRH(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ)
		{
			return DirectX::XMMatrixOrthographicOffCenterRH(viewLeft, viewRight, viewBottom, viewTop, nearZ, farZ);
		}

		ComputeMatrix matrixPerspectiveFovLH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
		{
			return DirectX::XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
		}

		ComputeMatrix matrixPerspectiveFovRH(float fovAngleY, float aspectRatio, float nearZ, float farZ)
		{
			return DirectX::XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, nearZ, farZ);
		}

		ComputeMatrix matrixPerspectiveLH(float viewWidth, float viewHeight, float nearZ, float farZ)
		{
			return DirectX::XMMatrixPerspectiveLH(viewWidth, viewHeight, nearZ, farZ);
		}

		ComputeMatrix matrixPerspectiveRH(float viewWidth, float viewHeight, float nearZ, float farZ)
		{
			return DirectX::XMMatrixPerspectiveRH(viewWidth, viewHeight, nearZ, farZ);
		}

		ComputeMatrix matrixPerspectiveOffCenterLH(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ)
		{
			return DirectX::XMMatrixPerspectiveOffCenterLH(viewLeft, viewRight, viewBottom, viewTop, nearZ, farZ);
		}

		ComputeMatrix matrixPerspectiveOffCenterRH(float viewLeft, float viewRight, float viewBottom, float viewTop, float nearZ, float farZ)
		{
			return DirectX::XMMatrixPerspectiveOffCenterRH(viewLeft, viewRight, viewBottom, viewTop, nearZ, farZ);
		}

		ComputeMatrix matrixReflect(const ComputeVector& reflectionPlane)
		{
			return DirectX::XMMatrixReflect(reflectionPlane);
		}

		ComputeMatrix matrixRotationAxis(const ComputeVector& axis, float angle)
		{
			return DirectX::XMMatrixRotationAxis(axis, angle);
		}

		ComputeMatrix matrixRotationNormal(const ComputeVector& normalAxis, float angle)
		{
			return DirectX::XMMatrixRotationNormal(normalAxis, angle);
		}

		ComputeMatrix matrixRotationQuaternion(const ComputeVector& quaternion)
		{
			return DirectX::XMMatrixRotationQuaternion(quaternion);
		}

		ComputeMatrix matrixRotationRollPithYaw(float pitch, float yaw, float roll)
		{
			return DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
		}

		ComputeMatrix matrixRotationRollPithYaw(const Vector3& angles)
		{
			return DirectX::XMMatrixRotationRollPitchYaw(angles.m_x, angles.m_y, angles.m_z);
		}

		ComputeMatrix matrixRotationRollPithYaw(const ComputeVector& angles)
		{
			return DirectX::XMMatrixRotationRollPitchYawFromVector(angles);
		}

		ComputeMatrix matrixRotationX(float angle)
		{
			return DirectX::XMMatrixRotationX(angle);
		}

		ComputeMatrix matrixRotationY(float angle)
		{
			return DirectX::XMMatrixRotationY(angle);
		}

		ComputeMatrix matrixRotationZ(float angle)
		{
			return DirectX::XMMatrixRotationZ(angle);
		}

		ComputeMatrix matrixScaling(float scaleX, float scaleY, float scaleZ)
		{
			return DirectX::XMMatrixScaling(scaleX, scaleY, scaleZ);
		}

		ComputeMatrix matrixScaling(const Vector3& scale)
		{
			return DirectX::XMMatrixScaling(scale.m_x, scale.m_y, scale.m_z);
		}

		ComputeMatrix matrixScaling(const ComputeVector& angles)
		{
			return DirectX::XMMatrixScalingFromVector(angles);
		}

		ComputeMatrix matrixShadow(const ComputeVector& shadowPlane, const ComputeVector& lightPosition)
		{
			return DirectX::XMMatrixShadow(shadowPlane, lightPosition);
		}

		ComputeMatrix matrixTransformation(const ComputeVector& scalingOrigin, const ComputeVector& scalingOrientationQuaternion, const ComputeVector& scaling,
			const ComputeQuaternion& rotationOrigin, const ComputeQuaternion& rotationQuaternion, const ComputeVector& translation)
		{
			return DirectX::XMMatrixTransformation(scalingOrigin, scalingOrientationQuaternion, scaling, rotationOrigin, rotationQuaternion, translation);
		}

		ComputeMatrix matrixTransformation2D(const ComputeVector& scalingOrigin, float scalingOrientationQuaternion, const ComputeVector& scaling,
			const ComputeQuaternion& rotationOrigin, float rotationQuaternion, const ComputeVector& translation)
		{
			return DirectX::XMMatrixTransformation2D(scalingOrigin, scalingOrientationQuaternion, scaling, rotationOrigin, rotationQuaternion, translation);
		}

		ComputeMatrix matrixTranslation(float offsetX, float offsetY, float offsetZ)
		{
			return DirectX::XMMatrixTranslation(offsetX, offsetY, offsetZ);
		}

		ComputeMatrix matrixTranslation(const Vector3& pos)
		{
			return DirectX::XMMatrixTranslation(pos.m_x, pos.m_y, pos.m_z);
		}

		ComputeMatrix matrixTranslation(const ComputeVector& offsets)
		{
			return DirectX::XMMatrixTranslationFromVector(offsets);
		}

		ComputeVector transformPoint(const ComputeVector& point, const ComputeMatrix& transform)
		{
			return DirectX::XMVector3TransformCoord(point, transform);
		}
		ComputeVector transformVector(const ComputeVector& vector, const ComputeMatrix& transform)
		{
			return DirectX::XMVector3TransformNormal(vector, transform);
		}
#pragma endregion
	}
}
#endif // VT_SIMD_MATH_DX