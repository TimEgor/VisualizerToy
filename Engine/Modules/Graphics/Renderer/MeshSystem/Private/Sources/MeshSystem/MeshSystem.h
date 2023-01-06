#pragma once

#include "MeshSystem/IMeshSystem.h"
#include "ManagedMeshHandle.h"
#include "NamedReferencePool/NamedObjectPool.h"
#include "NamedReferencePool/NamedObjectPoolHandle.h"
#include "ObjectPool/ThreadSafeObjectPool.h"

namespace VT
{
	struct MeshIndexData;
	struct MeshVertexData;

	class MeshSystem final : public IMeshSystem
	{
		friend ManagedMeshHandle;
		friend NamedManagedMeshHandle;

	private:
		using MeshPool = ThreadSafeObjectPool<ManagedMesh, ObjectPoolHandle32>;
		using MeshHandlePool = ThreadSafeObjectPool<ManagedMeshHandle, NamedObjectPoolHandle32>;
		using NamedMeshHandlePool = NamedObjectPool<NamedManagedMeshHandle, MeshReference, NamedObjectPoolHandle32>;

		MeshPool m_meshes;
		MeshHandlePool m_handles;
		NamedMeshHandlePool m_namedHandles;

		void deleteMesh(ManagedMesh* mesh);
		void deleteMeshHandle(MeshHandleID id);
		void deleteNamedMeshHandle(FileNameID nameID);

	public:
		MeshSystem() = default;
		~MeshSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual MeshReference createMesh(const MeshVertexData& vertData) override;
		virtual MeshReference createMesh(const MeshVertexData& vertData, const MeshIndexData& indexData) override;
		virtual MeshReference getMesh(MeshHandleID id) override;
		virtual MeshReference getNamedMesh(const MeshName& name) override;
		virtual MeshReference getNamedMesh(MeshNameID id) override;

		virtual MeshReference loadMesh(const MeshName& name) override;
		virtual MeshReference loadMeshAsync(const MeshName& name, OnLoadedMeshCallback callback) override;
	};
}
