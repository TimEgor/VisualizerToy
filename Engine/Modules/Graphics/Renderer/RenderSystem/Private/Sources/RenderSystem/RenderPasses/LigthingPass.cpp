#include "LigthingPass.h"

#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"

#include "RenderSystem/RenderPassEnvironment.h"
#include "RenderContext/IRenderContext.h"

#include "Textures/ITexture2D.h"

bool VT::LightBuffer::init(const Vector2UInt16& bufferResolution)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	Texture2DDesc bufferTextureDesc{};
	bufferTextureDesc.m_format = Format::R8G8B8A8_SNORM;
	bufferTextureDesc.m_width = bufferResolution.m_x;
	bufferTextureDesc.m_height = bufferResolution.m_y;
	bufferTextureDesc.m_usage = GRAPHIC_USAGE_ALLOW_UNORDERED_ACCESS;

	m_lightTexture = resManager->createTexture2D(bufferTextureDesc, GRAPHIC_STATE_UNORDERED_ACCESS);

	return true;
}

void VT::LightBuffer::release()
{
}

void VT::LightBuffer::fillEnvironment(RenderPassEnvironment& environment) const
{
}

bool VT::LightPass::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	////

	PipelineBindingLayoutDesc bindingDesc{};
	bindingDesc.m_descriptorBindings.emplace_back(2, 0, 0, ShaderStageVisibility::ALL_STAGES);

	m_presenterShader = resManager->loadComputeShader("Shaders/TestComputePresenterShader.hlsl");
	VT_CHECK_INITIALIZATION(m_presenterShader);
	m_bindingLayout = resManager->getPipelineBindingLayout(bindingDesc);
	VT_CHECK_INITIALIZATION(m_bindingLayout);

	return true;
}

void VT::LightPass::release()
{
	m_presenterShader = nullptr;
	m_bindingLayout = nullptr;
}

void VT::LightPass::execute(const RenderPassContext& passContext, const RenderPassEnvironment& passEnvironment)
{
	assert(passContext.m_context);

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	ComputePipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_computeShader = m_presenterShader->getTypedObject();

	ComputePipelineStateReference pipelineState = environment->m_graphicResourceManager->getComputePipelineState(
		pipelineStateInfo, m_bindingLayout);

	ShaderResourceViewReference colorSRV = passEnvironment.getShaderResourceView("gb_color_srv");
	ShaderResourceViewReference lightUAV = passEnvironment.getShaderResourceView("gb_color_srv");

	assert(colorSRV && lightUAV);

	passContext.m_context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	passContext.m_context->setBindingLayout(m_bindingLayout->getTypedObject());

	passContext.m_context->setGraphicBindingParameterValue(0, 0, colorSRV->getResourceView()->getBindingHeapOffset());
	passContext.m_context->setGraphicBindingParameterValue(0, 1, lightUAV->getResourceView()->getBindingHeapOffset());

	passContext.m_context->setPipelineState(pipelineState->getTypedObject());
	passContext.m_context->dispatch(50, 50, 1);
}
