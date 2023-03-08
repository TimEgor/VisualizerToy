#ifndef GBUFFER_BINDING_HLSL
#define GBUFFER_BINDING_HLSL

#include "../Common/CommonBindingTypes.hlsli"
#include "../Common/CameraData.hlsli"

ConstantBuffer<UniqueBinding> cameraTransformBufferBinding : register(b0);

#endif //GBUFFER_BINDING_HLSL