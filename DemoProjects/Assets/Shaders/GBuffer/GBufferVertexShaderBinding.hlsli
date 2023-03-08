#ifndef GBUFFER_SHADER_BINDING_HLSL
#define GBUFFER_SHADER_BINDING_HLSL

#include "GBufferShaderBinding.hlsli"

ConstantBuffer<ElementBinding> perObjectTransformBufferBinding : register(b1);

struct ObjectTransform
{
	float4x4 m_worldTransform;
};

#endif //GBUFFER_SHADER_BINDING_HLSL