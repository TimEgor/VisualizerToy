#pragma once

#include "Core/UtilitiesMacros.h"
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
		~EngineInstance() { reset(); }

		inline IEngine* operator->()
		{
			assert(m_engine);
			return m_engine;
		}

		inline IEngine* getEngine()
		{
			assert(m_engine);
			return m_engine;
		}
		inline void setEngine(IEngine* engine)
		{
			assert(engine);
			m_engine = engine;
		}
		inline void reset()
		{
			m_engine = nullptr;
		}
	};
}