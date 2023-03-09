#include "LightPrepearingPass.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineEnvironment.h"
#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "RenderContext/IRenderContext.h"

#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "GraphicResourceCommon/IGraphicResourceDescriptor.h"

#include "RenderSystem/RenderingData.h"
#include "RenderSystem/RenderPassEnvironment.h"
#include "RenderSystem/LightSources/PointLightData.h"
#include "LightingVolumeData.h"

#include "Math/ComputeMatrix.h"
#include "Math/ComputeVector.h"

void VT::LightPrepearingPass::sortPointLights(const RenderingData::PointLightDataCollection& data, const CameraTransforms& cameraTransform)
{
	COMPUTE_MATH::ComputeMatrix viewTransformMatrix = COMPUTE_MATH::loadComputeMatrixFromMatrix4x4(cameraTransform.m_viewTransform);
	COMPUTE_MATH::ComputeVector resultPosition;

	const uint32_t lightsCount = data.size();
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
	const RenderingData::PointLightDataCollection& lightData)
{
	PointLightData* mappedLights = nullptr;
	pointLightBuffer->mapData(reinterpret_cast<void**>(&mappedLights));

	const uint16_t lightsCount = m_sortedPointLights.size();
	for (uint16_t i = 0; i < lightsCount; ++i)
	{
		// TODO: need to improve this mapping
		const PointLightData* light = &lightData[m_sortedPointLights[i].m_lightIndex];
		memcpy(&mappedLights[i], light, sizeof(PointLightData));
	}

	pointLightBuffer->unmapData();
}

void VT::LightPrepearingPass::fillPointLightZSliceBuffer(IGPUBuffer* zSliceBuffer)
{
	assert(m_lightVolumeData);

	const uint16_t slicesCount = m_lightVolumeData->getSlicesCount();
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

				if (i > maxSliceLightIndex )
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

void VT::LightPrepearingPass::computePointLightTileMasks(IRenderContext* context, uint32_t pointLightsCount,
	ShaderResourceViewReference pointLightsSRV, ShaderResourceViewReference tilesUAV,
	ShaderResourceViewReference cameraTransformsSRV, ShaderResourceViewReference lightVolumeSRV)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();

	ComputePipelineStateInfo pipelineStateInfo{};
	pipelineStateInfo.m_computeShader = m_pointLightCullingShader->getTypedObject();

	ComputePipelineStateReference pipelineState = environment->m_graphicResourceManager->
		getComputePipelineState(pipelineStateInfo, m_bindingLayout);

	context->setDescriptorHeap(environment->m_graphicDevice->getBindlessResourceDescriptionHeap());
	context->setComputeBindingLayout(m_bindingLayout->getTypedObject());

	context->setComputeBindingParameterValue(0, 0, cameraTransformsSRV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(1, 0, lightVolumeSRV->getResourceView()->getBindingHeapOffset());

	context->setComputeBindingParameterValue(2, 0, pointLightsCount);
	context->setComputeBindingParameterValue(2, 1, pointLightsSRV->getResourceView()->getBindingHeapOffset());
	context->setComputeBindingParameterValue(2, 2, tilesUAV->getResourceView()->getBindingHeapOffset());

	context->setPipelineState(pipelineState->getTypedObject());

	const uint32_t threadGroupCount = (pointLightsCount + 15) / 16;
	context->dispatch(threadGroupCount, 1, 1);
}

bool VT::LightPrepearingPass::init()
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	PipelineBindingLayoutDesc bindingDesc{};
	bindingDesc.m_descriptorBindings.emplace_back(1, 0, 0, ShaderStageVisibility::ALL_STAGES);
	bindingDesc.m_descriptorBindings.emplace_back(1, 1, 0, ShaderStageVisibility::ALL_STAGES);
	bindingDesc.m_descriptorBindings.emplace_back(3, 2, 0, ShaderStageVisibility::ALL_STAGES);

	m_pointLightCullingShader = resManager->loadComputeShader("Shaders/Lighting/PointLightCulling.hlsl");
	VT_CHECK_INITIALIZATION(m_pointLightCullingShader);
	m_bindingLayout = resManager->getPipelineBindingLayout(bindingDesc);
	VT_CHECK_INITIALIZATION(m_bindingLayout);

	return true;
}

void VT::LightPrepearingPass::release()
{
	m_pointLightCullingShader = nullptr;
	m_bindingLayout = nullptr;
}

void VT::LightPrepearingPass::execute(const RenderPassContext& passContext, const RenderPassEnvironment& passEnvironment)
{
	const RenderingData::PointLightDataCollection& pointLightCollection = passContext.m_renderingData.getPointLighDataCollection();
	if (pointLightCollection.empty())
	{
		return;
	}

	GPUBufferReference pointLightBuffer = passEnvironment.getBuffer("lv_point_light_buffer");
	GPUBufferReference zSliceBuffer = passEnvironment.getBuffer("lv_point_light_zslice_buffer");

	ShaderResourceViewReference pointLightsSRV = passEnvironment.getShaderResourceView("lv_point_light_srv");
	ShaderResourceViewReference tilesUAV = passEnvironment.getShaderResourceView("lv_point_light_tile_mask_uav");
	ShaderResourceViewReference lightVolumeSRV = passEnvironment.getShaderResourceView("lv_info_srv");

	IGPUBuffer* pointLights = pointLightBuffer->getTypedResource();
	IGPUBuffer* zSlices = zSliceBuffer->getTypedResource();

	sortPointLights(pointLightCollection, passContext.m_renderingData.getCameraTransform());

	fillPointLightBuffer(pointLights, pointLightCollection);
	fillPointLightZSliceBuffer(zSlices);

	computePointLightTileMasks(passContext.m_context, pointLightCollection.size(), pointLightsSRV, tilesUAV,
		passContext.m_renderingData.getCameraTransformBufferView(), lightVolumeSRV);
}
