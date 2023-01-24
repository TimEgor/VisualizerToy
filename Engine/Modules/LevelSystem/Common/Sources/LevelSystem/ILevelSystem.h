#pragma once

#include "ILevel.h"

namespace VT
{
	class ILevelSystem
	{
	public:
		ILevelSystem() = default;
		virtual ~ILevelSystem() = default;

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual ILevel* createLevel() = 0;
		virtual void destroyLevel(ILevel* level) = 0;
		virtual ILevel* getLevel(LevelID levelID) = 0;

		virtual void setCurrentLevel(LevelID levelID) = 0;
		virtual void setCurrentLevel(ILevel* level) = 0;
		virtual ILevel* getCurrentLevel() = 0;
	};
}
