#pragma once

#include "GraphicResourceManager/ObjectHandles.h"
#include "Math/Vector.h"

namespace VT
{
	class RenderPassEnvironment;

	class GBuffer final
	{
	private:
		Texture2DReference m_color;
		Texture2DReference m_normal;
		Texture2DReference m_position;
		Texture2DReference m_depth;

		RenderTargetViewReference m_colorRTV;
		RenderTargetViewReference m_normalRTV;
		RenderTargetViewReference m_positionRTV;
		DepthStencilViewReference m_depthDSV;

		ShaderResourceViewReference m_colorSRV;
		ShaderResourceViewReference m_normalSRV;
		ShaderResourceViewReference m_positionSRV;
		ShaderResourceViewReference m_depthSRV;

	public:
		GBuffer() = default;

		bool init(const Vector2UInt16& bufferResolution);
		void release();

		void fillEnvironment(RenderPassEnvironment& environment) const;
	};
}