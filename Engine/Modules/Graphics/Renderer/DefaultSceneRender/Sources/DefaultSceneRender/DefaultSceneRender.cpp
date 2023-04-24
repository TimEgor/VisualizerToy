#include "DefaultSceneRender.h"

#include "PreparingRenderingDataSystem.h"

void VT::DefaultSceneRender::prepareRenderData(const ILevel& level)
{
	PreparingRenderingDataSystem::prepareData(level, getDefaultRenderingData());
}
