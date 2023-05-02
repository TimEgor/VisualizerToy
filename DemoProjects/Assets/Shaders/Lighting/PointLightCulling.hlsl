#include "PointLight.hlsli"
#include "LightVolume.hlsli"

#include "../Common/CameraData.hlsli"
#include "../Common/CommonBindingTypes.hlsli"
#include "../Common/Box.hlsli"
#include "../Common/Type.hlsli"

struct PointLightBindings
{
	uint m_pointLightsCount;

	uint m_lightBufferIndex;
	uint m_tileMaskBufferIndex;
};

ConstantBuffer<UniqueBinding> cameraTransformsBinding : register(b0);
ConstantBuffer<UniqueBinding> lightVolumeInfoBinding : register(b1);
ConstantBuffer<UniqueBinding> tileBBsBinding : register(b2);
ConstantBuffer<PointLightBindings> pointLightBindings : register(b3);

bool checkOutScreenPositions(in float2 minPosition, in float2 maxPosition)
{
	return minPosition.x > 1.0f || minPosition.y > 1.0f || maxPosition.x < -1.0f || maxPosition.y < -1.0f;
}

bool checkIntersectsAABB(in float3 lightCenter, in float lightRadius, in AABB aabb)
{
	float3 closestPoint = lightCenter;
	closestPoint = max(closestPoint, aabb.m_min);
	closestPoint = min(closestPoint, aabb.m_max);

	const float3 delta = lightCenter - closestPoint;
	const float distSq = dot(delta, delta);
	return distSq <= lightRadius * lightRadius;
}

float2 calcLightProjPos(in float3 pos, in float4x4 proj)
{
	const float4 projPos = mul(float4(pos, 1.0f), proj);
	return float2(projPos.xy / projPos.w);
}

struct LightProjBoundingBox
{
	float2 m_minPosition;
	float2 m_maxPosition;
};

LightProjBoundingBox calcLightMinMaxProjPos(in float3 lightViewPosition, float lightRange, in float4x4 proj)
{
	float2 lightBoundBoxCorners[8];
	lightBoundBoxCorners[0] = calcLightProjPos(float3(lightViewPosition.x - lightRange, lightViewPosition.y - lightRange, lightViewPosition.z - lightRange), proj);
	lightBoundBoxCorners[1] = calcLightProjPos(float3(lightViewPosition.x - lightRange, lightViewPosition.y + lightRange, lightViewPosition.z - lightRange), proj);
	lightBoundBoxCorners[2] = calcLightProjPos(float3(lightViewPosition.x - lightRange, lightViewPosition.y - lightRange, lightViewPosition.z + lightRange), proj);
	lightBoundBoxCorners[3] = calcLightProjPos(float3(lightViewPosition.x - lightRange, lightViewPosition.y + lightRange, lightViewPosition.z + lightRange), proj);

	lightBoundBoxCorners[4] = calcLightProjPos(float3(lightViewPosition.x + lightRange, lightViewPosition.y - lightRange, lightViewPosition.z - lightRange), proj);
	lightBoundBoxCorners[5] = calcLightProjPos(float3(lightViewPosition.x + lightRange, lightViewPosition.y + lightRange, lightViewPosition.z - lightRange), proj);
	lightBoundBoxCorners[6] = calcLightProjPos(float3(lightViewPosition.x + lightRange, lightViewPosition.y - lightRange, lightViewPosition.z + lightRange), proj);
	lightBoundBoxCorners[7] = calcLightProjPos(float3(lightViewPosition.x + lightRange, lightViewPosition.y + lightRange, lightViewPosition.z + lightRange), proj);

	float minX = FLT_MAX;
	float minY = -FLT_MAX;
	float maxX = -FLT_MAX;
	float maxY = FLT_MAX;

	for (uint i = 0; i < 8; ++i)
	{
		minX = min(minX, lightBoundBoxCorners[i].x);
		minY = max(minY, lightBoundBoxCorners[i].y);
		maxX = max(maxX, lightBoundBoxCorners[i].x);
		maxY = min(maxY, lightBoundBoxCorners[i].y);
	}

	LightProjBoundingBox boundingBox;
	boundingBox.m_minPosition = float2(minX, minY);
	boundingBox.m_maxPosition = float2(maxX, maxY);

	return boundingBox;
}

[numthreads(16, 1, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint lightIndex = dispatchThreadID.x;
	if (lightIndex >= pointLightBindings.m_pointLightsCount)
	{
		return;
	}

	ConstantBuffer<CameraTransforms> cameraTransforms = ResourceDescriptorHeap[cameraTransformsBinding.m_bindingIndex];
	ConstantBuffer<LightVolumeInfo> lightVolume = ResourceDescriptorHeap[lightVolumeInfoBinding.m_bindingIndex];
	StructuredBuffer<AABB> tilesBoundingBoxes = ResourceDescriptorHeap[tileBBsBinding.m_bindingIndex];

	StructuredBuffer<PointLight> pointLights = ResourceDescriptorHeap[pointLightBindings.m_lightBufferIndex];
	RWStructuredBuffer<PointLightTileMask> tileMasks = ResourceDescriptorHeap[pointLightBindings.m_tileMaskBufferIndex];
	
	const PointLight light = pointLights[lightIndex];
	const float4 lightViewPosition = mul(float4(light.m_position, 1.0f), cameraTransforms.m_viewTransformMatrix);

	const LightProjBoundingBox lightProjBoundingBox = calcLightMinMaxProjPos(lightViewPosition.xyz, light.m_range, cameraTransforms.m_projTransformMatrix);

	const float2 normMinLightScreenPosition = (lightProjBoundingBox.m_minPosition + float2(1.0f, 1.0f)) * 0.5f;
	const float2 normMaxLightScreenPosition = (lightProjBoundingBox.m_maxPosition + float2(1.0f, 1.0f)) * 0.5f;

	const float2 invertNormMinLightScreenPosition = float2(normMinLightScreenPosition.x, 1.0f - normMinLightScreenPosition.y);
	const float2 invertNormMaxLightScreenPosition = float2(normMaxLightScreenPosition.x, 1.0f - normMaxLightScreenPosition.y);

	//tmp checking (need be removed after culling addition)
	if (checkOutScreenPositions(invertNormMinLightScreenPosition, invertNormMaxLightScreenPosition)) 
	{
		return;
	}

	const float2 normTileSize = float2(1.0f / lightVolume.m_tilesCountX, 1.0f / lightVolume.m_tilesCountY);

	const uint2 minLightTileCoords = floor(saturate(invertNormMinLightScreenPosition) / normTileSize);
	const uint2 maxLightTileCoords = ceil(saturate(invertNormMaxLightScreenPosition) / normTileSize);

	for (uint y = minLightTileCoords.y; y < maxLightTileCoords.y; ++y)
	{
		for (uint x = minLightTileCoords.x; x < maxLightTileCoords.x; ++x)
		{
			const uint tileIndex = y * lightVolume.m_tilesCountX + x;

			const AABB tileBoundingBox = tilesBoundingBoxes[tileIndex];
			if (!checkIntersectsAABB(lightViewPosition.xyz, light.m_range, tileBoundingBox))
			{
				continue;
			}

			const uint tileMaskValIndex = lightIndex / 32;
			const uint tileMaskBitIndex = lightIndex % 32;

			const uint maskBitVal = 1u << tileMaskBitIndex;
			uint prevMaskVal = 0;
			InterlockedOr(tileMasks[tileIndex].m_mask[tileMaskValIndex], maskBitVal, prevMaskVal);
		}
	}
}