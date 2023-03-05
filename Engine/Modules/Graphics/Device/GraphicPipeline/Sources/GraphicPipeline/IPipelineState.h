#pragma once

#include "Core/HashFunctions/StdHashCombine.h"
#include "GraphicResourceCommon/Format.h"
#include "IPipelineBindingLayout.h"

namespace VT
{
	enum class Format;
	class IVertexShader;
	class IPixelShader;
	class IComputeShader;

	using PipelineStateHash = uint32_t;

	enum class GraphicComparisonFunc
	{
		NEVER,
		EQUAL,
		NOT_EQUAL,
		LESS,
		LESS_EQUAL,
		GREATER,
		GREATER_EQUAL,
		ALWAYS
	};

	struct GraphicPipelineStateInfo final
	{
		using TargetFormats = std::vector<Format>;

		struct DepthStensilTestInfo final
		{
			Format m_format = Format::UNDEFINED;
			GraphicComparisonFunc m_comparisonFunc = GraphicComparisonFunc::LESS;
			bool m_enabled = false;
		};


		TargetFormats m_targetFormats;

		DepthStensilTestInfo m_depthStencilTest;

		IVertexShader* m_vertexShader = nullptr;
		IPixelShader* m_pixelShader = nullptr;

		PipelineStateHash getHash() const
		{
			PipelineStateHash result = 0;
			hashCombine(result, m_vertexShader);
			hashCombine(result, m_pixelShader);

			for (Format format : m_targetFormats)
			{
				hashCombine(result, format);
			}

			return result;
		}
	};

	struct ComputePipelineStateInfo final
	{
		IComputeShader* m_computeShader = nullptr;

		PipelineStateHash getHash() const
		{
			PipelineStateHash result = 0;
			hashCombine(result, m_computeShader);

			return result;
		}
	};

	enum class PipelineStateType
	{
		GRAPHIC,
		COMPUTE
	};

	class IPipelineState : public IGraphicObject
	{
	public:
		IPipelineState() = default;

		virtual PipelineStateHash getHash() const = 0;
		virtual PipelineBindingLayoutHash getBindingLayoutHash() const = 0;

		virtual PipelineStateType getType() const = 0;
	};

	class IGraphicPipelineState : public IPipelineState
	{
	public:
		IGraphicPipelineState() = default;

		virtual PipelineStateType getType() const override { return PipelineStateType::GRAPHIC; }
	};

	class IComputePipelineState : public IPipelineState
	{
	public:
		IComputePipelineState() = default;

		virtual PipelineStateType getType() const override { return PipelineStateType::COMPUTE; }
	};
}
