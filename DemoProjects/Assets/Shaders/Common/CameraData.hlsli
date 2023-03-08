#ifndef CAMERA_DATA_HLSL
#define CAMERA_DATA_HLSL

struct CameraTransforms
{
	float4x4 m_viewTransformMatrix;
	float4x4 m_projTransformMatrix;

	float3 m_position;
	float m_nearPlane;
	float m_farPlane;
};

#endif //CAMERA_DATA_HLSL