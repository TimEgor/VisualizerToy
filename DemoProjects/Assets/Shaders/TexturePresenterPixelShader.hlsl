struct PSInput
{
	float4 position : SV_POSITION;
    float2 uv : TEXTURE0;
};

struct TextureBindingInfo
{
	uint srcTextureIndex;
};

ConstantBuffer<TextureBindingInfo> textureIndex : register(b0);
SamplerState textureSampler : register(s0);

float4 PS(PSInput input) : SV_TARGET0
{
	Texture2D<float4> scrTexture = ResourceDescriptorHeap[textureIndex.srcTextureIndex];
	return scrTexture.Sample(textureSampler, input.uv);
}