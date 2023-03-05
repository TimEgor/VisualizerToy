#include "LightingVolumeData.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GraphicDevice/IGraphicDevice.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"

#include "RenderSystem/RenderPassEnvironment.h"
#include "RenderSystem/LightSources/PointLightData.h"

#include "Math/ComputeVector.h"

bool VT::LightVolumeData::init(const Vector2UInt16& targetSize, const Vector2UInt16& tileSize, uint16_t slicesCount)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;
	IGraphicDevice* device = environment->m_graphicDevice;

	m_tilesSlicesCount.m_x = (targetSize.m_x + tileSize.m_x - 1) / tileSize.m_x;
	m_tilesSlicesCount.m_y = (targetSize.m_y + tileSize.m_y - 1) / tileSize.m_y;
	m_tilesSlicesCount.m_z = slicesCount;

	m_targetSize = targetSize;
	m_tileSize = tileSize;

	VT_CHECK_RETURN_FALSE(initPointLightBuffer(device, resManager));
	VT_CHECK_RETURN_FALSE(initPointLightTileMaskBuffer(device, resManager));
	VT_CHECK_RETURN_FALSE(initPointLightZSliceBuffer(device, resManager));
	VT_CHECK_RETURN_FALSE(initVolumeInfoBuffer(device, resManager));

	return true;
}

bool VT::LightVolumeData::initPointLightBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager)
{
	GPUBufferDesc bufferDesc{};
	bufferDesc.isHostVisible = true;
	bufferDesc.m_byteSize = sizeof(PointLightData) * MAX_POINT_LIGHT_NUM;
	bufferDesc.m_byteStride = sizeof(PointLightData);
	bufferDesc.m_flag = GPUBufferFlag::STRUCTURED;
	bufferDesc.m_usage = GRAPHIC_USAGE_SHADER_RESOURCE;
	m_pointLightBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_COMMON_READ);
	VT_CHECK_RETURN_FALSE(m_pointLightBuffer);
	m_pointLightBufferSRV = resManager->createShaderResourceDescriptor(m_pointLightBuffer.getObject());
	VT_CHECK_RETURN_FALSE(m_pointLightBufferSRV);

	device->setResourceName(m_pointLightBuffer->getResource(), "lv_pointLights");

	return true;
}

bool VT::LightVolumeData::initPointLightTileMaskBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager)
{
	GPUBufferDesc bufferDesc{};
	bufferDesc.isHostVisible = false;
	bufferDesc.m_byteSize = m_tilesSlicesCount.m_x * m_tilesSlicesCount.m_y * POINT_LIGHT_MASK_SIZE;
	bufferDesc.m_byteStride = POINT_LIGHT_MASK_SIZE;
	bufferDesc.m_flag = GPUBufferFlag::STRUCTURED;
	bufferDesc.m_usage = GRAPHIC_USAGE_SHADER_RESOURCE | GRAPHIC_USAGE_ALLOW_UNORDERED_ACCESS;
	m_pointLightTileMasksBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_SHADER_RESOURCE_COMPUTE);
	VT_CHECK_RETURN_FALSE(m_pointLightTileMasksBuffer);
	m_pointLightTileMasksBufferSRV = resManager->createShaderResourceDescriptor(m_pointLightTileMasksBuffer.getObject());
	VT_CHECK_RETURN_FALSE(m_pointLightTileMasksBufferSRV);
	m_pointLightTileMasksBufferUAV = resManager->createUnorderedAccessResourceDescriptor(m_pointLightTileMasksBuffer.getObject());
	VT_CHECK_RETURN_FALSE(m_pointLightTileMasksBufferUAV);

	device->setResourceName(m_pointLightTileMasksBuffer->getResource(), "lv_pointLightsTileMasks");

	return true;
}

bool VT::LightVolumeData::initPointLightZSliceBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager)
{
	GPUBufferDesc bufferDesc{};
	bufferDesc.isHostVisible = true;
	bufferDesc.m_byteSize = sizeof(ZSlice) * m_tilesSlicesCount.m_z;
	bufferDesc.m_byteStride = sizeof(ZSlice);
	bufferDesc.m_flag = GPUBufferFlag::STRUCTURED;
	bufferDesc.m_usage = GRAPHIC_USAGE_SHADER_RESOURCE;
	m_pointLightZSliceBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_SHADER_RESOURCE_COMPUTE);
	VT_CHECK_RETURN_FALSE(m_pointLightZSliceBuffer);
	m_pointLightZSliceBufferSRV = resManager->createShaderResourceDescriptor(m_pointLightZSliceBuffer.getObject());
	VT_CHECK_RETURN_FALSE(m_pointLightZSliceBufferSRV);

	device->setResourceName(m_pointLightZSliceBuffer->getResource(), "lv_pointLightsSlices");

	return true;
}

bool VT::LightVolumeData::initVolumeInfoBuffer(IGraphicDevice* device, IGraphicResourceManager* resManager)
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
	bufferDesc.m_usage = GRAPHIC_USAGE_SHADER_RESOURCE;
	m_lightVolumeInfoBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicResourceState::GRAPHIC_STATE_SHADER_RESOURCE_COMPUTE, &initialGpuBufferData);
	VT_CHECK_RETURN_FALSE(m_lightVolumeInfoBuffer);
	m_lightVolumeInfoBufferSRV = resManager->createBufferResourceDescriptor(m_lightVolumeInfoBuffer);
	VT_CHECK_RETURN_FALSE(m_lightVolumeInfoBufferSRV);

	device->setResourceName(m_lightVolumeInfoBuffer->getResource(), "lv_info");

	return true;
}

void VT::LightVolumeData::release()
{
	m_pointLightBuffer = nullptr;
	m_pointLightTileMasksBuffer = nullptr;
	m_pointLightZSliceBuffer = nullptr;

	m_lightVolumeInfoBuffer = nullptr;

	m_pointLightBufferSRV = nullptr;
	m_pointLightTileMasksBufferSRV = nullptr;
	m_pointLightTileMasksBufferUAV = nullptr;
	m_pointLightZSliceBufferSRV = nullptr;

	m_lightVolumeInfoBufferSRV = nullptr;
}

void VT::LightVolumeData::fillEnvironment(RenderPassEnvironment& environment) const
{
	environment.addBuffer("lv_point_light_buffer", m_pointLightBuffer);
	environment.addBuffer("lv_point_light_tile_mask_buffer", m_pointLightTileMasksBuffer);
	environment.addBuffer("lv_point_light_zslice_buffer", m_pointLightZSliceBuffer);

	environment.addBuffer("lv_info_buffer", m_lightVolumeInfoBuffer);

	environment.addShaderResourceView("lv_point_light_srv", m_pointLightBufferSRV);
	environment.addShaderResourceView("lv_point_light_tile_mask_srv", m_pointLightTileMasksBufferSRV);
	environment.addShaderResourceView("lv_point_light_tile_mask_uav", m_pointLightTileMasksBufferUAV);
	environment.addShaderResourceView("lv_point_light_zslice_srv", m_pointLightZSliceBufferSRV);

	environment.addShaderResourceView("lv_info_srv", m_lightVolumeInfoBufferSRV);
}