#ifdef SPIRV
#define INPUT_LAYOUT(index) [[vk::location(index)]]  
#else
#define INPUT_LAYOUT(index)
#endif

struct VSInput
{
	INPUT_LAYOUT(0) float2 position : POSITION0;
	INPUT_LAYOUT(1) float3 color : COLOR0;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	INPUT_LAYOUT(0) float3 color : COLOR0;
};

VSOutput VS(VSInput input, uint vertexId : SV_VertexID)
{
	VSOutput output;

	output.position = float4(input.position, 0.0, 1.0);
	output.color = input.color;

	return output;
}