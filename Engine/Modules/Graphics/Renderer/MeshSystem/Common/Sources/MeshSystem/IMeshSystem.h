#pragma once

#include "MeshHandle.h"
#include "IMesh.h"
#include "Core/FileName/FileName.h"

#include <cstdint>


namespace VT
{
	using MeshID = uint32_t;
	using MeshName = FileName;
	using MeshNameID = FileNameID;

	class IMeshSystem
	{
	public:
		using OnLoadedMeshCallback = std::function<void(MeshReference)>;

	public:
		IMeshSystem() = default;
		virtual ~IMeshSystem() {}

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual MeshReference createMesh(const MeshVertexData& vertData) = 0;
		virtual MeshReference createMesh(const MeshVertexData& vertData, const MeshIndexData& indexData) = 0;
		virtual MeshReference getMesh(MeshHandleID id) = 0;
		virtual MeshReference getNamedMesh(const MeshName& name) = 0;
		virtual MeshReference getNamedMesh(MeshNameID id) = 0;

		virtual MeshReference loadMesh(const MeshName& name) = 0;
		virtual MeshReference loadMeshAsync(const MeshName& name, OnLoadedMeshCallback callback) = 0;
	};
}
