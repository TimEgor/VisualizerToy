#pragma once

#include <d3dcommon.h>

#include <dxcapi.h>
#include <dxgi1_6.h>

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

#define IID(object) __uuidof(object)
#define PPV(object)	reinterpret_cast<void**>(&object)
#define IID_COM(objectPtr) __uuidof(objectPtr.Get())
#define PPV_COM(objectPtr) reinterpret_cast<void**>(objectPtr.GetAddressOf())