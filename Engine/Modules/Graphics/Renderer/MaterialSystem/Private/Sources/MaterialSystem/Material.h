#pragma once

#include "MaterialSystem/IMaterialSystem.h"

namespace VT
{
	class Material : public IMaterial
	{
	private:
		GraphicPipelineStateReference m_pipelineState = nullptr;

	public:
		Material(GraphicPipelineStateReference pipelineState)
			: m_pipelineState(pipelineState) {}

		virtual GraphicPipelineStateReference getPipelineState() const override { return m_pipelineState; }
	};

	class ManagedMaterial final : public Material
	{
	private:
		MaterialID m_id;

	public:
		ManagedMaterial(GraphicPipelineStateReference pipelineState, MaterialID id)
			: Material(pipelineState), m_id(id) {}
		~ManagedMaterial();

		MaterialID getID() const { return m_id; }
	};
}
