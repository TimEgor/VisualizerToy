#pragma once

namespace VT
{
	class IGraphicObject
	{
	public:
		IGraphicObject() = default;
		virtual ~IGraphicObject() = default;

		virtual void* getNativeHandle() const = 0;

		template<typename T>
		T* getNativeHandleCast() const { return reinterpret_cast<T*>(getNativeHandle()); }
	};
}
