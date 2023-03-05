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
#include "RenderSystem/RenderPassEnvironment.h"
#include "Textures/ITexture2D.h"


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
	transformBufferDesc.m_usage = GRAPHIC_USAGE_SHADER_RESOURCE;
	m_passData.m_perObjectTransformBuffer = resManager->createGPUBuffer(transformBufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_COMMON_READ);

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
	VT_CHECK_INITIALIZATION(initRenderingData());
	VT_CHECK_INITIALIZATION(initMaterial());

	return true;
}

void VT::GBufferPass::release()
{
	m_passData.m_perObjectTransformBuffer = nullptr;
	m_passData.m_perObjectTransformSRV = nullptr;
	m_passData.m_bindingLayout = nullptr;

	m_materialDrawingData.m_vertShader = nullptr;
	m_materialDrawingData.m_pixelShader = nullptr;
}

void VT::GBufferPass::execute(const RenderPassContext& passContext, const RenderPassEnvironment& passEnvironment)
{
	assert(passContext.m_context);

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	GraphicPipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_vertexShader = m_materialDrawingData.m_vertShader->getTypedObject();
	pipelineStateInfo.m_pixelShader = m_materialDrawingData.m_pixelShader->getTypedObject();

	TextureReference colorTexture = passEnvironment.getTexture("gb_color_texture");
	TextureReference normalTexture = passEnvironment.getTexture("gb_normal_texture");
	TextureReference positionTexture = passEnvironment.getTexture("gb_position_texture");
	TextureReference depthTexture = passEnvironment.getTexture("gb_depth_texture");

	RenderTargetViewReference colorRTV = passEnvironment.getRenderTargetView("gb_color_rtv");
	RenderTargetViewReference normalRTV = passEnvironment.getRenderTargetView("gb_normal_rtv");
	RenderTargetViewReference positionRTV = passEnvironment.getRenderTargetView("gb_position_rtv");
	DepthStencilViewReference depthDSV = passEnvironment.getDepthStencilView("gb_depth_dsv");

	assert(colorTexture && colorTexture->getTexture()->getDimension() == TextureDimensionType::DIMENSION_2D);
	ITexture2D* colorNativeTexture = colorTexture->getTextureCast<ITexture2D>();
	const Texture2DDesc& colorTextureDesc = colorNativeTexture->getDesc();

	assert(normalTexture && normalTexture->getTexture()->getDimension() == TextureDimensionType::DIMENSION_2D);
	ITexture2D* normalNativeTexture = normalTexture->getTextureCast<ITexture2D>();
	const Texture2DDesc& normalTextureDesc = normalNativeTexture->getDesc();

	assert(positionTexture && positionTexture->getTexture()->getDimension() == TextureDimensionType::DIMENSION_2D);
	ITexture2D* positionNativeTexture = positionTexture->getTextureCast<ITexture2D>();
	const Texture2DDesc& positionTextureDesc = positionNativeTexture->getDesc();

	assert(depthTexture && depthTexture->getTexture()->getDimension() == TextureDimensionType::DIMENSION_2D);
	ITexture2D* depthNativeTexture = depthTexture->getTextureCast<ITexture2D>();
	const Texture2DDesc& depthTextureDesc = depthNativeTexture->getDesc();

	assert(colorRTV && normalRTV && positionRTV && depthDSV);

	pipelineStateInfo.m_targetFormats.push_back(colorTextureDesc.m_format);
	pipelineStateInfo.m_targetFormats.push_back(normalTextureDesc.m_format);
	pipelineStateInfo.m_targetFormats.push_back(positionTextureDesc.m_format);

	GraphicRenderContextTarget renderTargets[] = {
		{
			colorNativeTexture,
			colorRTV.getObject()->getResourceView(),
			Viewport(colorTextureDesc.m_width, colorTextureDesc.m_height),
			Scissors(colorTextureDesc.m_width, colorTextureDesc.m_height),
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		},
		{
			normalNativeTexture,
			normalRTV.getObject()->getResourceView(),
			Viewport(normalTextureDesc.m_width, normalTextureDesc.m_height),
			Scissors(normalTextureDesc.m_width, normalTextureDesc.m_height),
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		},
		{
			positionNativeTexture,
			positionRTV.getObject()->getResourceView(),
			Viewport(positionTextureDesc.m_width, positionTextureDesc.m_height),
			Scissors(positionTextureDesc.m_width, positionTextureDesc.m_height),
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		}
	};

	DepthStencilContextTarget depthStencilTarget{
		depthNativeTexture,
		depthDSV.getObject()->getResourceView(),
		1.0f,
		0
	};

	GraphicRenderContextUtils::setRenderingTargets(passContext.m_context, 3, renderTargets, &depthStencilTarget);

	pipelineStateInfo.m_depthStencilTest.m_enabled = true;
	pipelineStateInfo.m_depthStencilTest.m_format = depthTextureDesc.m_format;

	const RenderingData::TransformDataCollection& transforms = passContext.m_renderingData.getTransformDataCollection();
	const RenderingData::MeshDataCollection& meshes = passContext.m_renderingData.getMeshDataCollection();
	const size_t meshesCount = meshes.size();

	passContext.m_context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	passContext.m_context->setGraphicBindingLayout(m_passData.m_bindingLayout->getTypedObject());

	passContext.m_context->setGraphicBindingParameterValue(0, 0, passContext.m_renderingData.getCameraTransformBufferView()->getResourceView()->getBindingHeapOffset());

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

		GraphicPipelineStateReference pipelineState = environment->m_graphicResourceManager->getGraphicPipelineState(
			pipelineStateInfo, m_passData.m_bindingLayout, vertexData.m_inputLayout);

		//

		COMPUTE_MATH::ComputeMatrix objectTransform = COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(transforms[meshData.m_transformIndex]);
		objectTransform = COMPUTE_MATH::matrixTranspose(objectTransform);
		mappingObjectTransform[meshDataIndex] = COMPUTE_MATH::saveComputeMatrixToMatrix4x4(objectTransform);

		//

		passContext.m_context->setGraphicBindingParameterValue(1, 0, m_passData.m_perObjectTransformSRV->getResourceView()->getBindingHeapOffset());
		passContext.m_context->setGraphicBindingParameterValue(1, 1, meshDataIndex);

		passContext.m_context->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
		GraphicRenderContextUtils::setPipelineState(passContext.m_context, pipelineState);
		GraphicRenderContextUtils::setVertexBuffers(passContext.m_context, vertexData.m_bindings.size(), vertexData.m_bindings.data(), vertexData.m_inputLayout->getDesc());
		GraphicRenderContextUtils::setIndexBuffer(passContext.m_context, indexData.m_indexBuffer, indexData.m_indexFormat);
		passContext.m_context->drawIndexed(indexData.m_indexCount);
	}

	m_passData.m_perObjectTransformBuffer->getTypedResource()->unmapData();
}
