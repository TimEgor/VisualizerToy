#include "Win32InputSystem.h"

#include "Core/UtilitiesMacros.h"
#include "Win32PlatformPlugin/Win32Platform.h"

constexpr size_t KeyMapSize = 256;

bool VT_WIN32::Win32InputSystem::init()
{
	m_keys = new uint8_t[KeyMapSize];
	memset(m_keys, 0, KeyMapSize * sizeof(uint8_t));

	m_keyMaps.reserve(KeyMapSize);

	m_keyMaps[VT::Key::MOUSE_RIGHT] = VK_RBUTTON;
	m_keyMaps[VT::Key::MOUSE_LEFT] = VK_LBUTTON;
	m_keyMaps[VT::Key::MOUSE_MIDDLE] = VK_MBUTTON;
	m_keyMaps[VT::Key::BACK_SPACE] = VK_BACK;
	m_keyMaps[VT::Key::DEL] = VK_RBUTTON;
	m_keyMaps[VT::Key::TAB] = VK_TAB;
	m_keyMaps[VT::Key::CLEAR] = VK_CLEAR;
	m_keyMaps[VT::Key::RETURN] = VK_RETURN;
	m_keyMaps[VT::Key::PAUSE] = VK_PAUSE;
	m_keyMaps[VT::Key::ESC] = VK_ESCAPE;
	m_keyMaps[VT::Key::SPACE] = VK_SPACE;
	m_keyMaps[VT::Key::KEY_PAD_0] = VK_NUMPAD0;
	m_keyMaps[VT::Key::KEY_PAD_1] = VK_NUMPAD1;
	m_keyMaps[VT::Key::KEY_PAD_2] = VK_NUMPAD2;
	m_keyMaps[VT::Key::KEY_PAD_3] = VK_NUMPAD3;
	m_keyMaps[VT::Key::KEY_PAD_4] = VK_NUMPAD4;
	m_keyMaps[VT::Key::KEY_PAD_5] = VK_NUMPAD5;
	m_keyMaps[VT::Key::KEY_PAD_6] = VK_NUMPAD6;
	m_keyMaps[VT::Key::KEY_PAD_7] = VK_NUMPAD7;
	m_keyMaps[VT::Key::KEY_PAD_8] = VK_NUMPAD8;
	m_keyMaps[VT::Key::KEY_PAD_9] = VK_NUMPAD9;
	m_keyMaps[VT::Key::DOT] = VK_OEM_PERIOD;
	m_keyMaps[VT::Key::COMMA] = VK_OEM_COMMA;
	m_keyMaps[VT::Key::DIVIDE] = VK_DIVIDE;
	m_keyMaps[VT::Key::MULTIPLE] = VK_MULTIPLY;
	m_keyMaps[VT::Key::MINUS] = VK_SUBTRACT;
	m_keyMaps[VT::Key::PLUS] = VK_ADD;
	m_keyMaps[VT::Key::ENTER] = VK_RETURN;
	m_keyMaps[VT::Key::ARROW_UP] = VK_UP;
	m_keyMaps[VT::Key::ARROW_DOWN] = VK_DOWN;
	m_keyMaps[VT::Key::ARROW_RIGHT] = VK_RIGHT;
	m_keyMaps[VT::Key::ARROW_LEFT] = VK_LEFT;
	m_keyMaps[VT::Key::INSERT] = VK_INSERT;
	m_keyMaps[VT::Key::HOME] = VK_HOME;
	m_keyMaps[VT::Key::END] = VK_END;
	m_keyMaps[VT::Key::PAGE_UP] = VK_PRIOR;
	m_keyMaps[VT::Key::PAGE_DOWN] = VK_NEXT;
	m_keyMaps[VT::Key::F1] = VK_F1;
	m_keyMaps[VT::Key::F2] = VK_F2;
	m_keyMaps[VT::Key::F3] = VK_F3;
	m_keyMaps[VT::Key::F4] = VK_F4;
	m_keyMaps[VT::Key::F5] = VK_F5;
	m_keyMaps[VT::Key::F6] = VK_F6;
	m_keyMaps[VT::Key::F7] = VK_F7;
	m_keyMaps[VT::Key::F8] = VK_F8;
	m_keyMaps[VT::Key::F9] = VK_F9;
	m_keyMaps[VT::Key::F10] = VK_F10;
	m_keyMaps[VT::Key::F11] = VK_F11;
	m_keyMaps[VT::Key::F12] = VK_F12;
	m_keyMaps[VT::Key::F13] = VK_F13;
	m_keyMaps[VT::Key::F14] = VK_F14;
	m_keyMaps[VT::Key::F15] = VK_F15;
	m_keyMaps[VT::Key::ALPHA_0] = 0x30;
	m_keyMaps[VT::Key::ALPHA_1] = 0x31;
	m_keyMaps[VT::Key::ALPHA_2] = 0x32;
	m_keyMaps[VT::Key::ALPHA_3] = 0x33;
	m_keyMaps[VT::Key::ALPHA_4] = 0x34;
	m_keyMaps[VT::Key::ALPHA_5] = 0x35;
	m_keyMaps[VT::Key::ALPHA_6] = 0x36;
	m_keyMaps[VT::Key::ALPHA_7] = 0x37;
	m_keyMaps[VT::Key::ALPHA_8] = 0x38;
	m_keyMaps[VT::Key::ALPHA_9] = 0x39;
	m_keyMaps[VT::Key::A] = 0x41;
	m_keyMaps[VT::Key::B] = 0x42;
	m_keyMaps[VT::Key::C] = 0x43;
	m_keyMaps[VT::Key::D] = 0x44;
	m_keyMaps[VT::Key::E] = 0x45;
	m_keyMaps[VT::Key::F] = 0x46;
	m_keyMaps[VT::Key::G] = 0x47;
	m_keyMaps[VT::Key::H] = 0x48;
	m_keyMaps[VT::Key::I] = 0x49;
	m_keyMaps[VT::Key::J] = 0x4A;
	m_keyMaps[VT::Key::K] = 0x4B;
	m_keyMaps[VT::Key::L] = 0x4C;
	m_keyMaps[VT::Key::M] = 0x4D;
	m_keyMaps[VT::Key::N] = 0x4E;
	m_keyMaps[VT::Key::O] = 0x4F;
	m_keyMaps[VT::Key::P] = 0x50;
	m_keyMaps[VT::Key::Q] = 0x51;
	m_keyMaps[VT::Key::R] = 0x52;
	m_keyMaps[VT::Key::S] = 0x53;
	m_keyMaps[VT::Key::T] = 0x54;
	m_keyMaps[VT::Key::U] = 0x55;
	m_keyMaps[VT::Key::V] = 0x56;
	m_keyMaps[VT::Key::W] = 0x57;
	m_keyMaps[VT::Key::X] = 0x58;
	m_keyMaps[VT::Key::Y] = 0x59;
	m_keyMaps[VT::Key::Z] = 0x5A;
	m_keyMaps[VT::Key::COLONS] = VK_OEM_1;
	m_keyMaps[VT::Key::BRACKETS_RIGHT] = VK_OEM_4;
	m_keyMaps[VT::Key::BRACKETS_LEFT] = VK_OEM_6;
	m_keyMaps[VT::Key::BACK_SLASH] = VK_OEM_5;
	m_keyMaps[VT::Key::QUOTES] = VK_OEM_7;
	m_keyMaps[VT::Key::QUESTION] = VK_OEM_2;
	m_keyMaps[VT::Key::TILDE] = VK_OEM_3;
	m_keyMaps[VT::Key::NUM_LOCK] = VK_NUMLOCK;
	m_keyMaps[VT::Key::CAPS_LOCK] = VK_CAPITAL;
	m_keyMaps[VT::Key::SCROLL_LOCK] = VK_SCROLL;
	m_keyMaps[VT::Key::SHIFT_RIGHT] = VK_RSHIFT;
	m_keyMaps[VT::Key::SHIFT_LEFT] = VK_LSHIFT;
	m_keyMaps[VT::Key::CONTROL_RIGHT] = VK_RCONTROL;
	m_keyMaps[VT::Key::CONTROL_LEFT] = VK_LCONTROL;
	m_keyMaps[VT::Key::ALT_RIGHT] = VK_RMENU;
	m_keyMaps[VT::Key::ALT_LEFT] = VK_LMENU;
	m_keyMaps[VT::Key::SYSTEM_RIGHT] = VK_RWIN;
	m_keyMaps[VT::Key::SYSTEM_LEFT] = VK_LWIN;
	m_keyMaps[VT::Key::HELP] = VK_HELP;
	m_keyMaps[VT::Key::PRINT] = VK_PRINT;
	m_keyMaps[VT::Key::SNAPSHOT] = VK_SNAPSHOT;

	return true;
}

void VT_WIN32::Win32InputSystem::release()
{
	m_keyMaps = KeyMapContainer();
	VT_SAFE_DESTROY_ARRAY(m_keys);
}

void VT_WIN32::Win32InputSystem::update()
{
	GetKeyboardState(m_keys);

	POINT mousePos;
	GetCursorPos(&mousePos);

	m_prevMousePos = m_mousePos;
	m_mousePos = VT::Vector2UInt16(static_cast<uint16_t>(mousePos.x), static_cast<uint16_t>(mousePos.y));
}

bool VT_WIN32::Win32InputSystem::isKeyDown(VT::Key key) const
{
	uint8_t index = m_keyMaps.at(key);
	return (m_keys[index] & 0x80);
}

bool VT_WIN32::Win32InputSystem::isKeyUp(VT::Key key) const
{
	uint8_t index = m_keyMaps.at(key);
	return !(m_keys[index] & 0x80);
}

VT::Vector2Int16 VT_WIN32::Win32InputSystem::getMouseMovementOffset() const
{
	return VT::Vector2Int16{ static_cast<int16_t>(m_mousePos.m_x - m_prevMousePos.m_x), static_cast<int16_t>(m_mousePos.m_y - m_prevMousePos.m_y) };
}

bool VT_WIN32::Win32InputSystem::isMouseMoved() const
{
	return ((m_mousePos.m_x != m_prevMousePos.m_x) || (m_mousePos.m_y != m_prevMousePos.m_y));
}
