#pragma once

#include "Core/Platform.h"
#include "PluginBase.h"

#include <type_traits>
#include <cassert>

#define VT_PLUGIN_CREATE_FUNC VTPluginCreate
#define VT_PLUGIN_RELEASE_FUNC VTPluginRelease

#define VT_PLUGIN_CREATE(PLUGIN_TYPE)																							\
extern "C" VT_DLL_EXPORT VT::PluginBase* VT_PLUGIN_CREATE_FUNC(VT::PluginID pluginID, VT_PLATFORM_HANDLE_MODULE nativeHandle)	\
{																																\
	static_assert(std::is_base_of<VT::PluginBase, PLUGIN_TYPE>::value);															\
	return new PLUGIN_TYPE(pluginID, nativeHandle);																				\
}

#define VT_PLUGIN_RELEASE(PLUGIN_TYPE)																							\
extern "C" VT_DLL_EXPORT void VT_PLUGIN_RELEASE_FUNC(PLUGIN_TYPE* plugin)														\
{																																\
	static_assert(std::is_base_of<VT::PluginBase, PLUGIN_TYPE>::value);															\
	delete plugin;																												\
}

#define VT_PLUGIN(PLUGIN_TYPE)	\
VT_PLUGIN_CREATE(PLUGIN_TYPE)	\
VT_PLUGIN_RELEASE(PLUGIN_TYPE)