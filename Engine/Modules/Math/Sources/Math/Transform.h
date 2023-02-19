#pragma once

#include "Matrix.h"

namespace VT
{
	using TransformMatrix = Matrix44;

	struct Transform final
	{
		TransformMatrix m_matrix = Matrix44Identity;

		Transform() = default;

		const Vector3& getAxisX() const { return m_matrix.m_row1.m_xyz; }
		const Vector3& getAxisY() const { return m_matrix.m_row2.m_xyz; }
		const Vector3& getAxisZ() const { return m_matrix.m_row3.m_xyz; }
		const Vector3& getOrigin() const { return m_matrix.m_row4.m_xyz; }
	};
}
