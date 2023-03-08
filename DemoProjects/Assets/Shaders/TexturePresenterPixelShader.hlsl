struct PSInput
{
	float4 m_position : SV_POSITION;
	float2 m_uv : TEXTURE0;
};

struct TextureBindingInfo
{
	uint m_srcTextureIndex;
};

ConstantBuffer<TextureBindingInfo> textureBinding : register(b0);
SamplerState textureSampler : register(s0);

float4 PS(PSInput input) : SV_TARGET0
{
	Texture2D<float4> scrTexture = ResourceDescriptorHeap[textureBinding.m_srcTextureIndex];
	return scrTexture.Sample(textureSampler, input.m_uv);
}