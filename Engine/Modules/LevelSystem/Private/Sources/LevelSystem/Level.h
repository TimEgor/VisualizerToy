#pragma once

#include "LevelSystem/ILevel.h"
#include "Scene/Scene.h"

#include <unordered_map>

namespace VT
{
	class Level final : public ILevel
	{
		using NodeEntityCollection = std::unordered_map<NodeID, VT_Entity>;

	private:
		NodeEntityCollection m_nodeEnities;

		EntityComponentSystem* m_entityComponentSystem = nullptr;
		Scene* m_scene = nullptr;

		LevelID m_id = 0;

		VT_Entity m_cameraEntity = InvalidEntity;

	public:
		Level(LevelID id)
			: m_id(id) {}
		~Level() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual VT_Entity createEntity() override;
		virtual VT_Entity createEntity(VT_Entity parentEntity) override;
		virtual void removeEntity(VT_Entity entity) override;

		virtual const EntityComponentSystem* getEntityComponentSystem() const override { return m_entityComponentSystem; }
		virtual EntityComponentSystem* getEntityComponentSystem() override { return m_entityComponentSystem; }
		virtual const IScene* getScene() const override { return m_scene; }
		virtual IScene* getScene() override { return m_scene; }

		virtual void setCameraEntity(VT_Entity entity) override;
		virtual VT_Entity getCameraEntity() const override;

		virtual LevelID getID() const override { return m_id; }
	};
}
