struct PSInput
{
	float4 m_position : SV_POSITION;
	float3 m_worldPosition : POSITIONW;
	float3 m_worldNormal : NORMALW;
	float3 m_color : COLOR0;
};

struct PSOutput
{
	float4 m_color : SV_TARGET0;
	float4 m_normal : SV_TARGET1;
	float4 m_position : SV_TARGET2;
};

PSOutput PS(PSInput input)
{
	PSOutput output;

	output.m_color = float4(input.m_color, 1.0);
	output.m_normal = float4(input.m_worldNormal, 1.0);
	output.m_position = float4(input.m_worldPosition, 1.0);

	return output;
}