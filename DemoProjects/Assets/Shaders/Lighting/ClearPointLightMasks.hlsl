#include "PointLight.hlsli"
#include "LightVolume.hlsli"

#include "../Common/CommonBindingTypes.hlsli"

ConstantBuffer<UniqueBinding> lightVolumeInfoBinding : register(b0);
ConstantBuffer<UniqueBinding> tileMaskBinding : register(b1);

[numthreads(8, 8, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    ConstantBuffer<LightVolumeInfo> lightVolume = ResourceDescriptorHeap[lightVolumeInfoBinding.m_bindingIndex];

	if (dispatchThreadID.x >= lightVolume.m_tilesCountX || dispatchThreadID.y >= lightVolume.m_tilesCountY)
	{
		return;
	}

    RWStructuredBuffer<PointLightTileMask> tileMasks = ResourceDescriptorHeap[tileMaskBinding.m_bindingIndex];

    const uint tileIndex = dispatchThreadID.y * lightVolume.m_tilesCountX + dispatchThreadID.x;

	for (uint maskIndex = 0; maskIndex < POINT_LIGHT_TILE_MASK_SIZE; ++maskIndex)
	{
        tileMasks[tileIndex].m_mask[maskIndex] = 0;
	}
}