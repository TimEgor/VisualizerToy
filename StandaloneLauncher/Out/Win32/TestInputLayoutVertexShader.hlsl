#include "CommonVertexShaderBinding.hlsli"

struct VSInput
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXTURE0;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 color : COLOR0;
};

VSOutput VS(VSInput input, uint vertexId : SV_VertexID)
{
	ConstantBuffer<CameraTransforms> cameraTransforms = ResourceDescriptorHeap[cameraTransformBufferBindingData.bufferIndex];
	StructuredBuffer<ObjectTransform> objectTransforms = ResourceDescriptorHeap[perObjectTransformBufferBindingData.bufferIndex];
	ObjectTransform objectTransform = objectTransforms[perObjectTransformBufferBindingData.elementIndex];

	VSOutput output;

	output.position = mul(float4(input.position, 1.0f), objectTransform.worldTransform);
	output.position = mul(output.position, cameraTransforms.viewTransformMatrix);
	output.position = mul(output.position, cameraTransforms.projTransformMatrix);
	output.color = float3(0.0, 0.0, 1.0);

	return output;
}