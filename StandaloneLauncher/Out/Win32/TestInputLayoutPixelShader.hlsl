#ifdef SPIRV
#define INPUT_LAYOUT(index) [[vk::location(index)]]  
#else
#define INPUT_LAYOUT(index)
#endif

struct PSInput
{
	float4 position : SV_POSITION;
	INPUT_LAYOUT(0) float3 color : COLOR0;
};

float4 PS(PSInput input) : SV_TARGET0
{
	return float4(input.color, 1.0);
}