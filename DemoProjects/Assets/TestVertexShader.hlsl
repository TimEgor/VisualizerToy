static float2 positions[3] = {
	{0.0, 0.5},
	{0.5, -0.5},
	{-0.5, -0.5}
};

float4 VS(uint vertexId : SV_VertexID) : SV_POSITION
{
	return float4(positions[vertexId], 0.0, 1.0);
}