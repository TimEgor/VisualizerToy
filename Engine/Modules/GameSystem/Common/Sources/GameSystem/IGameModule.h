#pragma once

#include <cstdint>

namespace VT
{
	using GameModuleID = uint32_t;

	class IGameModule
	{
	public:
		IGameModule() = default;
		virtual ~IGameModule() = default;

		virtual void update(float deltaTime) = 0;

		virtual GameModuleID getModuleID() const = 0;
	};
}
