#include "GBufferPass.h"

#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"

#include "GraphicDevice/IGraphicDevice.h"

#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "Math/ComputeMath.h"
#include "Math/ComputeMatrix.h"
#include "MeshSystem/IMesh.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"
#include "RenderSystem/GraphicRenderContext.h"
#include "RenderSystem/RenderingData.h"
#include "Textures/ITexture2D.h"

bool VT::GBuffer::init(const Vector2UInt16& bufferResolution)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	Texture2DDesc bufferTextureDesc{};
	bufferTextureDesc.m_format = Format::R32G32B32A32_SFLOAT;
	bufferTextureDesc.m_width = bufferResolution.m_x;
	bufferTextureDesc.m_height = bufferResolution.m_y;
	bufferTextureDesc.m_usage = TEXTURE_USAGE_RENDER_TARGET;

	m_color = resManager->createTexture2D(bufferTextureDesc, RENDER_TARGET);
	m_normal = resManager->createTexture2D(bufferTextureDesc, RENDER_TARGET);
	m_position = resManager->createTexture2D(bufferTextureDesc, RENDER_TARGET);

	//bufferTextureDesc.m_format = Format::D24_UNORM_S8_UINT;
	//bufferTextureDesc.m_usage = TEXTURE_USAGE_DEPTH_STENCIL;
	//m_depth = resManager->createTexture2D(bufferTextureDesc, DEPTH_STENCIL);

	m_colorRTV = resManager->createRenderTargetDescriptor(m_color.getObject());
	m_normalRTV = resManager->createRenderTargetDescriptor(m_normal.getObject());
	m_positionRTV = resManager->createRenderTargetDescriptor(m_position.getObject());
	//m_depthDSV = resManager->createDepthStencilDescriptor(m_depth.getObject());

	m_colorSRV = resManager->createShaderResourceDescriptor(m_color.getObject());
	m_normalSRV = resManager->createShaderResourceDescriptor(m_normal.getObject());
	m_positionSRV = resManager->createShaderResourceDescriptor(m_position.getObject());
	//m_depthSRV = resManager->createShaderResourceDescriptor(m_depth.getObject());

	return true;
}

void VT::GBuffer::release()
{
	m_color = nullptr;
	m_normal = nullptr;
	m_position = nullptr;
	m_depth = nullptr;

	m_colorRTV = nullptr;
	m_normalRTV = nullptr;
	m_positionRTV = nullptr;
	m_depthDSV = nullptr;

	m_colorSRV = nullptr;
	m_normalSRV = nullptr;
	m_positionSRV = nullptr;
	m_depthSRV = nullptr;
}

bool VT::GBufferPass::initRenderingData()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicDevice* device = environment->m_graphicDevice;
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	GPUBufferDesc transformBufferDesc{};

	transformBufferDesc.m_byteSize = sizeof(Matrix44) * 50;
	transformBufferDesc.m_byteStride = sizeof(Matrix44);
	transformBufferDesc.m_flag = GPUBufferFlag::STRUCTURED;
	transformBufferDesc.isHostVisible = true;
	m_passData.m_perObjectTransformBuffer = resManager->createGPUBuffer(transformBufferDesc, CommonGraphicState::COMMON_READ);

	device->setResourceName(m_passData.m_perObjectTransformBuffer->getResource(), "PerObjectTransform");

	m_passData.m_perObjectTransformSRV = resManager->createShaderResourceDescriptor(m_passData.m_perObjectTransformBuffer.getObject());

	////

	PipelineBindingLayoutDesc bindingDesc{};
	bindingDesc.m_descriptorBindings.emplace_back(1, 0, 0, ShaderStageVisibility::ALL_STAGES); // Camera transforms
	bindingDesc.m_descriptorBindings.emplace_back(2, 1, 0, ShaderStageVisibility::VERTEX_STAGE); // Object transforms

	m_passData.m_bindingLayout = resManager->getPipelineBindingLayout(bindingDesc);

	return true;
}

bool VT::GBufferPass::initMaterial()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	m_materialDrawingData.m_vertShader = resManager->loadVertexShader("Shaders/TestVertexShader.hlsl");
	m_materialDrawingData.m_pixelShader = resManager->loadPixelShader("Shaders/TestPixelShader.hlsl");

	return m_materialDrawingData.m_vertShader && m_materialDrawingData.m_pixelShader;
}

bool VT::GBufferPass::init()
{
	VT_CHECK_INITIALIZATION(m_gBuffer.init({500, 500}));
	VT_CHECK_INITIALIZATION(initRenderingData());
	VT_CHECK_INITIALIZATION(initMaterial());
	return true;
}

void VT::GBufferPass::release()
{
	m_gBuffer.release();
}

void VT::GBufferPass::render(const RenderPassContext& passContext)
{
	assert(passContext.context);

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	PipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_vertexShader = m_materialDrawingData.m_vertShader->getTypedObject();
	pipelineStateInfo.m_pixelShader = m_materialDrawingData.m_pixelShader->getTypedObject();

	const Texture2DDesc& targetDesc = m_gBuffer.m_color->getTypedTexture()->getDesc();

	pipelineStateInfo.m_formats.push_back(targetDesc.m_format);
	pipelineStateInfo.m_formats.push_back(targetDesc.m_format);
	pipelineStateInfo.m_formats.push_back(targetDesc.m_format);

	RenderContextTarget targets[] = {
		{
			m_gBuffer.m_color.getObject()->getTypedTexture(),
			m_gBuffer.m_colorRTV.getObject()->getResourceView(),
			Viewport(targetDesc.m_width, targetDesc.m_height),
			Scissors(targetDesc.m_width, targetDesc.m_height),
			{0.0f, 0.0f, 0.0f, 1.0f}
		},
		{
			m_gBuffer.m_normal.getObject()->getTypedTexture(),
			m_gBuffer.m_normalRTV.getObject()->getResourceView(),
			Viewport(targetDesc.m_width, targetDesc.m_height),
			Scissors(targetDesc.m_width, targetDesc.m_height),
			{0.0f, 0.0f, 0.0f, 1.0f}
		},
		{
			m_gBuffer.m_position.getObject()->getTypedTexture(),
			m_gBuffer.m_positionRTV.getObject()->getResourceView(),
			Viewport(targetDesc.m_width, targetDesc.m_height),
			Scissors(targetDesc.m_width, targetDesc.m_height),
			{0.0f, 0.0f, 0.0f, 1.0f}
		}
	};

	const RenderingData::TransformDataCollection& transforms = passContext.m_renderingData.getTransformDataCollection();
	const RenderingData::MeshDataCollection& meshes = passContext.m_renderingData.getMeshDataCollection();
	const size_t meshesCount = meshes.size();

	GraphicRenderContextUtils::setRenderingTargets(passContext.context, 3, targets);

	passContext.context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	passContext.context->setBindingLayout(m_passData.m_bindingLayout->getTypedObject());

	passContext.context->setBindingParameterValue(0, 0, passContext.m_cameraData.m_cameraTransformCBV->getResourceView()->getBindingHeapOffset());

	Matrix44* mappingObjectTransform = nullptr;
	m_passData.m_perObjectTransformBuffer->getTypedResource()->mapData(reinterpret_cast<void**>(&mappingObjectTransform));

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
			pipelineStateInfo, m_passData.m_bindingLayout, vertexData.m_inputLayout);

		//

		COMPUTE_MATH::ComputeMatrix objectTransform = COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(transforms[meshData.m_transformIndex]);
		objectTransform = COMPUTE_MATH::matrixTranspose(objectTransform);
		mappingObjectTransform[meshDataIndex] = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(objectTransform);

		//

		passContext.context->setBindingParameterValue(1, 0, m_passData.m_perObjectTransformSRV->getResourceView()->getBindingHeapOffset());
		passContext.context->setBindingParameterValue(1, 1, meshDataIndex);

		passContext.context->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
		GraphicRenderContextUtils::setPipelineState(passContext.context, pipelineState);
		GraphicRenderContextUtils::setVertexBuffers(passContext.context, vertexData.m_bindings.size(), vertexData.m_bindings.data(), vertexData.m_inputLayout->getDesc());
		GraphicRenderContextUtils::setIndexBuffer(passContext.context, indexData.m_indexBuffer, indexData.m_indexFormat);
		passContext.context->drawIndexed(indexData.m_indexCount);
	}

	m_passData.m_perObjectTransformBuffer->getTypedResource()->unmapData();
}
