#pragma once

#include "InputSystem/IInputSystem.h"

#include <unordered_map>

namespace VT_WIN32
{
	class Win32InputSystem final : public VT::IInputSystem
	{
		using KeyMapVTContainer = std::unordered_map<VT::Key, size_t>;
		using KeyMapWin32Container = std::unordered_map<size_t, VT::Key>;

	private:
		KeyMapVTContainer m_vtKeyMap;
		KeyMapWin32Container m_win32KeyMap;

		VT::Vector2UInt16 m_mousePos = VT::Vector2UInt16Zero;
		VT::Vector2UInt16 m_prevMousePos = VT::Vector2UInt16Zero;
		VT::Vector2Int16 m_mouseMovementDelta = VT::Vector2Int16Zero;

		uint8_t* m_keys = nullptr;

		void fillVTKeyMaps();
		void fillWin32KeyMaps();

	public:
		Win32InputSystem() = default;
		~Win32InputSystem() { release(); }

		virtual bool init() override;
		virtual void release() override;

		virtual void update() override;

		virtual bool isKeyDown(VT::Key key) const override;
		virtual bool isKeyUp(VT::Key key) const override;

		virtual const VT::Vector2UInt16& getMousePosition() const override { return m_mousePos; }
		virtual const VT::Vector2UInt16& getPrevMousePosition() const override { return m_prevMousePos; }
		virtual const VT::Vector2Int16& getMouseMovementOffset() const override { return m_mouseMovementDelta; }
		virtual bool isMouseMoved() const override;

		VT::Key convertKeyWin32ToVt(size_t win32Key);

		VT_INPUT_SYSTEM_TYPE(WIN32_INPUT_SYSTEM_TYPE);
	};
}
