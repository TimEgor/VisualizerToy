#pragma once

#include "Mesh.h"
#include "MeshSystem/MeshHandle.h"

namespace VT
{
	class ManagedMeshHandle : public MeshHandle
	{
	protected:
		ManagedMesh* m_mesh = nullptr;
		MeshHandleID m_id = 0;

		virtual void selfDestroy() override;

	public:
		ManagedMeshHandle(ManagedMesh* mesh, MeshHandleID id)
			: m_mesh(mesh), m_id(id) {}

		void setMesh(ManagedMesh* mesh) { m_mesh = mesh; }

		virtual IMesh* getMesh() const override { return m_mesh; }
		virtual MeshHandleID getID() const override { return m_id; }
	};

	class NamedManagedMeshHandle final : public ManagedMeshHandle
	{
	private:
		MeshNameID m_nameID;

		virtual void selfDestroy() override;

	public:
		NamedManagedMeshHandle(ManagedMesh* mesh, MeshHandleID id, MeshNameID nameID)
			: ManagedMeshHandle(mesh, id), m_nameID(nameID) {}

		MeshNameID getNameID() const { return m_nameID; }
	};
}
