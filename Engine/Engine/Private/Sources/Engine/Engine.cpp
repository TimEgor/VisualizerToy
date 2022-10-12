#include "Engine.h"

bool VT::Engine::init()
{
	return false;
}

void VT::Engine::release()
{}

void VT::Engine::run()
{}

void VT::Engine::updateFrame()
{}

void VT::Engine::startTimer()
{}

void VT::Engine::stop()
{}

bool VT::Engine::isStoped() const
{
	return false;
}

void VT::Engine::pauseRendering(bool state)
{}

bool VT::Engine::isRenderingPaused() const
{
	return false;
}

VT::EngineEnvironment* VT::Engine::getEnvironment()
{
	return nullptr;
}
