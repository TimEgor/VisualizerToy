#ifndef __VERTEX_SAHDER_COMMON__
#define __VERTEX_SAHDER_COMMON__

#include "CommonShaderBinding.hlsli"

ConstantBuffer<ElementBufferBindingInfo> perObjectTransformBufferBindingData : register(b1);

struct ObjectTransform
{
	float4x4 worldTransform;
};

#endif //__VERTEX_SAHDER_COMMON__