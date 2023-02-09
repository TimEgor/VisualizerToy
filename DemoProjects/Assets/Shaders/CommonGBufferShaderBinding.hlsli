#ifndef __SHADER_COMMON_BINDINGS__
#define __SHADER_COMMON_BINDINGS__

struct ElementBufferBindingInfo
{
	uint bufferIndex;
	uint elementIndex;
};

struct UniqueBufferBindingInfo
{
	uint bufferIndex;
};

ConstantBuffer<UniqueBufferBindingInfo> cameraTransformBufferBindingData : register(b0);

//

struct CameraTransforms
{
	float4x4 viewTransformMatrix;
	float4x4 projTransformMatrix;
};

#endif //__SHADER_COMMON_BINDINGS__