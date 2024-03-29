#include "PresentPass.h"

#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"

#include "MeshSystem/IMeshSystem.h"
#include "MeshSystem/IMesh.h"

#include "Render/RenderDrawingContext.h"
#include "RenderContext/GraphicRenderContext.h"
#include "GraphRender/RenderPassEnvironment.h"

#include "Textures/ITexture2D.h"

#include "Math/Vector.h"

bool VT::PresentPass::initPipelineData()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	PipelineBindingLayoutDesc bindingDesc{};
	bindingDesc.m_descriptorBindings.emplace_back(1, 0, 0, ShaderStageVisibility::PIXEL_STAGE);

	m_pipelineData.m_bindingLayout = resManager->getPipelineBindingLayout(bindingDesc);

	VT_CHECK_RETURN_FALSE(m_pipelineData.m_bindingLayout);

	m_pipelineData.m_presentVertexShader = resManager->loadVertexShader("Shaders/TexturePresenterVertexShader.hlsl");
	m_pipelineData.m_presentPixelShader = resManager->loadPixelShader("Shaders/TexturePresenterPixelShader.hlsl");

	VT_CHECK_RETURN_FALSE(m_pipelineData.m_presentVertexShader);
	VT_CHECK_RETURN_FALSE(m_pipelineData.m_presentPixelShader);

	return true;
}

bool VT::PresentPass::initScreenRect()
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

bool VT::PresentPass::initScreenRectVertexData(MeshVertexData& data, IGraphicResourceManager* resManager) const
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

bool VT::PresentPass::initScreenRectIndexData(MeshIndexData& data, IGraphicResourceManager* resManager) const
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

bool VT::PresentPass::init()
{
	VT_CHECK_INITIALIZATION(initPipelineData());
	VT_CHECK_INITIALIZATION(initScreenRect());

	return true;
}

void VT::PresentPass::release()
{
	m_pipelineData.m_presentVertexShader = nullptr;
	m_pipelineData.m_presentPixelShader = nullptr;
	m_pipelineData.m_bindingLayout = nullptr;

	m_screenRectGeom = nullptr;
}

void VT::PresentPass::execute(RenderDrawingContext& drawContext, const RenderPassEnvironment& passEnvironment, const IRenderPassData* data)
{
	assert(drawContext.m_context);

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	GraphicPipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_vertexShader = m_pipelineData.m_presentVertexShader->getTypedObject();
	pipelineStateInfo.m_pixelShader = m_pipelineData.m_presentPixelShader->getTypedObject();

	const Texture2DDesc& targetTextureDesc = drawContext.m_target->getDesc();
	pipelineStateInfo.m_targetFormats.push_back(targetTextureDesc.m_format);

	GraphicRenderContextTarget targets[] = {
		{
			drawContext.m_target,
			drawContext.m_targetView,
			Viewport(targetTextureDesc.m_width, targetTextureDesc.m_height),
			Scissors(targetTextureDesc.m_width, targetTextureDesc.m_height)
		}
	};

	GraphicRenderTargetClearingValue clearingValue = { 0.0f, 0.0f, 0.0f, 1.0f };

	GraphicRenderContextUtils::setClearingRenderingTargets(drawContext.m_context, 1, targets, &clearingValue);

	IMesh* screenGeom = m_screenRectGeom->getMesh();
	const MeshVertexData& vertexData = screenGeom->getVertexData();
	const MeshIndexData& indexData = screenGeom->getIndexData();

	GraphicPipelineStateReference pipelineState = environment->m_graphicResourceManager->getGraphicPipelineState(
		pipelineStateInfo, m_pipelineData.m_bindingLayout, vertexData.m_inputLayout);

	ShaderResourceViewReference colorSRV = passEnvironment.getShaderResourceView("gb_color_srv");

	drawContext.m_context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	drawContext.m_context->setGraphicBindingLayout(m_pipelineData.m_bindingLayout->getTypedObject());

	drawContext.m_context->setGraphicBindingParameterValue(0, 0, colorSRV->getResourceView()->getBindingHeapOffset());

	drawContext.m_context->setPrimitiveTopology(PrimitiveTopology::TRIANGLELIST);
	GraphicRenderContextUtils::setPipelineState(drawContext.m_context, pipelineState);
	GraphicRenderContextUtils::setVertexBuffers(drawContext.m_context, vertexData.m_bindings.size(),
		vertexData.m_bindings.data(), vertexData.m_inputLayout->getDesc());
	GraphicRenderContextUtils::setIndexBuffer(drawContext.m_context, indexData.m_indexBuffer, indexData.m_indexFormat);
	drawContext.m_context->drawIndexed(indexData.m_indexCount);
}
