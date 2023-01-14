#pragma once

#include <d3d12.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>

#include <dxgi1_6.h>

#include <wrl/client.h>

#define VT_D3D12ObjectRelease(objectComPtr)		\
	if (objectComPtr)							\
	{											\
		objectComPtr->Release();				\
		objectComPtr = nullptr;					\
	}

#define VT_IID(objectPtr) __uuidof(objectPtr)
#define VT_PPV(objectPtr)	reinterpret_cast<void**>(&objectPtr)
#define VT_IID_COM(objectComPtr) __uuidof(objectComPtr.Get())
#define VT_PPV_COM(objectComPtr) reinterpret_cast<void**>(objectComPtr.GetAddressOf())

namespace VT_D3D12
{
	template <typename ComObj>
	using ComPtr = Microsoft::WRL::ComPtr<ComObj>;
}