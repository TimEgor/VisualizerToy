#include "RenderSystem.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicPlatform/IGraphicPlatform.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "RenderSystem/IRenderContext.h"

#include "GraphicPipeline/IPipelineState.h"
#include "GraphicPipeline/IPipelineBindingLayout.h"
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

bool VT::RenderSystem::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicDevice* device = environment->m_graphicDevice;

	ICommandList* commandList = environment->m_graphicDevice->createCommandList();
	VT_CHECK_INITIALIZATION(commandList);

	m_context = environment->m_graphicPlatform->createRenderContext();
	VT_CHECK_INITIALIZATION(m_context && m_context->init(commandList));

	m_frameFence = environment->m_graphicDevice->createFence();
	VT_CHECK_INITIALIZATION(m_frameFence);

	////

	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;
	m_materialDrawingData.m_vertShader = resManager->loadVertexShader("TestInputLayoutVertexShader.hlsl");
	m_materialDrawingData.m_pixelShader = resManager->loadPixelShader("TestInputLayoutPixelShader.hlsl");

	VT_CHECK_INITIALIZATION(m_materialDrawingData.m_vertShader && m_materialDrawingData.m_pixelShader);

	////

	GPUBufferDesc transformBufferDesc{};

	//
	transformBufferDesc.m_byteSize = sizeof(CameraTransforms);
	m_drawingPassData.m_cameraTransformBuffer = resManager->createGPUBuffer(transformBufferDesc);

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
	m_drawingPassData.m_cameraTransformBuffer->getResource()->mapData(reinterpret_cast<void**>(&mappingCameraTransforms));
	memcpy(mappingCameraTransforms, &cameraTransforms, sizeof(CameraTransforms));
	m_drawingPassData.m_cameraTransformBuffer->getResource()->unmapData();

	device->setResourceName(m_drawingPassData.m_cameraTransformBuffer->getResource(), "CameraTransform");

	//
	transformBufferDesc.m_byteSize = sizeof(Matrix44) * 50;
	transformBufferDesc.m_byteStride = sizeof(Matrix44);
	transformBufferDesc.m_flag = GPUBufferFlag::STRUCTURED;
	m_drawingPassData.m_perObjectTransformBuffer = resManager->createGPUBuffer(transformBufferDesc);

	device->setResourceName(m_drawingPassData.m_perObjectTransformBuffer->getResource(), "PerObjectTransform");

	//

	m_drawingPassData.m_cameraTransformCBV = environment->m_graphicDevice->createBufferResourceDescriptor(
		m_drawingPassData.m_cameraTransformBuffer->getResource());
	m_drawingPassData.m_perObjectTransformCBV = environment->m_graphicDevice->createShaderResourceDescriptor(
		m_drawingPassData.m_perObjectTransformBuffer->getResource());

	////

	PipelineBindingLayoutDesc bindingDesc{};
	bindingDesc.m_descriptorBindings.emplace_back(1, 0, 0, ShaderStageVisibility::ALL_STAGES); // Camera transforms
	bindingDesc.m_descriptorBindings.emplace_back(2, 1, 0, ShaderStageVisibility::VERTEX_STAGE); // Object transforms

	m_drawingPassData.m_bindingLayout = resManager->getPipelineBindingLayout(bindingDesc);

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

	PipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_vertexShader = m_materialDrawingData.m_vertShader->getResource();
	pipelineStateInfo.m_pixelShader = m_materialDrawingData.m_pixelShader->getResource();

	const Texture2DDesc& targetDesc = target->getDesc();

	pipelineStateInfo.m_formats.push_back(targetDesc.m_format);

	RenderContextBeginInfo contextInfo{};
	contextInfo.m_targets.push_back({ target, targetView,
		Viewport(targetDesc.m_width, targetDesc.m_height),
		Scissors(targetDesc.m_width, targetDesc.m_height)
	});

	const RenderingData::TransformDataCollection& transforms = m_renderingData.getTransformDataCollection();
	const RenderingData::MeshDataCollection& meshes = m_renderingData.getMeshDataCollection();
	const size_t meshesCount = meshes.size();

	std::vector<IGPUBuffer*> vertBuffers;

	m_context->begin();
	m_context->prepareTextureForRendering(target);
	m_context->beginRendering(contextInfo);

	m_context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	m_context->setBindingLayout(m_drawingPassData.m_bindingLayout->getResource());

	m_context->setBindingParameterValue(0, 0, m_drawingPassData.m_cameraTransformCBV->getBindingHeapOffset());

	Matrix44* mappingObjectTransform = nullptr;
	m_drawingPassData.m_perObjectTransformBuffer->getResource()->mapData(reinterpret_cast<void**>(&mappingObjectTransform));

	for (size_t meshDataIndex = 0; meshDataIndex < meshesCount; ++meshDataIndex)
	{
		const MeshRenderingDataNode& meshData = meshes[meshDataIndex];

		//

		IMesh* mesh = meshData.m_mesh->getMesh();
		if (!mesh)
		{
			continue;
		}

		const MeshVertexData& vertexData = mesh->getVertexData();
		const MeshIndexData& indexData = mesh->getIndexData();

		PipelineStateReference pipelineState = environment->m_graphicResourceManager->getPipelineState(
			pipelineStateInfo, m_drawingPassData.m_bindingLayout, vertexData.m_inputLayout);

		vertBuffers.clear();
		const uint32_t vertBuffersCount = static_cast<uint32_t>(vertexData.m_bindings.size());
		vertBuffers.reserve(vertBuffersCount);

		for (uint32_t i = 0; i < vertBuffersCount; ++i)
		{
			vertBuffers.push_back(vertexData.m_bindings[i]->getResource());
		}

		//

		COMPUTE_MATH::ComputeMatrix objectTransform = COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(transforms[meshData.m_transformIndex]);
		objectTransform = COMPUTE_MATH::matrixTranspose(objectTransform);
		mappingObjectTransform[meshDataIndex] = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(objectTransform);

		//

		m_context->setBindingParameterValue(1, 0, m_drawingPassData.m_perObjectTransformCBV->getBindingHeapOffset());
		m_context->setBindingParameterValue(1, 1, meshDataIndex);

		m_context->setPipelineState(pipelineState->getResource());
		m_context->setVertexBuffers(vertBuffers.size(), vertBuffers.data(), vertexData.m_inputLayout->getDesc());
		m_context->setIndexBuffer(indexData.m_indexBuffer->getResource(), indexData.m_indexFormat);
		m_context->drawIndexed(indexData.m_indexCount);
	}

	m_drawingPassData.m_perObjectTransformBuffer->getResource()->unmapData();

	m_context->endRendering();
	m_context->prepareTextureForPresenting(target);
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