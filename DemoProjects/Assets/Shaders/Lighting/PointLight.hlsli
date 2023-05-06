#ifndef POINT_LIGHT_HLSL
#define POINT_LIGHT_HLSL

#define MAX_POINT_LIGHTS 256
#define POINT_LIGHT_TILE_MASK_SIZE MAX_POINT_LIGHTS / 32

struct PointLight
{
	float3 m_color;
	float m_range;
	float3 m_position;
	float m_intensity;
};

struct PointLightTileMask
{
	uint m_mask[POINT_LIGHT_TILE_MASK_SIZE];
};

#endif