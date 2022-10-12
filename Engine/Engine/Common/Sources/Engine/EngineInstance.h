#pragma once

#include "Core/Paterns/Singleton.h"

#include <cassert>

namespace VT
{
	class IEngine;

	class EngineInstance final : public Singleton<EngineInstance>
	{
	private:
		IEngine* m_engine = nullptr;

	public:
		EngineInstance() = default;

		inline IEngine* operator->() { return m_engine; }

		inline IEngine* getEngine() { return m_engine; }
		inline void setEngine(IEngine* engine)
		{
			assert(engine);
			m_engine = engine;
		}
	};
}