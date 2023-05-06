#ifndef DIR_LIGHT_HLSL
#define DIR_LIGHT_HLSL

#define MAX_DIR_LIGHTS 32

struct DirectionalLight
{
	float3 m_color;
	float m_intensity;
	float3 m_dir;
};

#endif