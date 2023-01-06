#pragma once

#include "Core/HashFunctions/Crc32.h"

#include <cstdint>

namespace VT
{
	struct MeshResourceDataHeader final
	{
		const uint32_t m_headerMagicNum = crc32("MeshResHeader");
		uint32_t m_inputLayoutSize = 0;
		uint32_t m_vertexCount = 0;
		uint32_t m_indexCount = 0;
		uint32_t m_indexSize = 0;
	};
}
