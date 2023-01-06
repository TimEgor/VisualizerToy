#pragma once

#include <vector>

#include "FileSystem/IFileSystem.h"
#include "InputLayout/InputLayout.h"
#include "Math/Vector.h"

#include "fbxsdk.h"

namespace VT
{
	class IFileSystem;
	class FileName;
}

namespace VT_MODEL_RC
{
	using ModelNodeIndex = uint32_t;

	struct ModelNode final
	{
		FbxNode* m_fbxNode = nullptr;
		ModelNodeIndex m_parent = -1;
	};

	struct ModelMesh final
	{
		FbxMesh* m_fbxMesh = nullptr;
		ModelNodeIndex m_nodeIndex = -1;
	};

	class ModelHier final
	{
		struct MeshData final
		{
			using IndexType = uint32_t;

			std::vector<VT::Vector3> m_positions;
			std::vector<VT::Vector3> m_normals;
			std::vector<VT::Vector2> m_UVs;

			std::vector<IndexType> m_indices;

			const VT::InputLayoutDesc m_desc = {
				{
					{ 0, 0, 0, 3, VT::InputLayoutElementType::FLOAT32 },
					{ 1, 1, 0, 3, VT::InputLayoutElementType::FLOAT32 },
					{ 2, 2, 0, 2, VT::InputLayoutElementType::FLOAT32 }
				},
				{
					{ 0, 3 * VT::getInputLayoutElementTypeSize(VT::InputLayoutElementType::FLOAT32), VT::InputLayoutBindingType::VERTEX_BINDING },
					{ 1, 3 * VT::getInputLayoutElementTypeSize(VT::InputLayoutElementType::FLOAT32), VT::InputLayoutBindingType::VERTEX_BINDING },
					{ 2, 2 * VT::getInputLayoutElementTypeSize(VT::InputLayoutElementType::FLOAT32), VT::InputLayoutBindingType::VERTEX_BINDING }
				}
			};
		};

	private:
		std::vector<ModelNode> m_nodes;
		std::vector<ModelMesh> m_meshes;

		ModelNodeIndex addNode(ModelNodeIndex parentNodeIndex, FbxNode* node);
		void addMesh(ModelNodeIndex nodeIndex, FbxMesh* mesh);

		void processNode(FbxNode* node, ModelNodeIndex parentNodeIndex);

		void saveHier(VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath);
		void saveNodesInfo(VT::IFileSystem::IWriteStream* stream, const ModelNode& node);
		void saveNodeTransforms();

		void saveMeshes(VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath);
		void prepareMeshData(FbxMesh* fbxMesh, MeshData& meshData);
		void saveMesh(VT::IFileSystem::IWriteStream* stream, const ModelMesh& mesh);

	public:
		ModelHier() = default;

		void buildHier(FbxNode* rootNode);

		void save(VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath);
	};
}
