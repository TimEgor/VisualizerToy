#pragma once

#include "MeshSystem/MeshHandle.h"
#include "Math/Matrix.h"

#include <vector>

namespace VT
{
	struct alignas(16) CameraTransforms final
	{
		Matrix44 m_viewTransform = Matrix44Identity;
		Matrix44 m_projectionTransform = Matrix44Identity;
	};

	struct MeshRenderingDataNode final
	{
		MeshConstReference m_mesh = nullptr;
		size_t m_transformIndex = -1;

		MeshRenderingDataNode(const MeshConstReference& mesh, size_t transformIndex)
			: m_mesh(mesh), m_transformIndex(transformIndex) {}
	};

	class RenderingData final
	{
	public:
		using MeshDataCollection = std::vector<MeshRenderingDataNode>;
		using TransformDataCollection = std::vector<Matrix44>;

	private:
		MeshDataCollection m_meshes;
		TransformDataCollection m_transforms;

	public:
		RenderingData() = default;

		void clear();
		void addMesh(const MeshConstReference& mesh, const Matrix44& transform);

		const TransformDataCollection& getTransformDataCollection() const { return m_transforms; }
		const MeshDataCollection& getMeshDataCollection() const { return m_meshes; }
	};
}
