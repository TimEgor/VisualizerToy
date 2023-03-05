#if defined(WIN32) || defined(_WIN32)
#include "PluginLoaderWin32.h"

#include "Core/String/Format.h"
#include "Core/FileName/FileNameUtils.h"

void VT::PluginLoaderWin32::outputLoadingError(const FileName& name)
{
#ifdef _MSC_VER
	LPVOID lpMsgBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	OutputDebugStringA(stringFormat("LoadLibrary \"%s\" error message:\n %s\n", name.c_str(), lpMsgBuf).c_str());
#endif // _MSC_VER
}

VT::IPlugin* VT::PluginLoaderWin32::load(const FileName& name, VT::PluginID id)
{
	FileName dirPath;
	FileNameUtils::getFileDirPath(name, dirPath);
	
	SetDllDirectory(dirPath.c_str());

	VT_PLATFORM_HANDLE_MODULE nativeHandle = LoadLibrary(name.c_str());
	if (nativeHandle == nullptr)
	{
		outputLoadingError(name);
		return nullptr;
	}

	PluginCreatingFunctionPtr creatorPtr = reinterpret_cast<PluginCreatingFunctionPtr>(GetProcAddress(nativeHandle, VT_PLUGIN_CREATE_FUNC_NAME));

	return creatorPtr(id, nativeHandle, EngineInstance::getInstance().getEngine());
}

void VT::PluginLoaderWin32::unload(IPlugin* plugin)
{
	VT_PLATFORM_HANDLE_MODULE nativeHandle = plugin->getNativeHandle();
	PluginReleasingFunctionPtr releasorPtr = reinterpret_cast<PluginReleasingFunctionPtr>(GetProcAddress(nativeHandle, VT_PLUGIN_RELEASE_FUNC_NAME));
	releasorPtr(plugin);

	FreeLibrary(nativeHandle);
}

#endif