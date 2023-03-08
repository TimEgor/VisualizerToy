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

	const float2 normMinLightScreenPosition = (minLightProjPosition.xy / minLightProjPosition.w + float2(1.0f, 1.0f)) / 2.0f;
	const float2 normMaxLightScreenPosition = (maxLightProjPosition.xy / maxLightProjPosition.w + float2(1.0f, 1.0f)) / 2.0f;

	const float2 normTileSize = float2(1.0f / lightVolume.m_tilesCountX, 1.0f / lightVolume.m_tilesCountY);

	const int2 minLightTileCoords = saturate(normMinLightScreenPosition) / normTileSize;
	const int2 maxLightTileCoords = floor(saturate(normMaxLightScreenPosition) / normTileSize);

	for (int y = minLightTileCoords.y; y <= maxLightTileCoords.y; ++y) {
		for (int x = minLightTileCoords.x; x <= maxLightTileCoords.x; ++x) {
			const uint tileIndex = y * lightVolume.m_tilesCountX + x;

			const uint tileMaskValIndex = lightIndex / 32;
			const uint tileMaskBitIndex = lightIndex % 32;

			const uint maskBitVal = 1u << tileMaskBitIndex;
			tileMasks[tileIndex].m_mask[tileMaskValIndex] |= maskBitVal;
		}
	}
}