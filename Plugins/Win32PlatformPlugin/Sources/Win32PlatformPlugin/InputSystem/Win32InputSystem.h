#pragma once

#include "InputSystem/IInputSystem.h"

#include <unordered_map>

namespace VT_WIN32
{
	class Win32InputSystem final : public VT::IInputSystem
	{
		using KeyMapContainer = std::unordered_map<VT::Key, size_t>;

	private:
		KeyMapContainer m_keyMaps;

		VT::Vector2UInt16 m_mousePos = VT::Vector2UInt16Zero;
		VT::Vector2UInt16 m_prevMousePos = VT::Vector2UInt16Zero;

		uint8_t* m_keys = nullptr;

	public:
		Win32InputSystem() = default;
		~Win32InputSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual void update() override;

		virtual bool isKeyDown(VT::Key key) const override;
		virtual bool isKeyUp(VT::Key key) const override;

		virtual VT::Vector2UInt16 getMousePosition() const override { return m_mousePos; }
		virtual VT::Vector2UInt16 getPrevMousePosition() const override { return m_prevMousePos; }
		virtual VT::Vector2Int16 getMouseMovementOffset() const override;
		virtual bool isMouseMoved() const override;
	};
}
