#include "PointLight.hlsli"
#include "LightVolume.hlsli"

#include "../Common/CameraData.hlsli"
#include "../Common/CommonBindingTypes.hlsli"

struct PointLightBindings
{
	uint m_pointLightsCount;

	uint m_lightBufferIndex;
	uint m_tileMaskBufferIndex;
};

ConstantBuffer<UniqueBinding> cameraTransformsBinding : register(b0);
ConstantBuffer<UniqueBinding> lightVolumeInfoBinding : register(b1);
ConstantBuffer<PointLightBindings> pointLightBindings : register(b2);

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

	StructuredBuffer<PointLight> pointLights = ResourceDescriptorHeap[pointLightBindings.m_lightBufferIndex];
	RWStructuredBuffer<PointLightTileMask> tileMasks = ResourceDescriptorHeap[pointLightBindings.m_tileMaskBufferIndex];
	
	const PointLight light = pointLights[lightIndex];

	const float4 lightViewPosition = mul(float4(light.m_position, 1.0f), cameraTransforms.m_viewTransformMatrix);
	const float4 lightRangeDir = float4(light.m_range, light.m_range, light.m_range, 0.0f);

	const float4 minLightViewPosition = lightViewPosition - lightRangeDir;
	const float4 maxLightViewPosition = lightViewPosition + lightRangeDir;

	const float4 minLightProjPosition = mul(minLightViewPosition, cameraTransforms.m_projTransformMatrix);
	const float4 maxLightProjPosition = mul(maxLightViewPosition, cameraTransforms.m_projTransformMatrix);

	const float2 normMinLightScreenPosition = (minLightProjPosition.xy / minLightProjPosition.w + float2(1.0f, 1.0f)) * 0.5f;
	const float2 normMaxLightScreenPosition = (maxLightProjPosition.xy / maxLightProjPosition.w + float2(1.0f, 1.0f)) * 0.5f;

	const float2 normTileSize = float2(1.0f / lightVolume.m_tilesCountX, 1.0f / lightVolume.m_tilesCountY);

	const uint2 minLightTileCoords = trunc(saturate(normMinLightScreenPosition) / normTileSize);
	const uint2 maxLightTileCoords = ceil(saturate(normMaxLightScreenPosition) / normTileSize);

	for (uint y = minLightTileCoords.y; y <= maxLightTileCoords.y; ++y)
	{
		for (uint x = minLightTileCoords.x; x <= maxLightTileCoords.x; ++x)
		{
			const uint tileIndex = y * lightVolume.m_tilesCountX + x;

			const uint tileMaskValIndex = lightIndex / 32;
			const uint tileMaskBitIndex = lightIndex % 32;

			const uint maskBitVal = 1u << tileMaskBitIndex;
			uint prevMaskVal = 0;
			InterlockedOr(tileMasks[tileIndex].m_mask[tileMaskValIndex], maskBitVal, prevMaskVal);
		}
	}
}