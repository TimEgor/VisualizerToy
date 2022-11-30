#pragma once

#include "Core/UtilitiesMacros.h"

#include <functional>

namespace VT
{
	class ResourceDependencyState: public CountableObjectBase
	{
	public:
		using Callback = std::function<void()>;

	protected:
		Callback m_callback;

	public:
		ResourceDependencyState(Callback callback)
			: m_callback(callback) {}
		~ResourceDependencyState() { VT_SAFE_CALL(m_callback); }
	};

	COUNTABLE_REFERENCES_DECLARATION(ResourceDependencyState);
}