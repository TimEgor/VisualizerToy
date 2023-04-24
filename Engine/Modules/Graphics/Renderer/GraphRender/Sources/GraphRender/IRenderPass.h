#pragma once

#include "Core/TypeHashMacros.h"
#include "Render/RenderDrawingContext.h"

#include <cassert>

namespace VT
{
	class IRenderContext;
	class RenderPassEnvironment;

	class ITexture2D;
	class IGraphicResourceDescriptor;

	class RenderPassGraphBuilder;

	using RenderPassDataType = HashTyped::Type;

	class IRenderPassData : public HashTyped
	{
	public:
		IRenderPassData() = default;
		virtual ~IRenderPassData() {}

		template <typename DataType>
		DataType& getData()
		{
			static_assert(std::is_base_of<IRenderPassData, DataType>::value);
			assert(getType() == DataType::getRenderPassDataType());

			return *reinterpret_cast<DataType*>(this);
		}

		template <typename DataType>
		const DataType& getData() const
		{
			return const_cast<const DataType&>(
				const_cast<IRenderPassData*>(this)->getData<DataType>());
		}

		virtual RenderPassDataType getType() const = 0;
	};

	class IRenderPass
	{
	public:
		IRenderPass() = default;
		virtual ~IRenderPass() = default;

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual void fillRenderPassDependency(RenderPassGraphBuilder& builder) const = 0;

		virtual void execute(RenderDrawingContext& context, const RenderPassEnvironment& environment, const IRenderPassData* data) = 0;
	};
}

#define VT_RENDER_PASS_DATA_TYPE(DATA_TYPE) VT_HASH_TYPE(#DATA_TYPE, VT::RenderPassDataType, RenderPassData)