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
	VSOutput output;

	output.position = float4(input.position, 1.0);
	output.color = float3(0.0, 0.0, 1.0);

	return output;
}