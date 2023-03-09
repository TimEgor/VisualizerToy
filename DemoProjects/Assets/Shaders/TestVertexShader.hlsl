#include "GBuffer/GBufferVertexShaderBinding.hlsli"

struct VSInput
{
	float3 m_position : POSITION0;
	float3 m_normal : NORMAL0;
	float2 m_uv : TEXTURE0;
};

struct VSOutput
{
	float4 m_position : SV_POSITION;
	float3 m_worldPosition : POSITIONW;
	float3 m_worldNormal : NORMALW;
	float3 m_color : COLOR0;
};

VSOutput VS(VSInput input, uint vertexId : SV_VertexID)
{
	ConstantBuffer<CameraTransforms> cameraTransforms = ResourceDescriptorHeap[cameraTransformBufferBinding.m_bindingIndex];
	StructuredBuffer<ObjectTransform> objectTransforms = ResourceDescriptorHeap[perObjectTransformBufferBinding.m_bindingIndex];
	ObjectTransform objectTransform = objectTransforms[perObjectTransformBufferBinding.m_elementIndex];

	VSOutput output;

	output.m_position = mul(float4(input.m_position, 1.0f), objectTransform.m_worldTransform);
	output.m_worldPosition = output.m_position.xyz;

	output.m_position = mul(output.m_position, cameraTransforms.m_viewTransformMatrix);
	output.m_position = mul(output.m_position, cameraTransforms.m_projTransformMatrix);

	output.m_worldNormal = normalize(mul(float4(input.m_normal, 0.0f), objectTransform.m_worldTransform).xyz);
	output.m_color = float3(1.0, 1.0, 1.0);

	return output;
}