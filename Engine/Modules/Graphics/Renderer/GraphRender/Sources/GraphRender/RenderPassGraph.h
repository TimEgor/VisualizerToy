#pragma once

#include <cstdint>
#include <vector>

#include "RenderPassEnvironment.h"

namespace VT
{
	class RenderPassGraphBuilder;
	class IRenderPass;

	using GraphPassID = uint32_t;
	constexpr GraphPassID InvalidGraphPassID = -1;

	class RenderPassGraph final
	{
		friend RenderPassGraphBuilder;

	public:
		using LayerResoureInfoHashType = uint32_t;

		struct RenderPass final
		{
			IRenderPass* m_pass = nullptr;
			GraphPassID m_id = InvalidGraphPassID;
		};

		struct LayerResoureInfo final
		{
			RenderPassEnvironmentResourceID m_id = 0;
			RenderPassEnvironmentResourceType m_type = RenderPassEnvironmentResourceType::UNKNOWN;

			LayerResoureInfoHashType m_hash = 0;

			LayerResoureInfo() = default;
			LayerResoureInfo(RenderPassEnvironmentResourceID id, RenderPassEnvironmentResourceType type)
				: m_id(id), m_type(type) { hash(*this); }

			static LayerResoureInfoHashType hash(const LayerResoureInfo& info);
		};

		using LayerRenderPassContainer = std::vector<RenderPass>;
		using LayerRenderPassResource = std::vector<LayerResoureInfo>;

		struct LayerData final
		{
			LayerRenderPassContainer m_passes;
			LayerRenderPassResource m_readingResoures;
			LayerRenderPassResource m_writingResoures;
		};

		using LayerDataContainer = std::vector<LayerData>;

	private:
		LayerDataContainer m_layers;

	public:
		RenderPassGraph() = default;

		void clear();

		const LayerDataContainer& getLayerData() const { return m_layers; }
		LayerDataContainer& getLayerData() { return m_layers; }
	};
}
