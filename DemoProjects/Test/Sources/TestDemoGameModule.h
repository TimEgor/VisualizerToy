#pragma once

#include "GameSystem/GameModuleBase.h"
#include "EntityComponentSystem/EntityComponentSystem.h"
#include "Scene/Node.h"

namespace VT_DEMO_TEST
{
	class TestDemoGameModule final : public VT::GameModuleBase
	{
	private:
		VT::NodeID m_cubeNodeID = VT::InvalidNodeID;
		VT::VT_Entity m_cameraEntity = VT::InvalidEntity;
		float m_time = 0.0f;

	public:
		TestDemoGameModule(VT::PluginID id, VT_PLATFORM_HANDLE_MODULE nativeHandle)
			: GameModuleBase(id, nativeHandle) {}

		virtual void onLoaded() override;
		virtual void onUnloaded() override;

		virtual void update(float deltaTime) override;
	};
}
