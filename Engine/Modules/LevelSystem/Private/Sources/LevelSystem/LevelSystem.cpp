#include "LevelSystem.h"

bool VT::LevelSystem::init()
{
	VT_CHECK_INITIALIZATION(m_levelPool.init(32, 0, 16));
	return true;
}

void VT::LevelSystem::release()
{
	m_levelPool.release();
}

VT::ILevel* VT::LevelSystem::createLevel()
{
	LevelPool::NewElementInfo newLevelInfo = m_levelPool.addElementRaw();
	return new (newLevelInfo.m_elementPtr) Level(newLevelInfo.m_elementHandle.getKey());
}

void VT::LevelSystem::destroyLevel(ILevel* level)
{
	m_levelPool.removeElement(level->getID());
}

VT::ILevel* VT::LevelSystem::getLevel(LevelID levelID)
{
	return m_levelPool.getElement(levelID);
}

void VT::LevelSystem::setCurrentLevel(LevelID levelID)
{
	m_currentLevelID = levelID;
}

void VT::LevelSystem::setCurrentLevel(ILevel* level)
{
	setCurrentLevel(level->getID());
}

VT::ILevel* VT::LevelSystem::getCurrentLevel()
{
	return m_levelPool.getElement(m_currentLevelID);
}
