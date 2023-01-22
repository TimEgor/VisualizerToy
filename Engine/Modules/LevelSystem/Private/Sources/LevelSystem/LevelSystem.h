#pragma once

#include "Level.h"
#include "LevelSystem/ILevelSystem.h"
#include "ObjectPool/ObjectPool.h"

namespace VT
{
	constexpr LevelID InvalidLevelID = 0;

	class LevelSystem final : public ILevelSystem
	{
		using LevelPool = ObjectPool<Level, ObjectPoolHandle32>;

	private:
		LevelPool m_levelPool;
		LevelID m_currentLevelID = InvalidLevelID;
		
	public:
		LevelSystem() = default;
		~LevelSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual ILevel* createLevel() override;
		virtual void destroyLevel(ILevel* level) override;
		virtual ILevel* getLevel(LevelID levelID) override;

		virtual ILevel* getCurrentLevel() override;
	};
}
