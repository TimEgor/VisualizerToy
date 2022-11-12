#pragma once

#include "SwapChain/SwapChainBase.h"
#include "Textures/Texture2DBase.h"
#include "ObjectPool/ObjectPoolHandle.h"

namespace VT::ManagedGraphicDevice
{
	using ManagedSwapChainHandle = ObjectPoolHandle8;
	using ManagedTexture2DHandle = ObjectPoolHandle32;

	class ManagedGraphicDevice;

	template <typename ObjectHandleType>
	class ManagedGraphicObject
	{
		friend ManagedGraphicDevice;

	public:
		using HandleType = ObjectHandleType;

	private:
		ObjectHandleType m_handle = 0;

	public:
		ManagedGraphicObject() = default;
		virtual ~ManagedGraphicObject() {}

		ObjectHandleType getHandle() const { return m_handle; }

	};

	using ManagedSwapChainObject = ManagedGraphicObject<ManagedSwapChainHandle>;
	using ManagedTexture2DObject = ManagedGraphicObject<ManagedTexture2DHandle>;

	class ManagedSwapChainBase : public SwapChainBase, public ManagedSwapChainObject
	{
	public:
		ManagedSwapChainBase(const SwapChainDesc& desc) : SwapChainBase(desc) {}
	};

	class ManagedTexture2DBase : public Texture2DBase, public ManagedTexture2DObject
	{
	public:
		ManagedTexture2DBase(const Texture2DDesc& desc) : Texture2DBase(desc) {}
	};
}