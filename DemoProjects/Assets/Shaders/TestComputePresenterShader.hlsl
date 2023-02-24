struct TextureBindingInfo
{
	uint srcTextureIndex;
	uint dstTextureIndex;
};

ConstantBuffer<TextureBindingInfo> textureIndices : register(b0);

[numthreads(10, 10, 1)]
void CS(int3 dispatchThreadID : SV_DispatchThreadID)
{
    Texture2D srcTexture = ResourceDescriptorHeap[textureIndices.srcTextureIndex];
    RWTexture2D<float4> dstTexture = ResourceDescriptorHeap[textureIndices.dstTextureIndex];

    dstTexture[dispatchThreadID.xy] = srcTexture[dispatchThreadID.xy];
}