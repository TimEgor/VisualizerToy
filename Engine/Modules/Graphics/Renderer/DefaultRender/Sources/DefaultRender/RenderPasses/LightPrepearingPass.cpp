#include "LightPrepearingPass.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "RenderContext/IRenderContext.h"
#include "RenderContext/RenderContextEvent.h"
#include "Render/RenderDrawingContext.h"

#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"

#include "GraphRender/RenderPassEnvironment.h"
#include "GraphRender/RenderPassGraphBuilder.h"
#include "DefaultRender/LightSources/PointLightData.h"
#include "LightingVolumeData.h"

#include "Math/ComputeMatrix.h"
#include "Math/ComputeVector.h"

#include "Textures/ITexture2D.h"

// need to be sync with val in the shaders
static constexpr uint32_t POINT_LIGHT_CULLING_THREAD_GROUP_SIZE = 16;
static constexpr uint32_t POINT_LIGHT_MASK_CLEARING_THREAD_GROUP_SIZE = 8;
static constexpr uint32_t TILES_BOUNDING_BOXES_CALC_THREAD_GROUP_SIZE = 8;

bool VT::LightPrepearingPass::initPointLightMaskClearingData(IGraphicResourceManager* resManager)
{
	PipelineBindingLayoutDesc bindingDesc{};
	bindingDesc.m_descriptorBindings.emplace_back(1, 0, 0, ShaderStageVisibility::ALL_STAGES); // LightVolumeInfo
	bindingDesc.m_descriptorBindings.emplace_back(1, 1, 0, ShaderStageVisibility::ALL_STAGES); // PointLightTileMasks

	m_pointLightMaskClearingShader = resManager->loadComputeShader("Shaders/Lighting/ClearPointLightMasks.hlsl");
	VT_CHECK_RETURN_FALSE(m_pointLightMaskClearingShader);
	m_pointLightMaskClearingBindingLayout = resManager->getPipelineBindingLayout(bindingDesc);
	VT_CHECK_RETURN_FALSE(m_pointLightMaskClearingBindingLayout);

	return true;
}

bool VT::LightPrepearingPass::initPointLightCullingData(IGraphicResourceManager* resManager)
{
	PipelineBindingLayoutDesc bindingDesc{};
	bindingDesc.m_descriptorBindings.emplace_back(1, 0, 0, ShaderStageVisibility::ALL_STAGES); // CameraTransform
	bindingDesc.m_descriptorBindings.emplace_back(1, 1, 0, ShaderStageVisibility::ALL_STAGES); // LightVolumeInfo
	bindingDesc.m_descriptorBindings.emplace_back(1, 2, 0, ShaderStageVisibility::ALL_STAGES); // LightVolumeBoundingBoxes
	bindingDesc.m_descriptorBindings.emplace_back(3, 3, 0, ShaderStageVisibility::ALL_STAGES); // PointLight bindings

	m_pointLightCullingShader = resManager->loadComputeShader("Shaders/Lighting/PointLightCulling.hlsl");
	VT_CHECK_RETURN_FALSE(m_pointLightCullingShader);
	m_pointLightCullingBindingLayout = resManager->getPipelineBindingLayout(bindingDesc);
	VT_CHECK_RETURN_FALSE(m_pointLightCullingBindingLayout);

	return true;
}

bool VT::LightPrepearingPass::initDepthCalcData(IGraphicResourceManager* resManager)
{
	PipelineBindingLayoutDesc bindingDesc{};
	bindingDesc.m_descriptorBindings.emplace_back(1, 0, 0, ShaderStageVisibility::ALL_STAGES); //DepthSource
	bindingDesc.m_descriptorBindings.emplace_back(1, 1, 0, ShaderStageVisibility::ALL_STAGES); //LightVolumeInfo
	bindingDesc.m_descriptorBindings.emplace_back(1, 2, 0, ShaderStageVisibility::ALL_STAGES); //LightVolumeDepth

	m_lightVolumeDepthCalcShader = resManager->loadComputeShader("Shaders/Lighting/TilesDepthCalc.hlsl");
	VT_CHECK_RETURN_FALSE(m_lightVolumeDepthCalcShader);
	m_lightVolumeDepthCalcBindingLayout = resManager->getPipelineBindingLayout(bindingDesc);
	VT_CHECK_RETURN_FALSE(m_lightVolumeDepthCalcBindingLayout);

	return true;
}

bool VT::LightPrepearingPass::initBoundingBoxCalcData(IGraphicResourceManager* resManager)
{
	PipelineBindingLayoutDesc bindingDesc{};
	bindingDesc.m_descriptorBindings.emplace_back(1, 0, 0, ShaderStageVisibility::ALL_STAGES); // CameraTransform
	bindingDesc.m_descriptorBindings.emplace_back(1, 1, 0, ShaderStageVisibility::ALL_STAGES); // LightVolumeInfo
	bindingDesc.m_descriptorBindings.emplace_back(1, 2, 0, ShaderStageVisibility::ALL_STAGES); // LightVolumeBoundingBoxes
	bindingDesc.m_descriptorBindings.emplace_back(1, 3, 0, ShaderStageVisibility::ALL_STAGES); // LightVolumeDepth

	m_lightVolumeBBCalcShader = resManager->loadComputeShader("Shaders/Lighting/TilesBBCalc.hlsl");
	VT_CHECK_RETURN_FALSE(m_lightVolumeBBCalcShader);
	m_lightVolumeBBCalcBindingLayout = resManager->getPipelineBindingLayout(bindingDesc);
	VT_CHECK_RETURN_FALSE(m_lightVolumeBBCalcBindingLayout);

	return true;
}

void VT::LightPrepearingPass::sortPointLights(const LightPrepearingRenderPassData::PointLightCollection& data, const CameraTransforms& cameraTransform)
{
	COMPUTE_MATH::ComputeMatrix viewTransformMatrix = COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(cameraTransform.m_viewTransform);
	COMPUTE_MATH::ComputeVector resultPosition;

	const uint32_t lightsCount = static_cast<uint32_t>(data.size());
	m_sortedPointLights.clear();
	m_sortedPointLights.reserve(lightsCount);

	const float zDelta = cameraTransform.farPlane - cameraTransform.nearPlane;

	for (uint32_t lightIndex = 0; lightIndex < lightsCount; ++lightIndex)
	{
		const PointLightData& light = data[lightIndex];

		resultPosition = COMPUTE_MATH::loadComputeVectorFromVector3(light.m_position);
		resultPosition = COMPUTE_MATH::transformPoint(resultPosition, viewTransformMatrix);

		Vector3 viewPosition = COMPUTE_MATH::saveComputeVectorToVector3(resultPosition);

		SortedPointLight& sortedLight = m_sortedPointLights.emplace_back();
		sortedLight.m_lightIndex = lightIndex;
		sortedLight.m_projPosZ = (viewPosition.m_z - cameraTransform.nearPlane) / zDelta;
		sortedLight.m_projMaxPosZ = (viewPosition.m_z + light.m_radius - cameraTransform.nearPlane) / zDelta;
		sortedLight.m_projMinPosZ = (viewPosition.m_z - light.m_radius - cameraTransform.nearPlane) / zDelta;
	}

	qsort(m_sortedPointLights.data(), lightsCount, sizeof(SortedPointLight),
		[](const void* l1, const void* l2)
		{
			const SortedPointLight* light1 = reinterpret_cast<const SortedPointLight*>(l1);
			const SortedPointLight* light2 = reinterpret_cast<const SortedPointLight*>(l2);

			if (light1->m_projPosZ < light2->m_projPosZ)
			{
				return -1;
			}
			if (light1->m_projPosZ > light2->m_projPosZ)
			{
				return 1;
			}

			return 0;
		});
}

void VT::LightPrepearingPass::fillPointLightBuffer(IGPUBuffer* pointLightBuffer,
	const LightPrepearingRenderPassData::PointLightCollection& lightData)
{
	const uint32_t lightsCount = static_cast<uint32_t>(m_sortedPointLights.size());
	if (lightsCount == 0)
	{
		return;
	}

	PointLightData* mappedLights = nullptr;
	pointLightBuffer->mapData(reinterpret_cast<void**>(&mappedLights));

	for (uint32_t i = 0; i < lightsCount; ++i)
	{
		// TODO: need to improve this mapping
		const PointLightData* light = &lightData[m_sortedPointLights[i].m_lightIndex];
		memcpy(&mappedLights[i], light, sizeof(PointLightData));
	}

	pointLightBuffer->unmapData();
}

void VT::LightPrepearingPass::fillPointLightZSliceBuffer(IGPUBuffer* zSliceBuffer, uint16_t slicesCount)
{
	const float sliceDepth = 1.0f / slicesCount;

	LightVolumeData::ZSlice* slicesMappedData = nullptr;
	zSliceBuffer->mapData(reinterpret_cast<void**>(&slicesMappedData));

	const uint16_t lightCountLimit = std::min(static_cast<uint32_t>(m_sortedPointLights.size()), MAX_POINT_LIGHT_NUM);

	for (uint16_t sliceIndex = 0; sliceIndex < slicesCount; ++sliceIndex) {
		uint16_t minSliceLightIndex = MAX_POINT_LIGHT_NUM;
		uint16_t maxSliceLightIndex = 0;

		const float binMinPos = sliceDepth * sliceIndex;
		const float binMaxPos = binMinPos + sliceDepth;

		for (uint16_t i = 0; i < lightCountLimit; ++i)
		{
			const SortedPointLight& light = m_sortedPointLights[i];

			if ((light.m_projPosZ >= binMinPos && light.m_projPosZ <= binMaxPos) ||
				(light.m_projMinPosZ >= binMinPos && light.m_projMinPosZ <= binMaxPos) ||
				(light.m_projMaxPosZ >= binMinPos && light.m_projMaxPosZ <= binMaxPos))
			{
				if (i < minSliceLightIndex)
				{
					minSliceLightIndex = i;
				}

				if (i > maxSliceLightIndex)
				{
					maxSliceLightIndex = i;
				}
			}
		}

		LightVolumeData::ZSlice& slice = slicesMappedData[sliceIndex];
		slice.m_minLightIndex = minSliceLightIndex;
		slice.m_maxLightIndex = maxSliceLightIndex;
	}

	zSliceBuffer->unmapData();
}

void VT::LightPrepearingPass::fillDirLightBuffer(IGPUBuffer* dirLightBuffer,
	const LightPrepearingRenderPassData::DirectionalLightCollection& lightData)
{
	const uint32_t lightsCount = static_cast<uint32_t>(lightData.size());
	if (lightsCount == 0)
	{
		return;
	}

	PointLightData* mappedLights = nullptr;
	dirLightBuffer->mapData(reinterpret_cast<void**>(&mappedLights));
	memcpy(mappedLights, lightData.data(), sizeof(DirectionalLightData) * lightsCount);
	dirLightBuffer->unmapData();
}

void VT::LightPrepearingPass::computeTilesDepth(IRenderContext* context, ShaderResourceViewReference tilesDepthUAV,
	ShaderResourceViewReference lightVolumeSRV, ShaderResourceViewReference depthSourceSRV, ITexture2D* depthSource)
{
	RenderContextEvent passEvent(context, "TilesDepthsComputation");

	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	ComputePipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_computeShader = m_lightVolumeDepthCalcShader->getTypedObject();

	ComputePipelineStateReference pipelineState = environment->m_graphicResourceManager->
		getComputePipelineState(pipelineStateInfo, m_lightVolumeDepthCalcBindingLayout);

	context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	context->setComputeBindingLayout(m_lightVolumeDepthCalcBindingLayout->getTypedObject());

	context->setComputeBindingParameterValue(0, 0, depthSourceSRV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(1, 0, lightVolumeSRV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(2, 0, tilesDepthUAV->getResourceView()->getBindingHeapOffset());

	context->setPipelineState(pipelineState->getTypedObject());

	const Texture2DDesc& depthSourceDesc = depthSource->getDesc();

	const uint32_t threadGroupCountX = depthSourceDesc.m_width / LIGHT_VOLUME_TILE_DIM_SIZE;
	const uint32_t threadGroupCountY = depthSourceDesc.m_height / LIGHT_VOLUME_TILE_DIM_SIZE;
	context->dispatch(threadGroupCountX, threadGroupCountY, 1);
}

void VT::LightPrepearingPass::computeTilesBoudingBoxes(IRenderContext* context, ShaderResourceViewReference tilesBbUAV,
	ShaderResourceViewReference tilesDepthSRV, ShaderResourceViewReference cameraTransformsSRV,
	ShaderResourceViewReference lightVolumeSRV, const Vector2UInt16& tilesCount)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	RenderContextEvent passEvent(context, "TilesBBsComputation");

	ComputePipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_computeShader = m_lightVolumeBBCalcShader->getTypedObject();

	ComputePipelineStateReference pipelineState = environment->m_graphicResourceManager->
		getComputePipelineState(pipelineStateInfo, m_lightVolumeBBCalcBindingLayout);

	context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	context->setComputeBindingLayout(m_lightVolumeBBCalcBindingLayout->getTypedObject());

	context->setComputeBindingParameterValue(0, 0, cameraTransformsSRV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(1, 0, lightVolumeSRV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(2, 0, tilesBbUAV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(3, 0, tilesDepthSRV->getResourceView()->getBindingHeapOffset());

	context->setPipelineState(pipelineState->getTypedObject());

	const uint32_t threadGroupCountX = (tilesCount.m_x + TILES_BOUNDING_BOXES_CALC_THREAD_GROUP_SIZE - 1)
		/ TILES_BOUNDING_BOXES_CALC_THREAD_GROUP_SIZE;
	const uint32_t threadGroupCountY = (tilesCount.m_y + TILES_BOUNDING_BOXES_CALC_THREAD_GROUP_SIZE - 1)
		/ TILES_BOUNDING_BOXES_CALC_THREAD_GROUP_SIZE;
	context->dispatch(threadGroupCountX, threadGroupCountY, 1);
}

void VT::LightPrepearingPass::clearPointLightTileMasks(IRenderContext* context, ShaderResourceViewReference tilesUAV,
	ShaderResourceViewReference lightVolumeSRV, const Vector2UInt16& tilesCount)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	RenderContextEvent passEvent(context, "PointLightTileMasksClearing");

	ComputePipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_computeShader = m_pointLightMaskClearingShader->getTypedObject();

	ComputePipelineStateReference pipelineState = environment->m_graphicResourceManager->
		getComputePipelineState(pipelineStateInfo, m_pointLightMaskClearingBindingLayout);

	context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	context->setComputeBindingLayout(m_pointLightMaskClearingBindingLayout->getTypedObject());

	context->setComputeBindingParameterValue(0, 0, lightVolumeSRV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(1, 0, tilesUAV->getResourceView()->getBindingHeapOffset());

	context->setPipelineState(pipelineState->getTypedObject());

	const uint32_t threadGroupCountX = (tilesCount.m_x + POINT_LIGHT_MASK_CLEARING_THREAD_GROUP_SIZE - 1)
		/ POINT_LIGHT_MASK_CLEARING_THREAD_GROUP_SIZE;
	const uint32_t threadGroupCountY = (tilesCount.m_y + POINT_LIGHT_MASK_CLEARING_THREAD_GROUP_SIZE - 1)
		/ POINT_LIGHT_MASK_CLEARING_THREAD_GROUP_SIZE;
	context->dispatch(threadGroupCountX, threadGroupCountY, 1);
}

void VT::LightPrepearingPass::computePointLightTileMasks(IRenderContext* context, uint32_t pointLightsCount,
	ShaderResourceViewReference pointLightsSRV, ShaderResourceViewReference tilesUAV, ShaderResourceViewReference tilesBbSRV,
	ShaderResourceViewReference cameraTransformsSRV, ShaderResourceViewReference lightVolumeSRV)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	RenderContextEvent passEvent(context, "PointLightTileMasksComputation");

	ComputePipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_computeShader = m_pointLightCullingShader->getTypedObject();

	ComputePipelineStateReference pipelineState = environment->m_graphicResourceManager->
		getComputePipelineState(pipelineStateInfo, m_pointLightCullingBindingLayout);

	context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	context->setComputeBindingLayout(m_pointLightCullingBindingLayout->getTypedObject());

	context->setComputeBindingParameterValue(0, 0, cameraTransformsSRV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(1, 0, lightVolumeSRV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(2, 0, tilesBbSRV->getResourceView()->getBindingHeapOffset());

	context->setComputeBindingParameterValue(3, 0, pointLightsCount);
	context->setComputeBindingParameterValue(3, 1, pointLightsSRV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(3, 2, tilesUAV->getResourceView()->getBindingHeapOffset());

	context->setPipelineState(pipelineState->getTypedObject());

	const uint32_t threadGroupCountX = (pointLightsCount + POINT_LIGHT_CULLING_THREAD_GROUP_SIZE - 1) / POINT_LIGHT_CULLING_THREAD_GROUP_SIZE;
	context->dispatch(threadGroupCountX, 1, 1);
}

bool VT::LightPrepearingPass::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	VT_CHECK_INITIALIZATION(initPointLightMaskClearingData(resManager));
	VT_CHECK_INITIALIZATION(initPointLightCullingData(resManager));

	VT_CHECK_INITIALIZATION(initDepthCalcData(resManager));
	VT_CHECK_INITIALIZATION(initBoundingBoxCalcData(resManager));

	return true;
}

void VT::LightPrepearingPass::release()
{
	m_pointLightMaskClearingShader = nullptr;
	m_pointLightCullingShader = nullptr;

	m_lightVolumeDepthCalcShader = nullptr;
	m_lightVolumeBBCalcShader = nullptr;

	m_pointLightMaskClearingBindingLayout = nullptr;
	m_pointLightCullingBindingLayout = nullptr;

	m_lightVolumeDepthCalcBindingLayout = nullptr;
	m_lightVolumeBBCalcBindingLayout = nullptr;
}

void VT::LightPrepearingPass::fillRenderPassDependency(RenderPassGraphBuilder& builder) const
{
	builder.addRenderPassWriteResource(this, "lv_point_light_buffer", RenderPassEnvironmentResourceType::BUFFER);
	builder.addRenderPassWriteResource(this, "lv_point_light_zslice_buffer", RenderPassEnvironmentResourceType::BUFFER);

	builder.addRenderPassWriteResource(this, "lv_dir_light_buffer", RenderPassEnvironmentResourceType::BUFFER);

	builder.addRenderPassReadResource(this, "gb_depth_texture", RenderPassEnvironmentResourceType::TEXTURE);
}

void VT::LightPrepearingPass::execute(RenderDrawingContext& drawContext, const RenderPassEnvironment& passEnvironment, const IRenderPassData* data)
{
	assert(data);

	RenderContextEvent passEvent(drawContext.m_context, "LightingPreparePass");

	const LightPrepearingRenderPassData& lightPrepearingData = data->getData<LightPrepearingRenderPassData>();

	const LightPrepearingRenderPassData::PointLightCollection& pointLightCollection = lightPrepearingData.getPointLightData();
	if (pointLightCollection.empty())
	{
		return;
	}

	GPUBufferReference pointLightBuffer = passEnvironment.getBuffer("lv_point_light_buffer");
	GPUBufferReference zSliceBuffer = passEnvironment.getBuffer("lv_point_light_zslice_buffer");

	GPUBufferReference dirLightBuffer = passEnvironment.getBuffer("lv_dir_light_buffer");

	ShaderResourceViewReference pointLightsSRV = passEnvironment.getShaderResourceView("lv_point_light_srv");

	ShaderResourceViewReference tilesUAV = passEnvironment.getShaderResourceView("lv_point_light_tile_mask_uav");
	ShaderResourceViewReference lightVolumeSRV = passEnvironment.getShaderResourceView("lv_info_srv");

	ShaderResourceViewReference tilesDepthUAV = passEnvironment.getShaderResourceView("lv_tilesDepth_uav");
	ShaderResourceViewReference tilesDepthSRV = passEnvironment.getShaderResourceView("lv_tilesDepth_srv");

	ShaderResourceViewReference tilesBbUAV = passEnvironment.getShaderResourceView("lv_tilesBB_uav");
	ShaderResourceViewReference tilesBbSRV = passEnvironment.getShaderResourceView("lv_tilesBB_srv");

	TextureReference depthSource = passEnvironment.getTexture("gb_depth_texture");
	ShaderResourceViewReference depthSourceSRV = passEnvironment.getShaderResourceView("gb_depth_srv");

	IGPUBuffer* pointLights = pointLightBuffer->getTypedResource();
	IGPUBuffer* zSlices = zSliceBuffer->getTypedResource();

	IGPUBuffer* dirLights = dirLightBuffer->getTypedResource();

	sortPointLights(pointLightCollection, lightPrepearingData.getCameraTransform());

	fillPointLightBuffer(pointLights, pointLightCollection);
	fillPointLightZSliceBuffer(zSlices, lightPrepearingData.getSlicesCount());

	fillDirLightBuffer(dirLights, lightPrepearingData.getDirectionalLightDatas());

	computeTilesDepth(drawContext.m_context, tilesDepthUAV, lightVolumeSRV,
		depthSourceSRV, depthSource->getTextureCast<ITexture2D>());

	computeTilesBoudingBoxes(drawContext.m_context, tilesBbUAV, tilesDepthSRV,
		lightPrepearingData.getCameraTransformView(), lightVolumeSRV, lightPrepearingData.getTilesCount());

	clearPointLightTileMasks(drawContext.m_context, tilesUAV, lightVolumeSRV, lightPrepearingData.getTilesCount());

	computePointLightTileMasks(drawContext.m_context, static_cast<uint32_t>(pointLightCollection.size()),
		pointLightsSRV, tilesUAV, tilesBbSRV,
		lightPrepearingData.getCameraTransformView(), lightVolumeSRV);
}
