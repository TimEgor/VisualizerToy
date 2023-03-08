struct VSInput
{
	float2 m_position : POSITION0;
	float2 m_uv : TEXTURE0;
};

struct VSOutput
{
	float4 m_position : SV_POSITION;
	float2 m_uv : TEXTURE0;
};

VSOutput VS(VSInput input)
{
	VSOutput output;

	output.m_position = float4(input.m_position, 0.0, 1.0);
	output.m_uv = input.m_uv;

	return output;
}