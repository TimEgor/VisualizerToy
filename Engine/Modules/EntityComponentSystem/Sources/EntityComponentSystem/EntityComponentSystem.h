#pragma once

#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <entt/entt.hpp>

namespace VT
{
	using VT_Entity = entt::entity;
	constexpr VT_Entity InvalidEntity = VT_Entity{};

	class EntityComponentSystem final
	{
	private:
		entt::registry m_registry;

	public:
		EntityComponentSystem() = default;
		~EntityComponentSystem() { release(); }

		bool init();
		void release();

		VT_Entity createEntity();
		void destroyEntity(VT_Entity entity);

		template <typename ComponentType, typename... Args>
		ComponentType& addComponent(VT_Entity entity, Args&&... args)
		{
			ComponentType& component = m_registry.emplace<ComponentType>(entity, std::forward<Args>(args)...);
			return component;
		}

		template <typename ComponentType, typename... Args>
		ComponentType& addOrReplaceComponent(VT_Entity entity, Args&&... args)
		{
			ComponentType& component = m_registry.emplace_or_replace<ComponentType>(entity, std::forward<Args>(args)...);
			return component;
		}

		template <typename ComponentType>
		ComponentType& getComponent(VT_Entity entity)
		{
			ComponentType& component = m_registry.get<ComponentType>(entity);
			return component;
		}

		template <typename ComponentType>
		const ComponentType& getComponent(VT_Entity entity) const
		{
			const ComponentType& component = m_registry.get<ComponentType>(entity);
			return component;
		}

		template <typename... ComponentTypes>
		bool hasAllOfComponent(VT_Entity entity) const
		{
			return m_registry.all_of<ComponentTypes...>(entity);
		}

		template <typename... ComponentTypes>
		bool hasAnyOfComponent(VT_Entity entity) const 
		{
			return m_registry.any_of<ComponentTypes...>(entity);
		}

		template <typename ComponentType>
		void removeComponent(VT_Entity entity)
		{
			m_registry.remove<ComponentType>(entity);
		}

		template <typename... ComponentTypes>
		auto getView() const
		{
			return m_registry.view<ComponentTypes...>();
		}
	};
}
