#include "DefaultRenderingData.h"

#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"

#include "Math/ComputeMatrix.h"
#include "Math/ComputeVector.h"


bool VT::DefaultRenderingData::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;
	IGraphicDevice* device = environment->m_graphicDevice;

	GPUBufferDesc transformBufferDesc{};

	transformBufferDesc.m_byteSize = sizeof(CameraTransforms);
	transformBufferDesc.isHostVisible = true;
	m_cameraTransformBuffer = resManager->createGPUBuffer(transformBufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_COMMON_READ);
	m_cameraTransformCBV = resManager->createBufferResourceDescriptor(m_cameraTransformBuffer);

	device->setResourceName(m_cameraTransformBuffer->getResource(), "CameraTransform");

	return true;

}

void VT::DefaultRenderingData::clear()
{
	m_transforms.clear();
	m_meshes.clear();

	m_pointLights.clear();
	m_directionalLights.clear();
}

void VT::DefaultRenderingData::addMesh(const MeshConstReference& mesh, const Matrix44& transform)
{
	m_transforms.push_back(transform);
	m_meshes.emplace_back(mesh, m_transforms.size() - 1);
}

void VT::DefaultRenderingData::addPointLight(const Vector3& color, float radius, float intensity, const Vector3& position)
{
	m_pointLights.emplace_back(color, radius, intensity, position);
}

void VT::DefaultRenderingData::addDirectionalLight(const Vector3& color, float intensity, const Vector3& dir)
{
	m_directionalLights.emplace_back(color, intensity, dir);
}

void VT::DefaultRenderingData::setCameraTransforms(const CameraTransforms& cameraTransforms)
{
	m_cameraTransforms = cameraTransforms;

	COMPUTE_MATH::ComputeMatrix viewTransform = COMPUTE_MATH::matrixTranspose(
		COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(m_cameraTransforms.m_viewTransform));
	COMPUTE_MATH::ComputeMatrix projectionTransform = COMPUTE_MATH::matrixTranspose(
		COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(m_cameraTransforms.m_projectionTransform));
	COMPUTE_MATH::ComputeMatrix invProjectionTransform = COMPUTE_MATH::matrixTranspose(
		COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(m_cameraTransforms.m_invProjectionTransform));

	CameraTransforms renderingCameraTransforms = m_cameraTransforms;
	renderingCameraTransforms.m_viewTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(viewTransform);
	renderingCameraTransforms.m_projectionTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(projectionTransform);
	renderingCameraTransforms.m_invProjectionTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(invProjectionTransform);

	CameraTransforms* mappingCameraTransforms = nullptr;
	m_cameraTransformBuffer->getTypedResource()->mapData(reinterpret_cast<void**>(&mappingCameraTransforms));
	memcpy(mappingCameraTransforms, &renderingCameraTransforms, sizeof(CameraTransforms));
	m_cameraTransformBuffer->getTypedResource()->unmapData();
}
