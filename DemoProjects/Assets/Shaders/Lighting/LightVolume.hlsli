#ifndef LIGHT_VOLUME_HLSL
#define LIGHT_VOLUME_HLSL

struct LightVolumeInfo
{
	uint m_tilesCountX;
	uint m_tilesCountY;
	uint m_slicesCount;
};

struct ZSlice
{
	uint m_minLightIndex;
	uint m_maxLightIndex;
};

#endif //LIGHT_VOLUME_HLSL