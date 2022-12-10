#pragma once

#include <wrl/client.h>
#include <dxcapi.h>

using Microsoft::WRL::ComPtr;

#define IID(object) __uuidof(object)
#define PPV(object)	reinterpret_cast<void**>(&object)
#define IID_COM(objectPtr) __uuidof(objectPtr.Get())
#define PPV_COM(objectPtr) reinterpret_cast<void**>(objectPtr.GetAddressOf())