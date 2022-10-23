#include "Engine/EngineInstance.h"
#include "Engine/IEngine.h"
#include "Engine/EngineEnvironment.h"

#include "Win32Platform.h"

inline Win32Platform* getWin32EnvironmentPlatform()
{
	VT::IPlatform* platform = VT::EngineInstance::getInstance()->getEnvironment()->m_platform;
	assert(platform->getType() == Win32Platform::getPlatformType());

	return reinterpret_cast<Win32Platform*>(platform);
}