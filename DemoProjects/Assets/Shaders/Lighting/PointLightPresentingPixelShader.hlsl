#include "PointLight.hlsli"
#include "LightVolume.hlsli"

#include "../Common/CameraData.hlsli"
#include "../Common/CommonBindingTypes.hlsli"

struct PSInput
{
	float4 m_position : SV_POSITION;
	float2 m_uv : TEXTURE0;
};

struct GBufferBindings
{
	uint m_colorTextureInfo;
	uint m_normalTextureInfo;
	uint m_positionTextureInfo;
};

struct PointLightBindings
{
	uint m_lightBufferIndex;
	uint m_tileMasksBufferIndex;
	uint m_zSliceBufferIndex;
};

ConstantBuffer<UniqueBinding> cameraTransformsBinding : register(b0);
ConstantBuffer<UniqueBinding> lightVolumeBinding : register(b1);
ConstantBuffer<GBufferBindings> gBufferBindings : register(b2);
ConstantBuffer<PointLightBindings> pointLightBindings : register(b3);
SamplerState textureSampler : register(s0);

float liminanceAttenuation(in float3 lightRay, float lightRadius)
{
	return max(0.0f, 1.0f - (length(lightRay) / lightRadius));
}

float3 calcPointLight(in float3 position, in float3 normal, in float3 color, in PointLight light)
{
	const float3 rayToLight = light.m_position - position;
	const float3 rayDir = normalize(rayToLight);
	const float dirAngle = saturate(dot(normal, rayDir));

	const float attenuation = liminanceAttenuation(rayToLight, light.m_range) * length(normal);
	
	const float lightNormalAngle = saturate(dot(normal, rayDir));
	const float3 luminanceColor = lightNormalAngle * light.m_color * color;

	return luminanceColor * attenuation;
}

float4 PS(PSInput input) : SV_TARGET0
{
	ConstantBuffer<CameraTransforms> cameraTransforms = ResourceDescriptorHeap[cameraTransformsBinding.m_bindingIndex];
	ConstantBuffer<LightVolumeInfo> lightVolume = ResourceDescriptorHeap[lightVolumeBinding.m_bindingIndex];

	StructuredBuffer<PointLight> pointLights = ResourceDescriptorHeap[pointLightBindings.m_lightBufferIndex];
	StructuredBuffer<PointLightTileMask> tileMasks = ResourceDescriptorHeap[pointLightBindings.m_tileMasksBufferIndex];
	StructuredBuffer<ZSlice> zSlice = ResourceDescriptorHeap[pointLightBindings.m_zSliceBufferIndex];

	Texture2D<float4> colorInput = ResourceDescriptorHeap[gBufferBindings.m_colorTextureInfo];
	Texture2D<float4> normalInput = ResourceDescriptorHeap[gBufferBindings.m_normalTextureInfo];
	Texture2D<float4> positionInput = ResourceDescriptorHeap[gBufferBindings.m_positionTextureInfo];

	float3 color = colorInput.Sample(textureSampler, input.m_uv).xyz;
	float3 normal = normalInput.Sample(textureSampler, input.m_uv).xyz;
	float3 position = positionInput.Sample(textureSampler, input.m_uv).xyz;

	const float4 viewPosition = mul(float4(position, 1.0f), cameraTransforms.m_viewTransformMatrix);
	const float deltaZ = cameraTransforms.m_farPlane - cameraTransforms.m_nearPlane;
	const float sliceDepth = (viewPosition.z - cameraTransforms.m_nearPlane) / deltaZ;

	const uint sliceIndex = sliceDepth / (deltaZ / lightVolume.m_slicesCount);
	const ZSlice slice = zSlice[sliceIndex];

	float4 resultColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	if (slice.m_minLightIndex != MAX_POINT_LIGHTS)
	{
		const float2 normTileSize = float2(1.0f / lightVolume.m_tilesCountX, 1.0f / lightVolume.m_tilesCountY);
		const uint2 tileCoords = trunc(input.m_uv / normTileSize);
		const uint tileIndex = tileCoords.y * lightVolume.m_tilesCountX + tileCoords.x;

		for (uint lightIndex = slice.m_minLightIndex; lightIndex <= slice.m_maxLightIndex; ++lightIndex)
		{
			const uint maskValIndex = lightIndex / 32;
			const uint maskBitIndex = lightIndex % 32;

			const uint maskBitVal = 1u << maskBitIndex;

			if (tileMasks[tileIndex].m_mask[maskValIndex] & maskBitVal)
			{
				const PointLight light = pointLights[lightIndex];
				resultColor.xyz += calcPointLight(position, normal, color, light);
				//resultColor += float4(light.m_color, 1.0f);
			}
		}
	}

	return resultColor;
}