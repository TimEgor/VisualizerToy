#include "LightVolume.hlsli"

#include "../Common/CameraData.hlsli"
#include "../Common/CommonBindingTypes.hlsli"
#include "../Common/Box.hlsli"
#include "../Common/Type.hlsli"

ConstantBuffer<UniqueBinding> cameraTransformsBinding : register(b0);
ConstantBuffer<UniqueBinding> lightVolumeInfoBinding : register(b1);
ConstantBuffer<UniqueBinding> lightVolumeBBBinding : register(b2);
ConstantBuffer<UniqueBinding> lightVolumeDepthBinding : register(b3);

float3 calcViewTilePosition(in float2 tilePosition, in float depth, in float4x4 invProj)
{
	const float2 invertTilePosition = float2(tilePosition.x, 1.0f - tilePosition.y);
	const float2 tileScreenPosition = invertTilePosition * 2.0f - 1.0;
	const float4 tileViewPosition = mul(float4(tileScreenPosition, depth, 1.0f), invProj);

	return float4(tileViewPosition / tileViewPosition.w).xyz;
}

[numthreads(8, 8, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	ConstantBuffer<LightVolumeInfo> lightVolume = ResourceDescriptorHeap[lightVolumeInfoBinding.m_bindingIndex];

	if (dispatchThreadID.x >= lightVolume.m_tilesCountX || dispatchThreadID.y >= lightVolume.m_tilesCountY)
	{
		return;
	}

	ConstantBuffer<CameraTransforms> cameraTransforms = ResourceDescriptorHeap[cameraTransformsBinding.m_bindingIndex];
	StructuredBuffer<TileDepth> depth = ResourceDescriptorHeap[lightVolumeDepthBinding.m_bindingIndex];
	RWStructuredBuffer<AABB> boundingBoxes = ResourceDescriptorHeap[lightVolumeBBBinding.m_bindingIndex];
	
	const float2 normTileSize = float2(1.0f / lightVolume.m_tilesCountX, 1.0f / lightVolume.m_tilesCountY);
	const uint tileIndex =  dispatchThreadID.y * lightVolume.m_tilesCountX + dispatchThreadID.x;

	TileDepth tileDepth = depth[tileIndex];

	float3 tileBoundBoxCorners[8];
	tileBoundBoxCorners[0] = calcViewTilePosition(float2(dispatchThreadID.x, dispatchThreadID.y) * normTileSize, tileDepth.m_min, cameraTransforms.m_invProjTransformMatrix);
	tileBoundBoxCorners[1] = calcViewTilePosition(float2(dispatchThreadID.x + 1, dispatchThreadID.y) * normTileSize, tileDepth.m_min, cameraTransforms.m_invProjTransformMatrix);
	tileBoundBoxCorners[2] = calcViewTilePosition(float2(dispatchThreadID.x, dispatchThreadID.y + 1) * normTileSize, tileDepth.m_min, cameraTransforms.m_invProjTransformMatrix);
	tileBoundBoxCorners[3] = calcViewTilePosition(float2(dispatchThreadID.x + 1, dispatchThreadID.y + 1) * normTileSize, tileDepth.m_min, cameraTransforms.m_invProjTransformMatrix);

	tileBoundBoxCorners[4] = calcViewTilePosition(float2(dispatchThreadID.x, dispatchThreadID.y) * normTileSize, tileDepth.m_max, cameraTransforms.m_invProjTransformMatrix);
	tileBoundBoxCorners[5] = calcViewTilePosition(float2(dispatchThreadID.x + 1, dispatchThreadID.y) * normTileSize, tileDepth.m_max, cameraTransforms.m_invProjTransformMatrix);
	tileBoundBoxCorners[6] = calcViewTilePosition(float2(dispatchThreadID.x, dispatchThreadID.y + 1) * normTileSize, tileDepth.m_max, cameraTransforms.m_invProjTransformMatrix);
	tileBoundBoxCorners[7] = calcViewTilePosition(float2(dispatchThreadID.x + 1, dispatchThreadID.y + 1) * normTileSize, tileDepth.m_max, cameraTransforms.m_invProjTransformMatrix);

	float3 minCorner = FLT_MAX;
	float3 maxCorner = -FLT_MAX;

	for (uint i = 0; i < 8; ++i)
	{
		minCorner = min(minCorner, tileBoundBoxCorners[i]);
		maxCorner = max(maxCorner, tileBoundBoxCorners[i]);
	}

	boundingBoxes[tileIndex].m_min = minCorner;
	boundingBoxes[tileIndex].m_max = maxCorner;
}