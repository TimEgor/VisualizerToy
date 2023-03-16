#include "LightVolume.hlsli"

#include "../Common/CommonBindingTypes.hlsli"
#include "../Common/Type.hlsli"

ConstantBuffer<UniqueBinding> depthSourceBinding : register(b0);
ConstantBuffer<UniqueBinding> lightVolumeInfoBinding : register(b1);
ConstantBuffer<UniqueBinding> lightVolumeDepthBinding : register(b2);
SamplerState textureSampler : register(s0);

groupshared uint groupDepthMin;
groupshared uint groupDepthMax;

static const uint LOCAL_TILE_GROUP_ITERATION_COUNT = LIGHT_VOLUME_TILE_DIM_SIZE / 8;

[numthreads(8, 8, 1)]
void CS(uint3 dispatchThreadID : SV_DispatchThreadID, uint3 groupThreadID : SV_GroupThreadID, uint3 groupID : SV_GroupID)
{
	Texture2D<float> depthSource = ResourceDescriptorHeap[depthSourceBinding.m_bindingIndex];

	uint2 depthSourceDimensions;
	depthSource.GetDimensions(depthSourceDimensions.x, depthSourceDimensions.y);

	if (groupThreadID.x == 0 && groupThreadID.y == 0)
	{
		groupDepthMin = UINT_MAX;
		groupDepthMax = 0;
	}

	float localDepthMin = FLT_MAX;
	float localDepthMax = 0.0f;

	for (uint localIteration = 0; localIteration < LOCAL_TILE_GROUP_ITERATION_COUNT * LOCAL_TILE_GROUP_ITERATION_COUNT; ++localIteration)
	{
		uint2 depthPixelCoord = dispatchThreadID.xy
			* uint2(LOCAL_TILE_GROUP_ITERATION_COUNT, LOCAL_TILE_GROUP_ITERATION_COUNT)
			+ uint2(localIteration % LOCAL_TILE_GROUP_ITERATION_COUNT, localIteration / LOCAL_TILE_GROUP_ITERATION_COUNT);

		depthPixelCoord = min(depthPixelCoord, depthSourceDimensions - 1);

		const float sourceDepth = depthSource[depthPixelCoord];

		localDepthMin = min(localDepthMin, sourceDepth);
		localDepthMax = max(localDepthMax, sourceDepth);
	}

	uint minDepthVal = asuint(localDepthMin);
	uint maxDepthVal = asuint(localDepthMax);

	GroupMemoryBarrierWithGroupSync();

	uint prevVal = 0;
	InterlockedMin(groupDepthMin, minDepthVal, prevVal);
	InterlockedMax(groupDepthMax, maxDepthVal, prevVal);

	GroupMemoryBarrierWithGroupSync();

	float minDepth = asfloat(groupDepthMin);
	float maxDepth = asfloat(groupDepthMax);

	if (groupThreadID.x == 0 && groupThreadID.y == 0)
	{
		ConstantBuffer<LightVolumeInfo> lightVolume = ResourceDescriptorHeap[lightVolumeInfoBinding.m_bindingIndex];
		RWStructuredBuffer<TileDepth> tilesDepth = ResourceDescriptorHeap[lightVolumeDepthBinding.m_bindingIndex];

		const uint tileIndex = groupID.y * lightVolume.m_tilesCountX + groupID.x;
		tilesDepth[tileIndex].m_min = minDepth;
		tilesDepth[tileIndex].m_max = maxDepth;
	}
}