struct PSInput
{
	float4 position : SV_POSITION;
	float3 worldPosition : POSITIONW;
	float3 worldNormal : NORMALW;
	float3 color : COLOR0;
};

struct PSOutput
{
	float4 color : SV_TARGET0;
	float4 normal : SV_TARGET1;
	float4 position : SV_TARGET2;
};

PSOutput PS(PSInput input)
{
	PSOutput output;

	output.color = float4(input.color, 1.0);
	output.normal = float4(input.worldNormal, 1.0);
	output.position = float4(input.worldPosition, 1.0);

	return output;
}