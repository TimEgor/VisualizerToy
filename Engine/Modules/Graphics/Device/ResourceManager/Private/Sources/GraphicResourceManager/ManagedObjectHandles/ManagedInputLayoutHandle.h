#pragma once

#include "GraphicResourceManager/InputLayoutHandle.h"

namespace VT
{
	class ManagedInputLayoutHandle final : public InputLayoutHandle
	{
	private:
		virtual void selfDestroy() override;

	public:
		ManagedInputLayoutHandle(const InputLayoutDesc& desc, InputLayoutHandleID id)
			: InputLayoutHandle(desc, id) {}
	};
}