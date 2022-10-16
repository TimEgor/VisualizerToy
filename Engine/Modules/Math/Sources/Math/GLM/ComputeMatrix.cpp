#pragma once

#ifdef SIMD_MATH_GLM
#include "Math/ComputeMatrix.h"

#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/transform.hpp>

namespace VT
{
	namespace computeMath
	{
		//
		//Loading
		//

#pragma region Loading
		ComputeMatrix loadComputeMatrixFromMatrix2x2(const Matrix22& matrix)
		{
			return glm::mat4(
				matrix.m_11, matrix.m_12, 0.0f, 0.0f,
				matrix.m_21, matrix.m_22, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f
			);
		}

		ComputeMatrix loadComputeMatrixFromMatrix3x3(const Matrix33& matrix)
		{
			return glm::mat4(
				matrix.m_11, matrix.m_12, matrix.m_13, 0.0f,
				matrix.m_21, matrix.m_22, matrix.m_23, 0.0f,
				matrix.m_31, matrix.m_32, matrix.m_33, 0.0f,
				0.0f, 0.0f, 0.0f, 0.0f
			);
		}

		ComputeMatrix loadComputeMatrixFromMatrix4x4(const Matrix44& matrix)
		{
			return glm::mat4(
				matrix.m_11, matrix.m_12, matrix.m_13, matrix.m_14,
				matrix.m_21, matrix.m_22, matrix.m_23, matrix.m_24,
				matrix.m_31, matrix.m_32, matrix.m_33, matrix.m_34,
				matrix.m_41, matrix.m_42, matrix.m_43, matrix.m_44
			);
		}

		ComputeMatrix loadComputeMatrixFromMatrix4x4(
			float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44)
		{
			return glm::mat4(
				m11, m12, m13, m14,
				m21, m22, m23, m24,
				m31, m32, m33, m34,
				m41, m42, m43, m44
			);
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

			Matrix22 result(
				matrix[0].x, matrix[0].y,
				matrix[1].x, matrix[1].y
			);
			return result;

		}

		Matrix33 saveComputeMatrixToMatrix3x3(const ComputeMatrix& matrix)
		{
			// quick realization
			//TODO: need to rework this method

			Matrix33 result(
				matrix[0].x, matrix[0].y, matrix[0].z,
				matrix[1].x, matrix[1].y, matrix[1].z,
				matrix[2].x, matrix[2].y, matrix[2].z
			);
			return result;
		}

		Matrix44 saveComputeMatrixToMatrix4x4(const ComputeMatrix& matrix)
		{
			Matrix44 result(
				matrix[0].x, matrix[0].y, matrix[0].z, matrix[0].w,
				matrix[1].x, matrix[1].y, matrix[1].z, matrix[1].w,
				matrix[2].x, matrix[2].y, matrix[2].z, matrix[2].w,
				matrix[3].x, matrix[3].y, matrix[3].z, matrix[3].w
			);

			return result;
		}
#pragma endregion

		//
		//Arithmetic
		//

#pragma region Arithmetic
		ComputeVector matrixDeterminant(const ComputeMatrix& matrix)
		{
			return glm::vec4(glm::determinant(matrix));
		}

		ComputeMatrix matrixIdentity()
		{
			return glm::mat4(1.0f);
		}

		ComputeMatrix matrixInverse(const ComputeMatrix& matrix)
		{
			return glm::inverse(matrix);
		}

		bool matrixIsIdentity(const ComputeMatrix& matrix)
		{
			const uint32_t* pWork = reinterpret_cast<const uint32_t*>(&matrix[0][0]);

			uint32_t uOne = pWork[0] ^ 0x3F800000U;


			uint32_t uZero = pWork[1];
			uZero |= pWork[2];
			uZero |= pWork[3];
			// 2nd row
			uZero |= pWork[4];
			uOne |= pWork[5] ^ 0x3F800000U;
			uZero |= pWork[6];
			uZero |= pWork[7];
			// 3rd row
			uZero |= pWork[8];
			uZero |= pWork[9];
			uOne |= pWork[10] ^ 0x3F800000U;
			uZero |= pWork[11];
			// 4th row
			uZero |= pWork[12];
			uZero |= pWork[13];
			uZero |= pWork[14];
			uOne |= pWork[15] ^ 0x3F800000U;
	

			uZero &= 0x7FFFFFFF;    // Allow -0.0f
			uOne |= uZero;
			return (uOne == 0);
		}

		bool matrixIsInfinite(const ComputeMatrix& matrix)
		{
			return glm::isinf(matrix);
		}

		bool matrixIsNaN(const ComputeMatrix& matrix)
		{
			return glm::isnan(matrix);
		}

		ComputeMatrix matrixMultiply(const ComputeMatrix& matrix1, const ComputeMatrix& matrix2)
		{
			return matrix1 * matrix2;
		}

		ComputeMatrix matrixMultiplyTranspose(const ComputeMatrix& matrix1, const ComputeMatrix& matrix2)
		{
			return glm::transpose(matrix1 * matrix2);
		}

		ComputeMatrix matrixTranspose(const ComputeMatrix& matrix)
		{
			return glm::transpose(matrix);
		}
#pragma endregion

		//
		//Transformation
		//

#pragma region Transforming
		ComputeMatrix matrixAffineTransformation(const ComputeVector& scaling, const ComputeQuaternion& rotationOrigin,
			const ComputeQuaternion& rotationQuaternion, const ComputeVector& translation)
		{
			// result = MScaling * Inverse(MRotationOrigin) * MRotation * MRotationOrigin * MTranslation;

			glm::mat4 scalingMatr = matrixIdentity();
			glm::scale(scalingMatr, scaling);
			glm::vec4 rotationOriginVec = XMVectorSelect(g_XMSelect1110.v, RotationOrigin, g_XMSelect1110.v);
			XMMATRIX MRotation = XMMatrixRotationQuaternion(RotationQuaternion);
			XMVECTOR VTranslation = XMVectorSelect(g_XMSelect1110.v, Translation, g_XMSelect1110.v);

			XMMATRIX M;
			M = MScaling;
			M.r[3] = XMVectorSubtract(M.r[3], VRotationOrigin);
			M = XMMatrixMultiply(M, MRotation);
			M.r[3] = XMVectorAdd(M.r[3], VRotationOrigin);
			M.r[3] = XMVectorAdd(M.r[3], VTranslation);

			return glm::(scaling, rotationOrigin, rotationQuaternion, translation);
		}

		ComputeMatrix matrixAffineTransformation2D(const ComputeVector& scaling, const ComputeVector& rotationOrigin,
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
			const ComputeVector& rotationOrigin, const ComputeVector& rotationQuaternion, const ComputeVector& translation)
		{
			return DirectX::XMMatrixTransformation(scalingOrigin, scalingOrientationQuaternion, scaling, rotationOrigin, rotationQuaternion, translation);
		}

		ComputeMatrix matrixTransformation2D(const ComputeVector& scalingOrigin, float scalingOrientationQuaternion, const ComputeVector& scaling,
			const ComputeVector& rotationOrigin, float rotationQuaternion, const ComputeVector& translation)
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
#pragma endregion
	}
}
#endif