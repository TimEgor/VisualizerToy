#pragma once

#include "MeshSystem/MeshHandle.h"
#include "GraphicResourceManager/ObjectHandles.h"
#include "Math/Matrix.h"
#include "LightSources/PointLightData.h"

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

		using PointLightDataCollection = std::vector<PointLightData>;

	private:
		MeshDataCollection m_meshes;
		TransformDataCollection m_transforms;

		PointLightDataCollection m_pointLights;

		CameraTransforms m_cameraTransforms;

		GPUBufferReference m_cameraTransformBuffer;
		ShaderResourceViewReference m_cameraTransformCBV;

	public:
		RenderingData() = default;

		void init();

		void clear();
		void addMesh(const MeshConstReference& mesh, const Matrix44& transform);

		void addPointLight(const Vector3& color, float radius, const Vector3& position);

		void setCameraTransforms();

		const TransformDataCollection& getTransformDataCollection() const { return m_transforms; }
		const MeshDataCollection& getMeshDataCollection() const { return m_meshes; }

		const PointLightDataCollection& getPointLighDataCollection() const { return m_pointLights; }

		const CameraTransforms& getCameraTransforms() const { return m_cameraTransforms; }
		GPUBufferReference getCameraTransformBuffer() const { return m_cameraTransformBuffer; }
		ShaderResourceViewReference getCameraTransformBufferView() const { return m_cameraTransformCBV; }
	};
}
