#include "RenderingData.h"

#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"

#include "Math/ComputeMatrix.h"
#include "Math/ComputeVector.h"
#include "Math/Consts.h"


bool VT::RenderingData::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	GPUBufferDesc transformBufferDesc{};

	transformBufferDesc.m_byteSize = sizeof(CameraTransforms);
	transformBufferDesc.isHostVisible = true;
	m_cameraTransformBuffer = resManager->createGPUBuffer(transformBufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_COMMON_READ);
	m_cameraTransformCBV = resManager->createBufferResourceDescriptor(m_cameraTransformBuffer);

	return true;

}

void VT::RenderingData::clear()
{
	m_transforms.clear();
	m_meshes.clear();

	m_pointLights.clear();
}

void VT::RenderingData::addMesh(const MeshConstReference& mesh, const Matrix44& transform)
{
	m_transforms.push_back(transform);
	m_meshes.emplace_back(mesh, m_transforms.size() - 1);
}

void VT::RenderingData::addPointLight(const Vector3& color, float radius, const Vector3& position)
{
	m_pointLights.emplace_back(color, radius, position);
}

void VT::RenderingData::setCameraTransforms()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicDevice* device = environment->m_graphicDevice;

	const Vector3 cameraPos(0.0f, 0.0f, -6.0f);

	COMPUTE_MATH::ComputeMatrix viewTransform = COMPUTE_MATH::matrixLookToLH(
		COMPUTE_MATH::loadComputeVectorFromVector3(cameraPos),
		COMPUTE_MATH::loadComputeVectorFromVector3(Vector3UnitZ),
		COMPUTE_MATH::loadComputeVectorFromVector3(Vector3UnitY)
	);

	m_cameraTransforms.m_cameraPosition = cameraPos;

	m_cameraTransforms.nearPlane = 0.1f;
	m_cameraTransforms.farPlane = 1000.0f;

	COMPUTE_MATH::ComputeMatrix projectionTransform = COMPUTE_MATH::matrixPerspectiveFovLH(45.0f * VT_DEG_TO_RAD,
		1.0f, m_cameraTransforms.nearPlane, m_cameraTransforms.farPlane);

	m_cameraTransforms.m_viewTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(viewTransform);
	m_cameraTransforms.m_projectionTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(projectionTransform);
	m_cameraTransforms.m_invProjectionTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(COMPUTE_MATH::matrixInverse(projectionTransform));

	viewTransform = COMPUTE_MATH::matrixTranspose(viewTransform);
	projectionTransform = COMPUTE_MATH::matrixTranspose(projectionTransform);

	CameraTransforms renderingCameraTransforms = m_cameraTransforms;
	renderingCameraTransforms.m_viewTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(viewTransform);
	renderingCameraTransforms.m_projectionTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(projectionTransform);

	CameraTransforms* mappingCameraTransforms = nullptr;
	m_cameraTransformBuffer->getTypedResource()->mapData(reinterpret_cast<void**>(&mappingCameraTransforms));
	memcpy(mappingCameraTransforms, &renderingCameraTransforms, sizeof(CameraTransforms));
	m_cameraTransformBuffer->getTypedResource()->unmapData();

	device->setResourceName(m_cameraTransformBuffer->getResource(), "CameraTransform");
}
