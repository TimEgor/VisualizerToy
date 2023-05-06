#pragma once

#include "MeshSystem/MeshHandle.h"
#include "GraphicResourceManager/ObjectHandles.h"
#include "Math/Matrix.h"
#include "LightSources/PointLightData.h"

#include <vector>

#include "LightSources/DirectionalLightData.h"

namespace VT
{
	struct CameraTransforms final
	{
		Matrix44 m_viewTransform = Matrix44Identity;
		Matrix44 m_projectionTransform = Matrix44Identity;
		Matrix44 m_invProjectionTransform = Matrix44Identity;

		Vector3 m_cameraPosition = Vector3Zero;

		float nearPlane = 0.0f;
		float farPlane = 0.0f;
	};

	struct MeshRenderingDataNode final
	{
		MeshConstReference m_mesh = nullptr;
		size_t m_transformIndex = -1;

		MeshRenderingDataNode(const MeshConstReference& mesh, size_t transformIndex)
			: m_mesh(mesh), m_transformIndex(transformIndex) {}
	};

	class DefaultRenderingData final
	{
	public:
		using MeshDataCollection = std::vector<MeshRenderingDataNode>;
		using TransformDataCollection = std::vector<Matrix44>;

		using PointLightDataCollection = std::vector<PointLightData>;
		using DirectionalLightDataCollection = std::vector<DirectionalLightData>;

	private:
		MeshDataCollection m_meshes;
		TransformDataCollection m_transforms;

		PointLightDataCollection m_pointLights;
		DirectionalLightDataCollection m_directionalLights;

		CameraTransforms m_cameraTransforms;

		GPUBufferReference m_cameraTransformBuffer;
		ShaderResourceViewReference m_cameraTransformCBV;

	public:
		DefaultRenderingData() = default;

		bool init();

		void clear();
		void addMesh(const MeshConstReference& mesh, const Matrix44& transform);

		void addPointLight(const Vector3& color, float radius, float intensity, const Vector3& position);
		void addDirectionalLight(const Vector3& color, float intensity, const Vector3& dir);

		void setCameraTransforms(const CameraTransforms& cameraTransforms);

		const TransformDataCollection& getTransformDataCollection() const { return m_transforms; }
		const MeshDataCollection& getMeshDataCollection() const { return m_meshes; }

		const PointLightDataCollection& getPointLighDataCollection() const { return m_pointLights; }
		const DirectionalLightDataCollection& getDirectionalLightDataCollection() const { return m_directionalLights; }

		const CameraTransforms& getCameraTransform() const { return m_cameraTransforms; }
		GPUBufferReference getCameraTransformBuffer() const { return m_cameraTransformBuffer; }
		ShaderResourceViewReference getCameraTransformBufferView() const { return m_cameraTransformCBV; }
	};
}
