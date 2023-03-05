struct VSInput
{
	float2 position : POSITION0;
	float2 uv : TEXTURE0;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float2 uv : TEXTURE0;
};

VSOutput VS(VSInput input)
{
	VSOutput output;

	output.position = float4(input.position, 0.0, 1.0);
	output.uv = input.uv;

	return output;
}