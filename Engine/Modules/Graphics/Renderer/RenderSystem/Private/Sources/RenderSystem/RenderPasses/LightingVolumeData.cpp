#include "LightingVolumeData.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "Math/Box.h"
#include "Math/ComputeVector.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "RenderSystem/RenderPassEnvironment.h"
#include "RenderSystem/LightSources/PointLightData.h"

bool VT::LightVolumeData::init(const Vector2UInt16& targetSize, const Vector2UInt16& tileSize, uint16_t slicesCount)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	m_tilesSlicesCount.m_x = (targetSize.m_x + tileSize.m_x - 1) / tileSize.m_x;
	m_tilesSlicesCount.m_y = (targetSize.m_y + tileSize.m_y - 1) / tileSize.m_y;
	m_tilesSlicesCount.m_z = slicesCount;

	m_targetSize = targetSize;
	m_tileSize = tileSize;

	VT_CHECK_RETURN_FALSE(initPointLightBuffer(resManager));
	VT_CHECK_RETURN_FALSE(initPointLightClusterBuffer(resManager));
	VT_CHECK_RETURN_FALSE(initVolumeInfoBuffer(resManager));
	VT_CHECK_RETURN_FALSE(initVolumeInfoBoundingBoxBuffer(resManager));

	return true;
}

bool VT::LightVolumeData::initPointLightBuffer(IGraphicResourceManager* resManager)
{
	GPUBufferDesc bufferDesc{};
	bufferDesc.isHostVisible = true;
	bufferDesc.m_byteSize = sizeof(PointLightData) * MAX_POINT_LIGHT_NUM;
	bufferDesc.m_byteStride = sizeof(PointLightData);
	bufferDesc.m_flag = GPUBufferFlag::STRUCTURED;
	m_pointLightBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_COMMON_READ);
	VT_CHECK_RETURN_FALSE(m_pointLightBuffer);
	m_pointLightBufferSRV = resManager->createBufferResourceDescriptor(m_pointLightBuffer);
	VT_CHECK_RETURN_FALSE(m_pointLightBufferSRV);

	return true;
}

bool VT::LightVolumeData::initPointLightClusterBuffer(IGraphicResourceManager* resManager)
{
	GPUBufferDesc bufferDesc{};
	bufferDesc.isHostVisible = false;
	bufferDesc.m_byteSize = sizeof(LightIndex) * m_tilesSlicesCount.m_x * m_tilesSlicesCount.m_y * m_tilesSlicesCount.m_z * MAX_POINT_LIGHT_NUM_PER_CLUSTER;
	bufferDesc.m_byteStride = sizeof(LightIndex);
	bufferDesc.m_flag = GPUBufferFlag::STRUCTURED;
	m_pointLightClusterBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_SHADER_RESOURCE_COMPUTE);
	VT_CHECK_RETURN_FALSE(m_pointLightClusterBuffer);
	m_pointLightClusterBufferSRV = resManager->createBufferResourceDescriptor(m_pointLightClusterBuffer);
	VT_CHECK_RETURN_FALSE(m_pointLightClusterBufferSRV);

	return true;
}

bool VT::LightVolumeData::initVolumeInfoBuffer(IGraphicResourceManager* resManager)
{
	LightVolumeInfo volumeInfo{};
	volumeInfo.m_tileCountX = m_tilesSlicesCount.m_x;
	volumeInfo.m_tileCountY = m_tilesSlicesCount.m_y;
	volumeInfo.m_sliceCount = m_tilesSlicesCount.m_z;

	InitialGPUBufferData initialGpuBufferData{};
	initialGpuBufferData.data = &volumeInfo;
	initialGpuBufferData.dataSize = sizeof(LightVolumeInfo);

	GPUBufferDesc bufferDesc{};
	bufferDesc.isHostVisible = false;
	bufferDesc.m_byteSize = sizeof(LightVolumeInfo);
	m_lightVolumeInfoBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_SHADER_RESOURCE_COMPUTE, &initialGpuBufferData);
	VT_CHECK_RETURN_FALSE(m_lightVolumeInfoBuffer);
	m_lightVolumeInfoBufferSRV = resManager->createBufferResourceDescriptor(m_lightVolumeInfoBuffer);
	VT_CHECK_RETURN_FALSE(m_lightVolumeInfoBufferSRV);

	return true;
}

bool VT::LightVolumeData::initVolumeInfoBoundingBoxBuffer(IGraphicResourceManager* resManager)
{
	const uint32_t clusterCount = m_tilesSlicesCount.m_x * m_tilesSlicesCount.m_y * m_tilesSlicesCount.m_z;

	GPUBufferDesc bufferDesc{};
	bufferDesc.isHostVisible = false;
	bufferDesc.m_byteSize = sizeof(AABB) * clusterCount;
	bufferDesc.m_byteStride = sizeof(AABB);
	bufferDesc.m_flag = GPUBufferFlag::STRUCTURED;
	m_lightVolumeBoundingBoxBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_SHADER_RESOURCE_COMPUTE);
	VT_CHECK_RETURN_FALSE(m_lightVolumeBoundingBoxBuffer);
	m_lightVolumeBoundingBoxBufferSRV = resManager->createBufferResourceDescriptor(m_lightVolumeBoundingBoxBuffer);
	VT_CHECK_RETURN_FALSE(m_lightVolumeBoundingBoxBufferSRV);

	return true;
}

void VT::LightVolumeData::release()
{
	m_pointLightBuffer = nullptr;
	m_pointLightClusterBuffer = nullptr;
	m_lightVolumeInfoBuffer = nullptr;
	m_lightVolumeBoundingBoxBuffer = nullptr;

	m_pointLightBufferSRV = nullptr;
	m_pointLightClusterBuffer = nullptr;
	m_lightVolumeInfoBufferSRV = nullptr;
	m_lightVolumeBoundingBoxBuffer = nullptr;
}

void VT::LightVolumeData::fillEnvironment(RenderPassEnvironment& environment) const
{
	environment.addBuffer("lv_point_light_buffer", m_pointLightBuffer.getObject());
	environment.addBuffer("lv_point_light_cluster_buffer", m_pointLightClusterBuffer.getObject());
	environment.addBuffer("lv_info_buffer", m_lightVolumeInfoBuffer.getObject());
	environment.addBuffer("lv_aabb_buffer", m_lightVolumeBoundingBoxBuffer.getObject());

	environment.addShaderResourceView("lv_point_light_srv", m_pointLightBufferSRV);
	environment.addShaderResourceView("lv_point_light_cluster_srv", m_pointLightClusterBufferSRV);
	environment.addShaderResourceView("lv_info_buffer", m_lightVolumeInfoBufferSRV);
	environment.addShaderResourceView("lv_aabb_buffer", m_lightVolumeBoundingBoxBufferSRV);
}