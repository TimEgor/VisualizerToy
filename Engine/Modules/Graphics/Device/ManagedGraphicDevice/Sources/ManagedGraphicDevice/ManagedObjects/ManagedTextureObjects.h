#pragma once

#include "ManagedGraphicDevice/ManagedGraphicObject.h"
#include "Textures/Texture2DBase.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedTexture2DHandle = ObjectPoolHandle32;
	using ManagedTexture2DObject = ManagedGraphicObject<ManagedTexture2DHandle>;

	class ManagedTexture2DBase : public Texture2DBase, public ManagedTexture2DObject
	{
	public:
		ManagedTexture2DBase(const Texture2DDesc& desc) : Texture2DBase(desc) {}
	};
}