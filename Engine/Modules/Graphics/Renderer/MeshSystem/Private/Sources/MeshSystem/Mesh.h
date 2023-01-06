#pragma once

#include "MeshSystem/IMesh.h"
#include "MeshSystem/IMeshSystem.h"

namespace VT
{
	class Mesh : public IMesh
	{
	private:
		MeshVertexData m_vertexData;
		MeshIndexData m_indexData;

	public:
		Mesh(const MeshVertexData& vertData)
			: m_vertexData(vertData) {}
		Mesh(const MeshVertexData& vertData, const MeshIndexData& indexData)
			: m_vertexData(vertData), m_indexData(indexData) {}

		virtual const MeshVertexData& getVertexData() const override { return m_vertexData; }
		virtual const MeshIndexData& getIndexData() const override { return m_indexData; }
	};

	class ManagedMesh final : public Mesh
	{
	private:
		MeshID m_id;

	public:
		ManagedMesh(const MeshVertexData& vertData, MeshID id)
			: Mesh(vertData), m_id(id) {}
		ManagedMesh(const MeshVertexData& vertData, const MeshIndexData& indexData, MeshID id)
			: Mesh(vertData, indexData), m_id(id) {}

		MeshID getID() const { return m_id; }
	};
}
