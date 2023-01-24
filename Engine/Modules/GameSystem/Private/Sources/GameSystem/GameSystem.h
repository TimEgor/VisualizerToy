#pragma once

#include "GameSystem/IGameSystem.h"

#include <unordered_map>

namespace VT
{
	class GameSystem final : public IGameSystem
	{
		using ModuleIndicesCollection = std::unordered_map<GameModuleID, size_t>;
		using ModulesCollection = std::vector<IGameModule*>;

	private:
		ModuleIndicesCollection m_indices;
		ModulesCollection m_modules;

	public:
		GameSystem() = default;
		~GameSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual void update(float deltaTime) override;

		virtual IGameModule* loadGameModule(const FileName& modulePath) override;
		virtual void unloadGameModule(const FileName& modulePath) override;
		virtual void unloadGameModule(GameModuleID moduleID) override;

		virtual IGameModule* getGameModule(const FileName& modulePath) override;
		virtual IGameModule* getGameModule(GameModuleID moduleID) override;
	};
}
