#include "CommonGBufferVertexShaderBinding.hlsli"

struct VSInput
{
	float3 position : POSITION0;
	float3 normal : NORMAL0;
	float2 uv : TEXTURE0;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITIONW;
	float3 worldNormal : NORMALW;
	float3 color : COLOR0;
};

VSOutput VS(VSInput input, uint vertexId : SV_VertexID)
{
	ConstantBuffer<CameraTransforms> cameraTransforms = ResourceDescriptorHeap[cameraTransformBufferBindingData.bufferIndex];
	StructuredBuffer<ObjectTransform> objectTransforms = ResourceDescriptorHeap[perObjectTransformBufferBindingData.bufferIndex];
	ObjectTransform objectTransform = objectTransforms[perObjectTransformBufferBindingData.elementIndex];

	VSOutput output;

	output.position = mul(float4(input.position, 1.0f), objectTransform.worldTransform);
	output.worldPosition = output.position.xyz;

	output.position = mul(output.position, cameraTransforms.viewTransformMatrix);
	output.position = mul(output.position, cameraTransforms.projTransformMatrix);

	output.worldNormal = normalize(mul(float4(input.normal, 0.0f), objectTransform.worldTransform).xyz);
	output.color = float3(0.0, 0.0, 1.0);

	return output;
}