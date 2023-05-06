#include "DefaultRender.h"

#include "Core/UtilitiesMacros.h"
#include "GraphRender/RenderPassGraphBuilder.h"

#include "RenderPasses/GBuffer.h"
#include "RenderPasses/LightingVolumeData.h"

#include "RenderPasses/GBufferPass.h"
#include "RenderPasses/LightPrepearingPass.h"
#include "RenderPasses/LigthingPass.h"

bool VT::DefaultRender::RenderPassData::init()
{
	const Vector2UInt16 targetSize = { 512, 512 };

	m_gBuffer = new GBuffer();
	VT_CHECK_INITIALIZATION(m_gBuffer && m_gBuffer->init(targetSize));
	m_lightVolumeData = new LightVolumeData();
	VT_CHECK_INITIALIZATION(m_lightVolumeData && m_lightVolumeData->init(targetSize));

	return true;
}

void VT::DefaultRender::RenderPassData::release()
{
	VT_SAFE_DESTROY_WITH_RELEASING(m_gBuffer);
	VT_SAFE_DESTROY_WITH_RELEASING(m_lightVolumeData);
}

void VT::DefaultRender::RenderPassData::fillRenderPassEnvironment(RenderPassEnvironment& environment)
{
	m_gBuffer->fillEnvironment(environment);
	m_lightVolumeData->fillEnvironment(environment);
}

bool VT::DefaultRender::fillRenderGraphInfo(RenderPassGraphBuilder& builder)
{
	m_passes.m_gBufferPassID = builder.addRenderPass(m_passes.m_gBufferPass);
	m_passes.m_gBufferPass->fillRenderPassDependency(builder);

	m_passes.m_lightPrepearingPassID = builder.addRenderPass(m_passes.m_lightPrepearingPass);
	m_passes.m_lightPrepearingPass->fillRenderPassDependency(builder);

	m_passes.m_lightPassID = builder.addRenderPass(m_passes.m_lightPass);
	m_passes.m_lightPass->fillRenderPassDependency(builder);

	return true;
}

bool VT::DefaultRender::init()
{
	VT_CHECK_INITIALIZATION(initEnvironments(1));

	VT_CHECK_INITIALIZATION(m_renderPassData.init());
	m_renderPassData.fillRenderPassEnvironment(getEnvironment(0));

	VT_CHECK_INITIALIZATION(m_renderingData.init());

	m_passes.m_gBufferPass = new GBufferPass();
	VT_CHECK_INITIALIZATION(m_passes.m_gBufferPass && m_passes.m_gBufferPass->init());
	m_passes.m_lightPrepearingPass = new LightPrepearingPass();
	VT_CHECK_INITIALIZATION(m_passes.m_lightPrepearingPass && m_passes.m_lightPrepearingPass->init());
	m_passes.m_lightPass = new LightPass();
	VT_CHECK_INITIALIZATION(m_passes.m_lightPass && m_passes.m_lightPass->init());

	RenderPassGraphBuilder builder;
	VT_CHECK_INITIALIZATION(fillRenderGraphInfo(builder));
	VT_CHECK_INITIALIZATION(initRenderGraph(builder));

	m_passDatas.m_gBufferPassData = new GBufferRenderPassData(
		m_renderingData.getTransformDataCollection(), m_renderingData.getMeshDataCollection(),
		m_renderingData.getCameraTransformBufferView());
	VT_CHECK_INITIALIZATION(m_passDatas.m_gBufferPassData);

	m_passDatas.m_lightPrepearingPassData = new LightPrepearingRenderPassData(
		m_renderPassData.m_lightVolumeData->getTilesCounts(), m_renderPassData.m_lightVolumeData->getSlicesCount(),
		m_renderingData.getPointLighDataCollection(), m_renderingData.getDirectionalLightDataCollection(),
		m_renderingData.getCameraTransform(), m_renderingData.getCameraTransformBufferView());
	VT_CHECK_INITIALIZATION(m_passDatas.m_lightPrepearingPassData);

	m_passDatas.m_lightRenderPassData = new LightRenderPassData(
		m_renderingData.getDirectionalLightDataCollection(), m_renderingData.getCameraTransformBufferView());
	VT_CHECK_INITIALIZATION(m_passDatas.m_lightRenderPassData);

	addRenderPassData(m_passes.m_gBufferPassID, m_passDatas.m_gBufferPassData);
	addRenderPassData(m_passes.m_lightPrepearingPassID, m_passDatas.m_lightPrepearingPassData);
	addRenderPassData(m_passes.m_lightPassID, m_passDatas.m_lightRenderPassData);

	return true;
}

void VT::DefaultRender::release()
{
	clearRenderGraph();
	releaseEnvironments();

	m_renderPassData.release();

	VT_SAFE_DESTROY_WITH_RELEASING(m_passes.m_gBufferPass);
	VT_SAFE_DESTROY_WITH_RELEASING(m_passes.m_lightPrepearingPass);
	VT_SAFE_DESTROY_WITH_RELEASING(m_passes.m_lightPass);

	VT_SAFE_DESTROY(m_passDatas.m_gBufferPassData);
	VT_SAFE_DESTROY(m_passDatas.m_lightPrepearingPassData);
	VT_SAFE_DESTROY(m_passDatas.m_lightRenderPassData);
}