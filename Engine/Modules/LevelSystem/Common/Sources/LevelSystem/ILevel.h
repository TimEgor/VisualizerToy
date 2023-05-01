#pragma once

#include "EntityComponentSystem/EntityComponentSystem.h"

namespace VT
{
	class IScene;

	using LevelID = uint32_t;

	class ILevel
	{
	public:
		ILevel() = default;
		virtual ~ILevel() = default;

		virtual bool init() = 0;
		virtual void release() = 0;

		virtual VT_Entity createEntity() = 0;
		virtual VT_Entity createEntity(VT_Entity parentEntity) = 0;
		virtual void removeEntity(VT_Entity entity) = 0;

		virtual const EntityComponentSystem* getEntityComponentSystem() const = 0;
		virtual EntityComponentSystem* getEntityComponentSystem() = 0;
		virtual const IScene* getScene() const = 0;
		virtual IScene* getScene() = 0;

		virtual void setCameraEntity(VT_Entity entity) = 0;
		virtual VT_Entity getCameraEntity() const = 0;

		virtual LevelID getID() const = 0;
	};
}
