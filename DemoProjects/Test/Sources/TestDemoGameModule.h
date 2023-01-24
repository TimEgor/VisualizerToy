#pragma once

#include "GameSystem/GameModuleBase.h"

namespace VT
{
	struct NodeTransforms;
}

namespace VT_DEMO_TEST
{
	class TestDemoGameModule final : public VT::GameModuleBase
	{
	private:
		VT::NodeTransforms* m_transforms = nullptr;

	public:
		TestDemoGameModule(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: GameModuleBase(id, nativeHandle) {}

		virtual void onLoaded() override;
		virtual void onUnloaded() override;

		virtual void update(float deltaTime) override;
	};
}
