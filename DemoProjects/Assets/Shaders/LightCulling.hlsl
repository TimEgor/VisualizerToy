#define MAX_POINT_LIGHTS 256
#define TILE_MASK_SIZE MAX_POINT_LIGHTS / 32

struct PointLightBuffers
{
	uint m_pointLightsCount;

	uint m_lightBufferIndex;
	uint m_tileMaskBufferIndex;
};

struct UniqueBufferBindingInfo
{
	uint m_bufferIndex;
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
ConstantBuffer<PointLightBuffers> lightBuffersBinding : register(b2);

[numthreads(16, 1, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
	uint lightIndex = dispatchThreadID.x;
	if (lightIndex >= lightBuffersBinding.m_pointLightsCount)
	{
		return;
	}

	ConstantBuffer<CameraTransforms> cameraTransforms = ResourceDescriptorHeap[cameraTransformsBinding.m_bufferIndex];
	ConstantBuffer<LightVolumeInfo> lightVolume = ResourceDescriptorHeap[lightVolumeInfoBinding.m_bufferIndex];

	StructuredBuffer<PointLight> pointLights = ResourceDescriptorHeap[lightBuffersBinding.m_lightBufferIndex];
	RWStructuredBuffer<TileMask> tileMasks = ResourceDescriptorHeap[lightBuffersBinding.m_tileMaskBufferIndex];
	
	const PointLight light = pointLights[lightIndex];

	const float4 lightViewPosition = mul(float4(light.m_position, 1.0f), cameraTransforms.m_viewTransformMatrix);
	const float4 lightRangeDir = float4(light.m_range, light.m_range, light.m_range, 0.0f);

	const float4 minLightViewPosition = lightViewPosition - lightRangeDir;
	const float4 maxLightViewPosition = lightViewPosition + lightRangeDir;

	//const float2 lightProjPosition = (mul(lightViewPosition, cameraTransforms.m_projTransformMatrix)).xy;
	const float4 minLightProjPosition = mul(minLightViewPosition, cameraTransforms.m_projTransformMatrix);
	const float4 maxLightProjPosition = mul(maxLightViewPosition, cameraTransforms.m_projTransformMatrix);

	//const float2 normLightScreenPosition = (lightProjPosition + float2(1.0f, 1.0f)) / 2.0f;
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