#ifndef LIGHT_VOLUME_HLSL
#define LIGHT_VOLUME_HLSL

static const uint LIGHT_VOLUME_TILE_DIM_SIZE = 32;

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

struct TileDepth
{
	float m_min;
	float m_max;
};

#endif //LIGHT_VOLUME_HLSL