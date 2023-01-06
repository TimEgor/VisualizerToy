#pragma once

namespace VT
{
	struct MeshIndexData;
	struct MeshVertexData;

	class MeshLoader final
	{
	public:
		MeshLoader() = default;

		static void loadMesh(const void* meshResourceData, size_t meshResourceDataSize, MeshVertexData& vertexData, MeshIndexData& indexData);
	};
}
