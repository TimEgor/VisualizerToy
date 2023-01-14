#pragma once

#include "Core/HashFunctions/StdHashCombine.h"
#include "GraphicResourceCommon/Format.h"
#include "IPipelineBindingLayout.h"

namespace VT
{
	enum class Format;
	class IVertexShader;
	class IPixelShader;

	using PipelineStateHash = uint32_t;

	struct PipelineStateInfo final
	{
		using TargetFormats = std::vector<Format>;
		TargetFormats m_formats;

		IVertexShader* m_vertexShader = nullptr;
		IPixelShader* m_pixelShader = nullptr;

		PipelineStateHash getHash() const
		{
			PipelineStateHash result = 0;
			hashCombine(result, m_vertexShader);
			hashCombine(result, m_pixelShader);

			for (Format format : m_formats)
			{
				hashCombine(result, format);
			}

			return result;
		}
	};

	class IPipelineState
	{
	public:
		IPipelineState() = default;
		virtual ~IPipelineState() = default;

		virtual PipelineStateHash getHash() const = 0;
		virtual PipelineBindingLayoutHash getBindingLayoutHash() const = 0;
	};
}
