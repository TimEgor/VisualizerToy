#pragma once

#include "IGraphicResourceDescriptor.h"

namespace VT
{
	class GraphicResourceDescriptorBase : public IGraphicResourceDescriptor
	{
	private:
		GraphicResourceDescriptorType m_type;

	public:
		GraphicResourceDescriptorBase(GraphicResourceDescriptorType type)
			: m_type(type) {}

		virtual GraphicResourceDescriptorType getType() const { return m_type; }
	};
}
