#include "FbxHierData.h"

#include "FileSystem/IFileSystem.h"
#include "Core/FileName/FileName.h"
#include "MeshSystem/MeshResouceData.h"

#include <cassert>

namespace VT
{
	struct MeshResourceDataHeader;
}

VT_MODEL_RC::ModelNodeIndex VT_MODEL_RC::ModelHier::addNode(ModelNodeIndex parentNodeIndex, FbxNode* node)
{
	ModelNodeIndex nodeIndex = m_nodes.size();
	m_nodes.push_back(ModelNode{ node, parentNodeIndex });

	return nodeIndex;
}

void VT_MODEL_RC::ModelHier::addMesh(ModelNodeIndex nodeIndex, FbxMesh* mesh)
{
	m_meshes.push_back({ mesh, nodeIndex });
}

void VT_MODEL_RC::ModelHier::processNode(FbxNode* node, ModelNodeIndex parentNodeIndex)
{
	assert(node);

	ModelNodeIndex nodeIndex = addNode(parentNodeIndex, node);

	FbxMesh* nodeMesh = node->GetMesh();
	if (nodeMesh)
	{
		addMesh(nodeIndex, nodeMesh);
	}

	int nodeChildCount = node->GetChildCount();
	for (int childIndex = 0; childIndex < nodeChildCount; ++childIndex)
	{
		FbxNode* childNode = node->GetChild(childIndex);
		processNode(childNode, nodeIndex);
	}
}

void VT_MODEL_RC::ModelHier::saveHier(VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath)
{
	//VT::IFileSystem::IWriteStream* stream = outFileSystem.openBinaryWriteStream(outFilePath + "/hier");



	//outFileSystem.closeWriteStream(stream);
}

void VT_MODEL_RC::ModelHier::saveMeshes(VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath)
{
	const VT::FileName outDir = outFilePath + "_";

	if (!m_meshes.empty())
	{
		outFileSystem.createDirectory(outDir);
	}

	for (const ModelMesh& mesh : m_meshes)
	{
		VT::FileName meshName = m_nodes[mesh.m_nodeIndex].m_fbxNode->GetName();
		meshName += ".mesh";

		VT::IFileSystem::IWriteStream* stream = outFileSystem.openBinaryWriteStream(outDir + "/" + meshName);

		saveMesh(stream, mesh);

		outFileSystem.closeWriteStream(stream);
	}
}

void VT_MODEL_RC::ModelHier::prepareMeshData(FbxMesh* fbxMesh, MeshData& meshData)
{
	assert(fbxMesh);

	FbxVector4* positions = fbxMesh->GetControlPoints();

	FbxGeometryElementNormal* elementNormal = fbxMesh->GetElementNormal();
	if (!elementNormal ||
		(elementNormal->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			elementNormal->GetMappingMode() != FbxGeometryElement::eByControlPoint)
		)
	{
		assert(false && "Normal element or mapping is invalid.");
		return;
	}

	FbxGeometryElementUV* elementUV = fbxMesh->GetElementUV();
	if (!elementUV ||
		(elementUV->GetMappingMode() != FbxGeometryElement::eByPolygonVertex &&
			elementUV->GetMappingMode() != FbxGeometryElement::eByControlPoint))
	{
		assert(false && "UV element or mapping is invalid.");
		return;
	}

	MeshData::IndexType index = 0;

	int polygonCount = fbxMesh->GetPolygonCount();
	for (int polygonIndex = 0; polygonIndex < polygonCount; ++polygonIndex)
	{
		int verticesCount = fbxMesh->GetPolygonSize(polygonIndex);
		for (int vertexIndex = 0; vertexIndex < verticesCount; ++vertexIndex)
		{
			MeshData::IndexType globalVertexIndex = fbxMesh->GetPolygonVertex(polygonIndex, vertexIndex);

			//Index
			meshData.m_indices.push_back(index++);

			//Position
			FbxVector4& meshVertPosition = positions[globalVertexIndex];
			meshData.m_positions.emplace_back(
				static_cast<float>(meshVertPosition[0]),
				static_cast<float>(meshVertPosition[1]),
				static_cast<float>(meshVertPosition[2])
			);

			//Normal
			int normalIndex = 0;
			FbxLayerElement::EReferenceMode normalReferenceMode = elementNormal->GetReferenceMode();
			if (normalReferenceMode == FbxGeometryElement::eDirect)
			{
				normalIndex = globalVertexIndex;
			}
			else
			{
				normalIndex = elementNormal->GetIndexArray().GetAt(globalVertexIndex);
			}

			FbxVector4 meshVertNormal = elementNormal->GetDirectArray().GetAt(normalIndex);
			meshData.m_normals.emplace_back(
				static_cast<float>(meshVertNormal[0]),
				static_cast<float>(meshVertNormal[1]),
				static_cast<float>(meshVertNormal[2])
			);

			//UV
			int uvIndex = 0;
			FbxLayerElement::EReferenceMode uvReferenceMode = elementUV->GetReferenceMode();
			if (uvReferenceMode == FbxGeometryElement::eDirect)
			{
				uvIndex = globalVertexIndex;
			}
			else
			{
				uvIndex = elementUV->GetIndexArray().GetAt(globalVertexIndex);
			}

			FbxVector2 meshVertUV = elementUV->GetDirectArray().GetAt(uvIndex);
			meshData.m_UVs.emplace_back(
				static_cast<float>(meshVertNormal[0]),
				static_cast<float>(meshVertNormal[1])
			);
		}
	}
}

void VT_MODEL_RC::ModelHier::saveMesh(VT::IFileSystem::IWriteStream* stream, const ModelMesh& mesh)
{
	FbxMesh* fbxMesh = mesh.m_fbxMesh;

	static const size_t vertexCount = 50'000;

	MeshData meshData;
	meshData.m_positions.reserve(vertexCount);
	meshData.m_normals.reserve(vertexCount);
	meshData.m_UVs.reserve(vertexCount);

	meshData.m_indices.reserve(vertexCount * 3);

	prepareMeshData(fbxMesh, meshData);

	void* inputLayoutData = nullptr;
	size_t inputLayoutDataSize = 0;
	VT::InputLayoutDesc::serialize(meshData.m_desc, &inputLayoutData, inputLayoutDataSize);

	VT::MeshResourceDataHeader meshHeader;
	meshHeader.m_inputLayoutSize = inputLayoutDataSize;
	meshHeader.m_vertexCount = static_cast<uint32_t>(meshData.m_positions.size());
	meshHeader.m_indexCount = static_cast<uint32_t>(meshData.m_indices.size());
	meshHeader.m_indexSize = static_cast<uint32_t>(sizeof(MeshData::IndexType));

	stream->write(&meshHeader, sizeof(VT::MeshResourceDataHeader));
	stream->write(inputLayoutData, inputLayoutDataSize);

	stream->write(meshData.m_positions.data(), meshData.m_positions.size() * sizeof(VT::Vector3));
	stream->write(meshData.m_normals.data(), meshData.m_normals.size() * sizeof(VT::Vector3));
	stream->write(meshData.m_UVs.data(), meshData.m_UVs.size() * sizeof(VT::Vector2));

	stream->write(meshData.m_indices.data(), meshData.m_indices.size() * sizeof(MeshData::IndexType));
}

void VT_MODEL_RC::ModelHier::buildHier(FbxNode* rootNode)
{
	processNode(rootNode, -1);
}

void VT_MODEL_RC::ModelHier::save(VT::IFileSystem& outFileSystem, const VT::FileName& outFilePath)
{
	if (m_nodes.empty())
	{
		return;
	}

	//saveHier(outFileSystem, outFilePath);
	saveMeshes(outFileSystem, outFilePath);
}
