#pragma once

#include "GraphicResourceManager/ResourceHandles.h"
#include "GraphicResourceManager/InputLayoutHandle.h"

#include <cstdint>
#include <vector>

namespace VT
{
	struct MeshVertexData final
	{
		using MeshVertexBindingContainer = std::vector<GPUBufferReference>;

		MeshVertexBindingContainer m_bindings;
		InputLayoutReference m_inputLayout = nullptr;
		uint32_t m_vertexCount = 0;
	};

	struct MeshIndexData final
	{
		GPUBufferReference m_indexBuffer = nullptr;
		InputLayoutElementType m_indexFormat = InputLayoutElementType::UNDEFINED;
		uint32_t m_indexCount = 0;
	};

	class IMesh
	{
	public:
		IMesh() = default;
		virtual ~IMesh() {}

		virtual const MeshVertexData& getVertexData() const = 0;
		virtual const MeshIndexData& getIndexData() const = 0;
	};
}