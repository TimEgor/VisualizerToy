#pragma once

#include "ObjectPool/ObjectPoolHandle.h"

namespace VT::ManagedGraphicDevice
{
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
}