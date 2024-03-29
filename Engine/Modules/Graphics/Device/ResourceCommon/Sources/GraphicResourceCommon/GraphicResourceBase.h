#pragma once

#include "IGraphicResource.h"

namespace VT
{
	class GraphicResourceBase : public IGraphicResource
	{
	protected:
		FenceValueType m_usingFenceValues[static_cast<uint32_t>(GraphicResourceUsingMarkType::COUNT)];

		GraphicResourceStateValueType m_state = CommonGraphicResourceState::GRAPHIC_STATE_UNDEFINED;

	public:
		GraphicResourceBase()
		{
			m_usingFenceValues[static_cast<uint32_t>(GraphicResourceUsingMarkType::GRAPHIC)] = 0;
			m_usingFenceValues[static_cast<uint32_t>(GraphicResourceUsingMarkType::COPY)] = 0;
		}

		virtual bool isUsing(GraphicResourceUsingMarkType markType, FenceValueType fenceVal) const override
		{
			return m_usingFenceValues[static_cast<uint32_t>(markType)] >= fenceVal;
		}

		virtual void markUsage(GraphicResourceUsingMarkType markType, FenceValueType fenceVal) override
		{
			FenceValueType& currentFenceVal = m_usingFenceValues[static_cast<uint32_t>(markType)];
			if (currentFenceVal < fenceVal)
			{
				currentFenceVal = fenceVal;
			}
		}

		virtual GraphicResourceStateValueType getState() const override { return m_state; }
		virtual void setState(GraphicResourceStateValueType state) override { m_state = state; }
	};
}
