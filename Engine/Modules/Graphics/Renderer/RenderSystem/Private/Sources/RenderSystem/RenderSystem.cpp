#include "RenderSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicPlatform/IGraphicPlatform.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "GraphicRenderContext.h"

#include "Textures/ITexture2D.h"
#include "GPUBuffer/IGPUBuffer.h"

#include "Math/Vector.h"
#include "MeshSystem/IMeshSystem.h"
#include "PreparingRenderingDataSystem.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"

#include "Math/ComputeMath.h"
#include "Math/ComputeMatrix.h"
#include "Math/ComputeVector.h"
#include "Math/Consts.h"
#include "RenderPasses/GBuffer.h"

bool VT::RenderSystem::initCameraData()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicDevice* device = environment->m_graphicDevice;
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	GPUBufferDesc transformBufferDesc{};

	transformBufferDesc.m_byteSize = sizeof(CameraTransforms);
	transformBufferDesc.isHostVisible = true;
	m_cameraData.m_cameraTransformBuffer = resManager->createGPUBuffer(transformBufferDesc, CommonGraphicState::COMMON_READ);

	CameraTransforms cameraTransforms;
	COMPUTE_MATH::ComputeMatrix viewTransform = COMPUTE_MATH::matrixLookToLH(
		COMPUTE_MATH::loadComputeVectorFromVector3(Vector3(0.0f, 0.0f, -6.0f)),
		COMPUTE_MATH::loadComputeVectorFromVector3(Vector3UnitZ),
		COMPUTE_MATH::loadComputeVectorFromVector3(Vector3UnitY)
	);
	viewTransform = COMPUTE_MATH::matrixTranspose(viewTransform);

	COMPUTE_MATH::ComputeMatrix projectionTransform = COMPUTE_MATH::matrixPerspectiveFovLH(45.0f * VT_DEG_TO_RAD, 1.0f, 0.1f, 1000.0f);
	projectionTransform = COMPUTE_MATH::matrixTranspose(projectionTransform);

	cameraTransforms.m_viewTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(viewTransform);
	cameraTransforms.m_projectionTransform = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(projectionTransform);

	CameraTransforms* mappingCameraTransforms = nullptr;
	m_cameraData.m_cameraTransformBuffer->getTypedResource()->mapData(reinterpret_cast<void**>(&mappingCameraTransforms));
	memcpy(mappingCameraTransforms, &cameraTransforms, sizeof(CameraTransforms));
	m_cameraData.m_cameraTransformBuffer->getTypedResource()->unmapData();

	device->setResourceName(m_cameraData.m_cameraTransformBuffer->getResource(), "CameraTransform");

	m_cameraData.m_cameraTransformCBV = resManager->createBufferResourceDescriptor(m_cameraData.m_cameraTransformBuffer);

	return true;
}

bool VT::RenderSystem::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	ICommandList* commandList = environment->m_graphicDevice->createCommandList();
	VT_CHECK_INITIALIZATION(commandList);

	m_context = environment->m_graphicPlatform->createRenderContext();
	VT_CHECK_INITIALIZATION(m_context && m_context->init(commandList));

	m_frameFence = environment->m_graphicDevice->createFence();
	VT_CHECK_INITIALIZATION(m_frameFence);

	//
	VT_CHECK_INITIALIZATION(initCameraData());

	//
	m_passEnvironment = new RenderPassEnvironment();
	VT_CHECK_INITIALIZATION(m_passEnvironment);

	m_gBuffer = new GBuffer();
	VT_CHECK_INITIALIZATION(m_gBuffer && m_gBuffer->init({ 500, 500 }));
	m_gBuffer->fillEnvironment(*m_passEnvironment);

	m_gBufferPass = new GBufferPass();
	VT_CHECK_INITIALIZATION(m_gBufferPass && m_gBufferPass->init());

	return true;
}

void VT::RenderSystem::release()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	if (m_frameFence)
	{
		environment->m_graphicDevice->destroyFence(m_frameFence);
		m_frameFence = nullptr;
	}

	VT_SAFE_DESTROY_WITH_RELEASING(m_gBufferPass);
	VT_SAFE_DESTROY_WITH_RELEASING(m_gBuffer);
	VT_SAFE_DESTROY(m_passEnvironment);

	VT_SAFE_DESTROY_WITH_RELEASING(m_context);
}

void VT::RenderSystem::collectRenderingData(const ILevel& level)
{
	PreparingRenderingDataSystem::prepareData(level, m_renderingData);
}

void VT::RenderSystem::render(ITexture2D* target, IGraphicResourceDescriptor* targetView)
{
	assert(m_context);

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	m_context->begin();

	m_gBufferPass->render({ m_context, m_renderingData, m_cameraData }, *m_passEnvironment);

	GraphicRenderContextUtils::prepareTextureResourceForPresenting(m_context, target);
	m_context->end();

	CommandListSubmitInfo submitInfo;
	submitInfo.m_fence = m_frameFence;

	m_lastSubmittedFenceValue = m_frameFence->getValue();
	environment->m_graphicDevice->submitCommandList(m_context->getCommandList(), submitInfo);
}

void VT::RenderSystem::waitFrame()
{
	m_frameFence->wait(m_lastSubmittedFenceValue);
}