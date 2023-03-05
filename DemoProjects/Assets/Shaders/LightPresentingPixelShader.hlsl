#define MAX_POINT_LIGHTS 256
#define TILE_MASK_SIZE MAX_POINT_LIGHTS / 32

struct PSInput
{
	float4 position : SV_POSITION;
	float2 uv : TEXTURE0;
};

struct UniqueBufferBindingInfo
{
	uint m_bufferIndex;
};

struct GBufferBindingInfo
{
	uint m_colorTextureInfo;
	uint m_normalTextureInfo;
	uint m_positionTextureInfo;
};

struct PointLightBindingInfo
{
	uint m_lightBufferIndex;
	uint m_tileMasksBufferIndex;
	uint m_zSliceBufferIndex;
};

struct PointLight
{
	float3 m_color;
	float m_range;
	float3 m_position;
};

struct TileMask
{
	uint m_mask[TILE_MASK_SIZE];
};

struct ZSlice
{
	uint m_minLightIndex;
	uint m_maxLightIndex;
};

struct CameraTransforms
{
	float4x4 m_viewTransformMatrix;
	float4x4 m_projTransformMatrix;

	float3 m_position;
	float m_nearPlane;
	float m_farPlane;
};

struct LightVolumeInfo
{
	uint m_tilesCountX;
	uint m_tilesCountY;
	uint m_slicesCount;
};

ConstantBuffer<UniqueBufferBindingInfo> cameraTransformsBinding : register(b0);
ConstantBuffer<UniqueBufferBindingInfo> lightVolumeInfoBinding : register(b1);
ConstantBuffer<GBufferBindingInfo> gBufferBindingInfo : register(b2);
ConstantBuffer<PointLightBindingInfo> pointLightBindingInfo : register(b3);
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
	ConstantBuffer<CameraTransforms> cameraTransforms = ResourceDescriptorHeap[cameraTransformsBinding.m_bufferIndex];
	ConstantBuffer<LightVolumeInfo> lightVolume = ResourceDescriptorHeap[lightVolumeInfoBinding.m_bufferIndex];

	StructuredBuffer<PointLight> pointLights = ResourceDescriptorHeap[pointLightBindingInfo.m_lightBufferIndex];
	StructuredBuffer<TileMask> tileMasks = ResourceDescriptorHeap[pointLightBindingInfo.m_tileMasksBufferIndex];
	StructuredBuffer<ZSlice> zSlice = ResourceDescriptorHeap[pointLightBindingInfo.m_zSliceBufferIndex];

	Texture2D<float4> colorInput = ResourceDescriptorHeap[gBufferBindingInfo.m_colorTextureInfo];
	Texture2D<float4> normalInput = ResourceDescriptorHeap[gBufferBindingInfo.m_normalTextureInfo];
	Texture2D<float4> positionInput = ResourceDescriptorHeap[gBufferBindingInfo.m_positionTextureInfo];

	float3 color = colorInput.Sample(textureSampler, input.uv).xyz;
	float3 normal = normalInput.Sample(textureSampler, input.uv).xyz;
	float3 position = positionInput.Sample(textureSampler, input.uv).xyz;

	const float4 viewPosition = mul(float4(position, 1.0f), cameraTransforms.m_viewTransformMatrix);
	const float deltaZ = cameraTransforms.m_farPlane - cameraTransforms.m_nearPlane;
	const float sliceDepth = (viewPosition.z - cameraTransforms.m_nearPlane) / deltaZ;

	const int sliceIndex = sliceDepth / (deltaZ / lightVolume.m_slicesCount);
	const ZSlice slice = zSlice[sliceIndex];

	float4 resultColor = float4(0.0f, 0.0f, 0.0f, 1.0f);

	if (slice.m_minLightIndex != lightVolume.m_slicesCount + 1)
	{
		const float2 normTileSize = float2(1.0f / lightVolume.m_tilesCountX, 1.0f / lightVolume.m_tilesCountY);
		const int2 tileCoords = float2(input.uv.x, 1.0f - input.uv.y) / normTileSize;
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
			}
		}
	}

	return resultColor;
}