#include "MeshLoader.h"


#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "GPUBuffer/IGPUBuffer.h"
#include "MeshSystem/MeshResouceData.h"
#include "MeshSystem/IMesh.h"

#include "GraphicResourceManager/IGraphicResourceManager.h"

namespace VT
{
	inline IGraphicResourceManager* getGraphicResourceManager()
	{
		return EngineInstance::getInstance()->getEnvironment()->m_graphicResourceManager;
	}
}

void VT::MeshLoader::loadMesh(const void* meshResourceData, size_t meshResourceDataSize,
	MeshVertexData& vertexData, MeshIndexData& indexData)
{
	IGraphicResourceManager* graphicResourceManager = getGraphicResourceManager();

	const MeshResourceDataHeader* meshHeader = reinterpret_cast<const MeshResourceDataHeader*>(meshResourceData);
	const void* inputLayoutData = reinterpret_cast<const uint8_t*>(meshResourceData) + sizeof(MeshResourceDataHeader);

	InputLayoutDesc inputLayoutDesc;
	InputLayoutDesc::deserialize(inputLayoutDesc, inputLayoutData, meshHeader->m_inputLayoutSize);
	vertexData.m_inputLayout = graphicResourceManager->addInputLayout(inputLayoutDesc);

	vertexData.m_vertexCount = meshHeader->m_vertexCount;

	const uint8_t* vertData = reinterpret_cast<const uint8_t*>(inputLayoutData) + meshHeader->m_inputLayoutSize;

	{
		GPUBufferDesc vertexBufferDesc{};
		vertexBufferDesc.m_usage = GPUBufferUsageType::VERTEX_BUFFER;

		const uint32_t bindingsCount = inputLayoutDesc.m_bindings.size();
		vertexData.m_bindings.reserve(bindingsCount);
		for (uint32_t i = 0; i < bindingsCount; ++i)
		{
			const InputLayoutBindingDesc& bindingDesc = inputLayoutDesc.m_bindings[i];

			vertexBufferDesc.m_byteSize = bindingDesc.m_stride * vertexData.m_vertexCount;
			GPUBufferReference vertexBuffer = graphicResourceManager->createGPUBuffer(vertexBufferDesc);

			void* mappingVertData = nullptr;
			vertexBuffer->getBuffer()->mapData(&mappingVertData);
			memcpy(mappingVertData, vertData, vertexBufferDesc.m_byteSize);
			vertexBuffer->getBuffer()->unmapData();

			vertexData.m_bindings.emplace_back(vertexBuffer);

			vertData += vertexBufferDesc.m_byteSize;
		}
	}

	{
		indexData.m_indexCount = meshHeader->m_indexCount;
		indexData.m_indexFormat = meshHeader->m_indexSize == 2 ? InputLayoutElementType::UINT16 : InputLayoutElementType::UINT32;

		GPUBufferDesc indexBufferDesc{};
		indexBufferDesc.m_usage = GPUBufferUsageType::INDEX_BUFFER;
		indexBufferDesc.m_byteSize = indexData.m_indexCount * meshHeader->m_indexSize;

		indexData.m_indexBuffer = graphicResourceManager->createGPUBuffer(indexBufferDesc);

		void* mappingIndexData = nullptr;
		indexData.m_indexBuffer->getBuffer()->mapData(&mappingIndexData);
		memcpy(mappingIndexData, vertData, indexBufferDesc.m_byteSize);
		indexData.m_indexBuffer->getBuffer()->unmapData();
	}
}
