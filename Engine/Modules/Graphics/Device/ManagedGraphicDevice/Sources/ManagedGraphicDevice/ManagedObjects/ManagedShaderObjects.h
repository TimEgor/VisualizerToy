#pragma once

#include "ManagedGraphicDevice/ManagedGraphicObject.h"
#include "Shaders/IShaders.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedShaderHandle = ObjectPoolHandle32;
	using ManagedShaderObject = ManagedGraphicObject<ManagedShaderHandle>;

	class ManagedVertexShaderBase : public IVertexShader, public ManagedShaderObject
	{
	public:
		ManagedVertexShaderBase() = default;
	};

	class ManagedPixelShaderBase : public IPixelShader, public ManagedShaderObject
	{
	public:
		ManagedPixelShaderBase() = default;
	};
}