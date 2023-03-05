#include "LigthingPass.h"

#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"

#include "MeshSystem/IMeshSystem.h"
#include "MeshSystem/IMesh.h"

#include "RenderSystem/GraphicRenderContext.h"
#include "RenderSystem/RenderPassEnvironment.h"

#include "Textures/ITexture2D.h"

#include "Math/Vector.h"
#include "RenderSystem/RenderingData.h"

bool VT::LightPass::initPipelineData()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	PipelineBindingLayoutDesc bindingDesc{};
	bindingDesc.m_descriptorBindings.emplace_back(1, 0, 0, ShaderStageVisibility::PIXEL_STAGE);
	bindingDesc.m_descriptorBindings.emplace_back(1, 1, 0, ShaderStageVisibility::PIXEL_STAGE);
	bindingDesc.m_descriptorBindings.emplace_back(3, 2, 0, ShaderStageVisibility::PIXEL_STAGE);
	bindingDesc.m_descriptorBindings.emplace_back(3, 3, 0, ShaderStageVisibility::PIXEL_STAGE);

	m_pipelineData.m_bindingLayout = resManager->getPipelineBindingLayout(bindingDesc);

	VT_CHECK_RETURN_FALSE(m_pipelineData.m_bindingLayout);

	m_pipelineData.m_presentVertexShader = resManager->loadVertexShader("Shaders/LightPresentingVertexShader.hlsl");
	m_pipelineData.m_presentPixelShader = resManager->loadPixelShader("Shaders/LightPresentingPixelShader.hlsl");

	VT_CHECK_RETURN_FALSE(m_pipelineData.m_presentVertexShader);
	VT_CHECK_RETURN_FALSE(m_pipelineData.m_presentPixelShader);

	return true;
}

bool VT::LightPass::initScreenRect()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	IMeshSystem* meshSystem = environment->m_meshSystem;
	MeshVertexData vertexData;
	MeshIndexData indexData;

	VT_CHECK_RETURN_FALSE(initScreenRectVertexData(vertexData, resManager));
	VT_CHECK_RETURN_FALSE(initScreenRectIndexData(indexData, resManager));

	m_screenRectGeom = meshSystem->createMesh(vertexData, indexData);

	return m_screenRectGeom;
}

bool VT::LightPass::initScreenRectVertexData(MeshVertexData& data, IGraphicResourceManager* resManager) const
{
	InputLayoutDesc inputLayoutDesc{};
	inputLayoutDesc.m_elements = {
		{ "POSITION", 0, 0, 0, 2, InputLayoutElementType::FLOAT32 },
		{ "TEXTURE", 0, 1, 0, 2, InputLayoutElementType::FLOAT32 }
	};
	inputLayoutDesc.m_bindings = {
		{ 0, INPUT_LAYOUT_ELEMENT_TYPE_FLOAT32_SIZE * 2 },
		{ 1, INPUT_LAYOUT_ELEMENT_TYPE_FLOAT32_SIZE * 2 }
	};

	data.m_inputLayout = resManager->addInputLayout(inputLayoutDesc);
	VT_CHECK_RETURN_FALSE(data.m_inputLayout);

	{
		const Vector2 vertecies[] = {
			{-1.0f, -1.0f},
			{1.0f, -1.0f},
			{-1.0f, 1.0f},
			{1.0f, 1.0f}
		};

		InitialGPUBufferData bufferData{ vertecies, sizeof(vertecies) };

		GPUBufferDesc gpuBufferDesc{};
		gpuBufferDesc.m_byteSize = sizeof(vertecies);
		gpuBufferDesc.m_byteStride = sizeof(Vector2);

		GPUBufferReference posVertBuffer = resManager->createGPUBuffer(gpuBufferDesc, GPU_BUFFER_STATE_VERTEX_BUFFER, &bufferData);
		VT_CHECK_RETURN_FALSE(posVertBuffer);

		data.m_bindings.push_back(posVertBuffer);
	}

	{
		const Vector2 UVs[] = {
			{0.0f, 1.0f},
			{1.0f, 1.0f},
			{0.0f, 0.0f},
			{1.0f, 0.0f}
		};

		InitialGPUBufferData bufferData{ UVs, sizeof(UVs) };

		GPUBufferDesc gpuBufferDesc{};
		gpuBufferDesc.m_byteSize = sizeof(UVs);
		gpuBufferDesc.m_byteStride = sizeof(Vector2);

		GPUBufferReference uvVertBuffer = resManager->createGPUBuffer(gpuBufferDesc, GPU_BUFFER_STATE_VERTEX_BUFFER, &bufferData);
		VT_CHECK_RETURN_FALSE(uvVertBuffer);

		data.m_bindings.push_back(uvVertBuffer);
	}

	data.m_vertexCount = 4;

	return true;
}

bool VT::LightPass::initScreenRectIndexData(MeshIndexData& data, IGraphicResourceManager* resManager) const
{
	const uint16_t indices[] = {
		0, 2, 1,
		1, 2, 3
	};

	InitialGPUBufferData bufferData{ indices, sizeof(indices) };

	GPUBufferDesc gpuBufferDesc{};
	gpuBufferDesc.m_byteSize = sizeof(indices);
	gpuBufferDesc.m_byteStride = sizeof(uint16_t);

	data.m_indexBuffer = resManager->createGPUBuffer(gpuBufferDesc, GPU_BUFFER_STATE_INDEX_BUFFER, &bufferData);
	VT_CHECK_RETURN_FALSE(data.m_indexBuffer);

	data.m_indexCount = 6;
	data.m_indexFormat = InputLayoutElementType::UINT16;

	return true;
}

bool VT::LightPass::init()
{
	VT_CHECK_INITIALIZATION(initPipelineData());
	VT_CHECK_INITIALIZATION(initScreenRect());

	return true;
}

void VT::LightPass::release()
{
	m_pipelineData.m_presentVertexShader = nullptr;
	m_pipelineData.m_presentPixelShader = nullptr;
	m_pipelineData.m_bindingLayout = nullptr;

	m_screenRectGeom = nullptr;
}

void VT::LightPass::execute(const RenderPassContext& passContext, const RenderPassEnvironment& passEnvironment)
{
	assert(passContext.m_context);

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	GraphicPipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_vertexShader = m_pipelineData.m_presentVertexShader->getTypedObject();
	pipelineStateInfo.m_pixelShader = m_pipelineData.m_presentPixelShader->getTypedObject();

	const Texture2DDesc& targetTextureDesc = passContext.m_target->getDesc();
	pipelineStateInfo.m_targetFormats.push_back(targetTextureDesc.m_format);

	GraphicRenderContextTarget targets[] = {
		{
			passContext.m_target,
			passContext.m_targetView,
			Viewport(targetTextureDesc.m_width, targetTextureDesc.m_height),
			Scissors(targetTextureDesc.m_width, targetTextureDesc.m_height),
			{ 0.0f, 0.0f, 0.0f, 1.0f }
		}
	};

	GraphicRenderContextUtils::setRenderingTargets(passContext.m_context, 1, targets, nullptr);

	IMesh* screenGeom = m_screenRectGeom->getMesh();
	const MeshVertexData& vertexData = screenGeom->getVertexData();
	const MeshIndexData& indexData = screenGeom->getIndexData();

	GraphicPipelineStateReference pipelineState = environment->m_graphicResourceManager->getGraphicPipelineState(
		pipelineStateInfo, m_pipelineData.m_bindingLayout, vertexData.m_inputLayout);

	ShaderResourceViewReference colorSRV = passEnvironment.getShaderResourceView("gb_color_srv");
	ShaderResourceViewReference normalSRV = passEnvironment.getShaderResourceView("gb_normal_srv");
	ShaderResourceViewReference positionSRV = passEnvironment.getShaderResourceView("gb_position_srv");

	ShaderResourceViewReference pointLightsSRV = passEnvironment.getShaderResourceView("lv_point_light_srv");
	ShaderResourceViewReference tilesSRV = passEnvironment.getShaderResourceView("lv_point_light_tile_mask_srv");
	ShaderResourceViewReference lightVolumeSRV = passEnvironment.getShaderResourceView("lv_info_srv");

	passContext.m_context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	passContext.m_context->setGraphicBindingLayout(m_pipelineData.m_bindingLayout->getTypedObject());

	passContext.m_context->setGraphicBindingParameterValue(0, 0, passContext.m_renderingData.getCameraTransformBufferView()->getResourceView()->getBindingHeapOffset());
	passContext.m_context->setGraphicBindingParameterValue(1, 0, lightVolumeSRV->getResourceView()->getBindingHeapOffset());

	passContext.m_context->setGraphicBindingParameterValue(2, 0, colorSRV->getResourceView()->getBindingHeapOffset());
	passContext.m_context->setGraphicBindingParameterValue(2, 1, normalSRV->getResourceView()->getBindingHeapOffset());
	passContext.m_context->setGraphicBindingParameterValue(2, 2, positionSRV->getResourceView()->getBindingHeapOffset());

	passContext.m_context->setGraphicBindingParameterValue(3, 0, pointLightsSRV->getResourceView()->getBindingHeapOffset());
	passContext.m_context->setGraphicBindingParameterValue(3, 1, tilesSRV->getResourceView()->getBindingHeapOffset());
	passContext.m_context->setGraphicBindingParameterValue(3, 2, lightVolumeSRV->getResourceView()->getBindingHeapOffset());

	passContext.m_context->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	GraphicRenderContextUtils::setPipelineState(passContext.m_context, pipelineState);
	GraphicRenderContextUtils::setVertexBuffers(passContext.m_context, vertexData.m_bindings.size(),
		vertexData.m_bindings.data(), vertexData.m_inputLayout->getDesc());
	GraphicRenderContextUtils::setIndexBuffer(passContext.m_context, indexData.m_indexBuffer, indexData.m_indexFormat);
	passContext.m_context->drawIndexed(indexData.m_indexCount);
}
