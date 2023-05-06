#include "GBufferPass.h"

#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"

#include "GraphicDevice/IGraphicDevice.h"

#include "GraphRender/RenderPassEnvironment.h"
#include "GraphRender/RenderPassGraphBuilder.h"

#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"

#include "Render/RenderDrawingContext.h"
#include "RenderContext/GraphicRenderContext.h"

#include "Math/ComputeMath.h"
#include "Math/ComputeMatrix.h"

#include "MeshSystem/IMesh.h"
#include "RenderContext/RenderContextEvent.h"
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

void VT::GBufferPass::fillRenderPassDependency(RenderPassGraphBuilder& builder) const
{
	builder.addRenderPassWriteResource(this, "gb_color_texture", RenderPassEnvironmentResourceType::TEXTURE);
	builder.addRenderPassWriteResource(this, "gb_normal_texture", RenderPassEnvironmentResourceType::TEXTURE);
	builder.addRenderPassWriteResource(this, "gb_position_texture", RenderPassEnvironmentResourceType::TEXTURE);
	builder.addRenderPassWriteResource(this, "gb_depth_texture", RenderPassEnvironmentResourceType::TEXTURE);
}

void VT::GBufferPass::execute(RenderDrawingContext& drawContext, const RenderPassEnvironment& passEnvironment, const IRenderPassData* data)
{
	assert(drawContext.m_context);
	assert(data);

	RenderContextEvent passEvent(drawContext.m_context, "GBufferPass");

	const GBufferRenderPassData& gBufferRenderingData = data->getData<GBufferRenderPassData>();

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
			Scissors(colorTextureDesc.m_width, colorTextureDesc.m_height)
		},
		{
			normalNativeTexture,
			normalRTV.getObject()->getResourceView(),
			Viewport(normalTextureDesc.m_width, normalTextureDesc.m_height),
			Scissors(normalTextureDesc.m_width, normalTextureDesc.m_height)
		},
		{
			positionNativeTexture,
			positionRTV.getObject()->getResourceView(),
			Viewport(positionTextureDesc.m_width, positionTextureDesc.m_height),
			Scissors(positionTextureDesc.m_width, positionTextureDesc.m_height)
		}
	};

	GraphicRenderTargetClearingValue targetClearingValues[] = {
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 0.0f, 1.0f }
	};

	DepthStencilContextTarget depthStencilTarget{
		depthNativeTexture,
		depthDSV.getObject()->getResourceView()
	};

	GraphicRenderContextUtils::setClearingRenderingTargets(
		drawContext.m_context, 3,
		renderTargets, &depthStencilTarget,
		targetClearingValues, 1.0f, 0
	);

	pipelineStateInfo.m_depthStencilTest.m_enabled = true;
	pipelineStateInfo.m_depthStencilTest.m_format = depthTextureDesc.m_format;

	const GBufferRenderPassData::TransformCollection& transforms = gBufferRenderingData.getTransformData();
	const GBufferRenderPassData::MeshCollection& meshes = gBufferRenderingData.getMeshData();
	const size_t meshesCount = meshes.size();

	drawContext.m_context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	drawContext.m_context->setGraphicBindingLayout(m_passData.m_bindingLayout->getTypedObject());

	drawContext.m_context->setGraphicBindingParameterValue(0, 0,
		gBufferRenderingData.getCameraTransformView()->getResourceView()->getBindingHeapOffset());

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

		drawContext.m_context->setGraphicBindingParameterValue(1, 0, m_passData.m_perObjectTransformSRV->getResourceView()->getBindingHeapOffset());
		drawContext.m_context->setGraphicBindingParameterValue(1, 1, static_cast<uint32_t>(meshDataIndex));

		drawContext.m_context->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
		GraphicRenderContextUtils::setPipelineState(drawContext.m_context, pipelineState);
		GraphicRenderContextUtils::setVertexBuffers(drawContext.m_context, static_cast<uint32_t>(vertexData.m_bindings.size()), vertexData.m_bindings.data(), vertexData.m_inputLayout->getDesc());
		GraphicRenderContextUtils::setIndexBuffer(drawContext.m_context, indexData.m_indexBuffer, indexData.m_indexFormat);
		drawContext.m_context->drawIndexed(indexData.m_indexCount);
	}

	m_passData.m_perObjectTransformBuffer->getTypedResource()->unmapData();
}
