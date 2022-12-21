#pragma once

#include "Core/HashFunctions/StdHashCombine.h"

namespace VT
{
	enum class Format;
	using PipelineStateHash = uint32_t;

	struct PipelineStateInfo final
	{
		IVertexShader* m_vertexShader = nullptr;
		IPixelShader* m_pixelShader = nullptr;

		PipelineStateHash getHash() const
		{
			PipelineStateHash result = 0;
			hashCombine(result, m_vertexShader);
			hashCombine(result, m_pixelShader);

			return result;
		}
	};

	class IPipelineState
	{
	public:
		IPipelineState() = default;
		virtual ~IPipelineState() = default;

		virtual PipelineStateHash getHash() const = 0;
	};
}