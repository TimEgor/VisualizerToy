#include "LightingVolumeData.h"

#include "Core/UtilitiesMacros.h"

#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "Math/Box.h"
#include "Math/ComputeVector.h"
#include "GraphicResourceManager/IGraphicResourceManager.h"
#include "RenderSystem/LightSources/PointLightData.h"

bool VT::LightVolumeData::init(const Vector2UInt16& targetSize, float nearProjPlane, float farProjPlane,
	const Vector2UInt16& tileSize, uint16_t slicesCount)
{
	EngineEnvironment* environment = EngineInstance::getInstance()->getEnvironment();
	IGraphicResourceManager* resManager = environment->m_graphicResourceManager;

	m_tilesSlicesCount.m_x = (targetSize.m_x + tileSize.m_x - 1) / tileSize.m_x;
	m_tilesSlicesCount.m_y = (targetSize.m_y + tileSize.m_y - 1) / tileSize.m_y;
	m_tilesSlicesCount.m_z = slicesCount;

	m_targetSize = targetSize;
	m_tileSize = tileSize;

	m_nearProjPlane = nearProjPlane;
	m_farProjPlane = farProjPlane;

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
	m_pointLightBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicState::COMMON_READ);
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
	m_pointLightClusterBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicState::SHADER_RESOURCE_COMPUTE);
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
	initialGpuBufferData.dataSize = sizeof(InitialGPUBufferData);

	GPUBufferDesc bufferDesc{};
	bufferDesc.isHostVisible = false;
	bufferDesc.m_byteSize = sizeof(LightVolumeInfo);
	m_lightVolumeInfoBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicState::SHADER_RESOURCE_COMPUTE, &initialGpuBufferData);
	VT_CHECK_RETURN_FALSE(m_lightVolumeInfoBuffer);
	m_lightVolumeInfoBufferSRV = resManager->createBufferResourceDescriptor(m_lightVolumeInfoBuffer);
	VT_CHECK_RETURN_FALSE(m_lightVolumeInfoBufferSRV);
}

bool VT::LightVolumeData::initVolumeInfoBoundingBoxBuffer(IGraphicResourceManager* resManager)
{
	const uint32_t clusterCount = m_tilesSlicesCount.m_x * m_tilesSlicesCount.m_y * m_tilesSlicesCount.m_z;
	std::vector<AABB> clusterBoundingBoxes;

	calcClusterBoudingBoxes(clusterBoundingBoxes);

	InitialGPUBufferData initialGpuBufferData{};
	initialGpuBufferData.data = clusterBoundingBoxes.data();
	initialGpuBufferData.dataSize = sizeof(AABB) * clusterBoundingBoxes.size();

	GPUBufferDesc bufferDesc{};
	bufferDesc.isHostVisible = false;
	bufferDesc.m_byteSize = sizeof(AABB) * clusterCount;
	bufferDesc.m_byteStride = sizeof(AABB);
	bufferDesc.m_flag = GPUBufferFlag::STRUCTURED;
	m_lightVolumeBoundingBoxBuffer = resManager->createGPUBuffer(bufferDesc, CommonGraphicState::SHADER_RESOURCE_COMPUTE, &initialGpuBufferData);
	VT_CHECK_RETURN_FALSE(m_lightVolumeBoundingBoxBuffer);
	m_lightVolumeBoundingBoxBufferSRV = resManager->createBufferResourceDescriptor(m_lightVolumeBoundingBoxBuffer);
	VT_CHECK_RETURN_FALSE(m_lightVolumeBoundingBoxBufferSRV);

	return true;
}

void VT::LightVolumeData::calcClusterBoudingBoxes(std::vector<AABB>& boxes) const
{
	const uint16_t sliceTilesCount = m_tilesSlicesCount.m_x * m_tilesSlicesCount.m_y;
	const uint16_t clusterCount = sliceTilesCount  * m_tilesSlicesCount.m_z;

	const float projPlaneRatio = m_farProjPlane / m_nearProjPlane;

	boxes.clear();
	boxes.reserve(clusterCount);

	for (uint16_t sliceIndex = 0; sliceIndex < m_tilesSlicesCount.m_z; ++sliceIndex)
	{
		for (uint16_t tileY = 0; tileY < m_tilesSlicesCount.m_y; ++tileY)
		{
			for (uint16_t tileX = 0; tileX < m_tilesSlicesCount.m_x; ++tileX)
			{
				const Vector3 minScreenPos = Vector3(tileX * m_tilesSlicesCount.m_x, tileY * m_tilesSlicesCount.m_y, 1.0);
				const Vector3 maxScreenPos = Vector3((tileX + 1) * m_tilesSlicesCount.m_x, (tileY + 1) * m_tilesSlicesCount.m_y, 1.0);

				const Vector3 minEyePos = convertScreenPosToView(minScreenPos);
				const Vector3 maxEyePos = convertScreenPosToView(maxScreenPos);

				const float clusterNearSlice = m_nearProjPlane * pow(projPlaneRatio, sliceIndex / static_cast<float>(m_tilesSlicesCount.m_z));
				const float clusterFarSlice = m_nearProjPlane * pow(projPlaneRatio, (sliceIndex + 1) / static_cast<float>(m_tilesSlicesCount.m_z));

				const COMPUTE_MATH::ComputeVector minEyePosComputeVector = COMPUTE_MATH::loadComputeVectorFromVector3(minEyePos);
				const COMPUTE_MATH::ComputeVector maxEyePosComputeVector = COMPUTE_MATH::loadComputeVectorFromVector3(maxEyePos);

				const COMPUTE_MATH::ComputeVector minNearSlicePos = COMPUTE_MATH::vectorScale(minEyePosComputeVector, clusterNearSlice / minEyePos.m_z);
				const COMPUTE_MATH::ComputeVector minFarSlicePos = COMPUTE_MATH::vectorScale(minEyePosComputeVector, clusterFarSlice / minEyePos.m_z);
				const COMPUTE_MATH::ComputeVector maxNearSlicePos = COMPUTE_MATH::vectorScale(maxEyePosComputeVector, clusterNearSlice / maxEyePos.m_z);
				const COMPUTE_MATH::ComputeVector maxFarSlicePos = COMPUTE_MATH::vectorScale(maxEyePosComputeVector, clusterFarSlice / maxEyePos.m_z);

				AABB& box = boxes.emplace_back();
				box.m_min = COMPUTE_MATH::saveComputeVectorToVector3(
					COMPUTE_MATH::vectorMin(
						COMPUTE_MATH::vectorMin(minNearSlicePos, minFarSlicePos),
						COMPUTE_MATH::vectorMin(maxNearSlicePos, maxFarSlicePos)
					)
				);
				box.m_max = COMPUTE_MATH::saveComputeVectorToVector3(
					COMPUTE_MATH::vectorMax(
						COMPUTE_MATH::vectorMax(minNearSlicePos, minFarSlicePos),
						COMPUTE_MATH::vectorMax(maxNearSlicePos, maxFarSlicePos)
					)
				);
			}
		}
	}
}

VT::Vector3 VT::LightVolumeData::convertScreenPosToView(const Vector3& screenPos) const
{
	return Vector3(
		2.0f * screenPos.m_x / m_targetSize.m_x - 1.0f,
		2.0f * screenPos.m_y / m_targetSize.m_y - 1.0f,
		2.0f * screenPos.m_z - 1.0f
	);
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
