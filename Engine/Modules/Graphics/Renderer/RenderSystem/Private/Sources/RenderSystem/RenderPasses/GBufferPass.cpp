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
	VT_CHECK_INITIALIZATION(initRenderingData());
	VT_CHECK_INITIALIZATION(initMaterial());

	return true;
}

void VT::GBufferPass::release()
{
}

void VT::GBufferPass::render(const RenderPassContext& passContext, const RenderPassEnvironment& passEnvironment)
{
	assert(passContext.context);

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	PipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_vertexShader = m_materialDrawingData.m_vertShader->getTypedObject();
	pipelineStateInfo.m_pixelShader = m_materialDrawingData.m_pixelShader->getTypedObject();

	TextureReference colorTexture = passEnvironment.getTexture("gb_color_texture");
	TextureReference normalTexture = passEnvironment.getTexture("gb_normal_texture");
	TextureReference positionTexture = passEnvironment.getTexture("gb_position_texture");

	RenderTargetViewReference colorRTV = passEnvironment.getRenderTargetView("gb_color_rtv");
	RenderTargetViewReference normalRTV = passEnvironment.getRenderTargetView("gb_normal_rtv");
	RenderTargetViewReference positionRTV = passEnvironment.getRenderTargetView("gb_position_rtv");

	assert(colorTexture && colorTexture->getTexture()->getDimension() == TextureDimensionType::DIMENSION_2D);
	ITexture2D* colorNativeTexture = colorTexture->getTextureCast<ITexture2D>();
	const Texture2DDesc& colorTextureDesc = colorNativeTexture->getDesc();

	assert(normalTexture && normalTexture->getTexture()->getDimension() == TextureDimensionType::DIMENSION_2D);
	ITexture2D* normalNativeTexture = normalTexture->getTextureCast<ITexture2D>();
	const Texture2DDesc& normalTextureDesc = normalNativeTexture->getDesc();

	assert(positionTexture && positionTexture->getTexture()->getDimension() == TextureDimensionType::DIMENSION_2D);
	ITexture2D* positionNativeTexture = positionTexture->getTextureCast<ITexture2D>();
	const Texture2DDesc& positionTextureDesc = positionNativeTexture->getDesc();

	assert(colorRTV && normalRTV && positionRTV);

	pipelineStateInfo.m_formats.push_back(colorTextureDesc.m_format);
	pipelineStateInfo.m_formats.push_back(normalTextureDesc.m_format);
	pipelineStateInfo.m_formats.push_back(positionTextureDesc.m_format);

	GraphicRenderContextTarget targets[] = {
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

	const RenderingData::TransformDataCollection& transforms = passContext.m_renderingData.getTransformDataCollection();
	const RenderingData::MeshDataCollection& meshes = passContext.m_renderingData.getMeshDataCollection();
	const size_t meshesCount = meshes.size();

	GraphicRenderContextUtils::setRenderingTargets(passContext.context, 3, targets);

	passContext.context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	passContext.context->setBindingLayout(m_passData.m_bindingLayout->getTypedObject());

	passContext.context->setBindingParameterValue(0, 0, passContext.m_renderingData.getCameraTransformBufferView()->getResourceView()->getBindingHeapOffset());

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
