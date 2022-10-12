#include "TestPlugin.h"

VT_PLUGIN(TestPlugin)

void TestPlugin::onLoaded()
{
	OutputDebugString("TestPlugin -> onLoaded()\n");
}

void TestPlugin::onUnloaded()
{
	OutputDebugString("TestPlugin -> onUnloaded()\n");
}