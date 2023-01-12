#pragma once

#include "ManagedGraphicDevice/ManagedGraphicObject.h"
#include "GraphicResourceCommon/GraphicResourceDescriptorBase.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedGraphicResourceDescriptorHandle = ObjectPoolHandle32;
	using ManagedGraphicResourceDescriptorObject = ManagedGraphicObject<ManagedGraphicResourceDescriptorHandle>;

	class ManagedGraphicResourceDescriptorBase : public GraphicResourceDescriptorBase, public ManagedGraphicResourceDescriptorObject
	{
	public:
		ManagedGraphicResourceDescriptorBase(VT::GraphicResourceDescriptorType type)
			: GraphicResourceDescriptorBase(type) {}
	};
}