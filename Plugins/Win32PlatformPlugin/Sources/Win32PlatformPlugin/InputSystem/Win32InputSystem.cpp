#include "Win32InputSystem.h"

#include "Core/UtilitiesMacros.h"
#include "Win32PlatformPlugin/Win32Platform.h"

constexpr size_t KeyMapSize = 256;

void VT_WIN32::Win32InputSystem::fillVTKeyMaps()
{
	m_vtKeyMap[VT::Key::MOUSE_RIGHT] = VK_RBUTTON;
	m_vtKeyMap[VT::Key::MOUSE_LEFT] = VK_LBUTTON;
	m_vtKeyMap[VT::Key::MOUSE_MIDDLE] = VK_MBUTTON;
	m_vtKeyMap[VT::Key::BACK_SPACE] = VK_BACK;
	m_vtKeyMap[VT::Key::DEL] = VK_RBUTTON;
	m_vtKeyMap[VT::Key::TAB] = VK_TAB;
	m_vtKeyMap[VT::Key::CLEAR] = VK_CLEAR;
	m_vtKeyMap[VT::Key::RETURN] = VK_RETURN;
	m_vtKeyMap[VT::Key::PAUSE] = VK_PAUSE;
	m_vtKeyMap[VT::Key::ESC] = VK_ESCAPE;
	m_vtKeyMap[VT::Key::SPACE] = VK_SPACE;
	m_vtKeyMap[VT::Key::KEY_PAD_0] = VK_NUMPAD0;
	m_vtKeyMap[VT::Key::KEY_PAD_1] = VK_NUMPAD1;
	m_vtKeyMap[VT::Key::KEY_PAD_2] = VK_NUMPAD2;
	m_vtKeyMap[VT::Key::KEY_PAD_3] = VK_NUMPAD3;
	m_vtKeyMap[VT::Key::KEY_PAD_4] = VK_NUMPAD4;
	m_vtKeyMap[VT::Key::KEY_PAD_5] = VK_NUMPAD5;
	m_vtKeyMap[VT::Key::KEY_PAD_6] = VK_NUMPAD6;
	m_vtKeyMap[VT::Key::KEY_PAD_7] = VK_NUMPAD7;
	m_vtKeyMap[VT::Key::KEY_PAD_8] = VK_NUMPAD8;
	m_vtKeyMap[VT::Key::KEY_PAD_9] = VK_NUMPAD9;
	m_vtKeyMap[VT::Key::DOT] = VK_OEM_PERIOD;
	m_vtKeyMap[VT::Key::COMMA] = VK_OEM_COMMA;
	m_vtKeyMap[VT::Key::DIVIDE] = VK_DIVIDE;
	m_vtKeyMap[VT::Key::MULTIPLE] = VK_MULTIPLY;
	m_vtKeyMap[VT::Key::MINUS] = VK_SUBTRACT;
	m_vtKeyMap[VT::Key::PLUS] = VK_ADD;
	m_vtKeyMap[VT::Key::ENTER] = VK_RETURN;
	m_vtKeyMap[VT::Key::ARROW_UP] = VK_UP;
	m_vtKeyMap[VT::Key::ARROW_DOWN] = VK_DOWN;
	m_vtKeyMap[VT::Key::ARROW_RIGHT] = VK_RIGHT;
	m_vtKeyMap[VT::Key::ARROW_LEFT] = VK_LEFT;
	m_vtKeyMap[VT::Key::INSERT] = VK_INSERT;
	m_vtKeyMap[VT::Key::HOME] = VK_HOME;
	m_vtKeyMap[VT::Key::END] = VK_END;
	m_vtKeyMap[VT::Key::PAGE_UP] = VK_PRIOR;
	m_vtKeyMap[VT::Key::PAGE_DOWN] = VK_NEXT;
	m_vtKeyMap[VT::Key::F1] = VK_F1;
	m_vtKeyMap[VT::Key::F2] = VK_F2;
	m_vtKeyMap[VT::Key::F3] = VK_F3;
	m_vtKeyMap[VT::Key::F4] = VK_F4;
	m_vtKeyMap[VT::Key::F5] = VK_F5;
	m_vtKeyMap[VT::Key::F6] = VK_F6;
	m_vtKeyMap[VT::Key::F7] = VK_F7;
	m_vtKeyMap[VT::Key::F8] = VK_F8;
	m_vtKeyMap[VT::Key::F9] = VK_F9;
	m_vtKeyMap[VT::Key::F10] = VK_F10;
	m_vtKeyMap[VT::Key::F11] = VK_F11;
	m_vtKeyMap[VT::Key::F12] = VK_F12;
	m_vtKeyMap[VT::Key::F13] = VK_F13;
	m_vtKeyMap[VT::Key::F14] = VK_F14;
	m_vtKeyMap[VT::Key::F15] = VK_F15;
	m_vtKeyMap[VT::Key::ALPHA_0] = 0x30;
	m_vtKeyMap[VT::Key::ALPHA_1] = 0x31;
	m_vtKeyMap[VT::Key::ALPHA_2] = 0x32;
	m_vtKeyMap[VT::Key::ALPHA_3] = 0x33;
	m_vtKeyMap[VT::Key::ALPHA_4] = 0x34;
	m_vtKeyMap[VT::Key::ALPHA_5] = 0x35;
	m_vtKeyMap[VT::Key::ALPHA_6] = 0x36;
	m_vtKeyMap[VT::Key::ALPHA_7] = 0x37;
	m_vtKeyMap[VT::Key::ALPHA_8] = 0x38;
	m_vtKeyMap[VT::Key::ALPHA_9] = 0x39;
	m_vtKeyMap[VT::Key::A] = 0x41;
	m_vtKeyMap[VT::Key::B] = 0x42;
	m_vtKeyMap[VT::Key::C] = 0x43;
	m_vtKeyMap[VT::Key::D] = 0x44;
	m_vtKeyMap[VT::Key::E] = 0x45;
	m_vtKeyMap[VT::Key::F] = 0x46;
	m_vtKeyMap[VT::Key::G] = 0x47;
	m_vtKeyMap[VT::Key::H] = 0x48;
	m_vtKeyMap[VT::Key::I] = 0x49;
	m_vtKeyMap[VT::Key::J] = 0x4A;
	m_vtKeyMap[VT::Key::K] = 0x4B;
	m_vtKeyMap[VT::Key::L] = 0x4C;
	m_vtKeyMap[VT::Key::M] = 0x4D;
	m_vtKeyMap[VT::Key::N] = 0x4E;
	m_vtKeyMap[VT::Key::O] = 0x4F;
	m_vtKeyMap[VT::Key::P] = 0x50;
	m_vtKeyMap[VT::Key::Q] = 0x51;
	m_vtKeyMap[VT::Key::R] = 0x52;
	m_vtKeyMap[VT::Key::S] = 0x53;
	m_vtKeyMap[VT::Key::T] = 0x54;
	m_vtKeyMap[VT::Key::U] = 0x55;
	m_vtKeyMap[VT::Key::V] = 0x56;
	m_vtKeyMap[VT::Key::W] = 0x57;
	m_vtKeyMap[VT::Key::X] = 0x58;
	m_vtKeyMap[VT::Key::Y] = 0x59;
	m_vtKeyMap[VT::Key::Z] = 0x5A;
	m_vtKeyMap[VT::Key::COLONS] = VK_OEM_1;
	m_vtKeyMap[VT::Key::BRACKETS_RIGHT] = VK_OEM_4;
	m_vtKeyMap[VT::Key::BRACKETS_LEFT] = VK_OEM_6;
	m_vtKeyMap[VT::Key::BACK_SLASH] = VK_OEM_5;
	m_vtKeyMap[VT::Key::QUOTES] = VK_OEM_7;
	m_vtKeyMap[VT::Key::QUESTION] = VK_OEM_2;
	m_vtKeyMap[VT::Key::TILDE] = VK_OEM_3;
	m_vtKeyMap[VT::Key::NUM_LOCK] = VK_NUMLOCK;
	m_vtKeyMap[VT::Key::CAPS_LOCK] = VK_CAPITAL;
	m_vtKeyMap[VT::Key::SCROLL_LOCK] = VK_SCROLL;
	m_vtKeyMap[VT::Key::SHIFT_RIGHT] = VK_RSHIFT;
	m_vtKeyMap[VT::Key::SHIFT_LEFT] = VK_LSHIFT;
	m_vtKeyMap[VT::Key::CONTROL_RIGHT] = VK_RCONTROL;
	m_vtKeyMap[VT::Key::CONTROL_LEFT] = VK_LCONTROL;
	m_vtKeyMap[VT::Key::ALT_RIGHT] = VK_RMENU;
	m_vtKeyMap[VT::Key::ALT_LEFT] = VK_LMENU;
	m_vtKeyMap[VT::Key::SYSTEM_RIGHT] = VK_RWIN;
	m_vtKeyMap[VT::Key::SYSTEM_LEFT] = VK_LWIN;
	m_vtKeyMap[VT::Key::HELP] = VK_HELP;
	m_vtKeyMap[VT::Key::PRINT] = VK_PRINT;
	m_vtKeyMap[VT::Key::SNAPSHOT] = VK_SNAPSHOT;
}

void VT_WIN32::Win32InputSystem::fillWin32KeyMaps()
{
	m_win32KeyMap[VK_RBUTTON] = VT::Key::MOUSE_RIGHT;
	m_win32KeyMap[VK_LBUTTON] = VT::Key::MOUSE_LEFT;
	m_win32KeyMap[VK_MBUTTON] = VT::Key::MOUSE_MIDDLE;
	m_win32KeyMap[VK_BACK] = VT::Key::BACK_SPACE;
	m_win32KeyMap[VK_RBUTTON] = VT::Key::DEL;
	m_win32KeyMap[VK_TAB] = VT::Key::TAB;
	m_win32KeyMap[VK_CLEAR] = VT::Key::CLEAR;
	m_win32KeyMap[VK_RETURN] = VT::Key::RETURN;
	m_win32KeyMap[VK_PAUSE] = VT::Key::PAUSE;
	m_win32KeyMap[VK_ESCAPE] = VT::Key::ESC;
	m_win32KeyMap[VK_SPACE] = VT::Key::SPACE;
	m_win32KeyMap[VK_NUMPAD0] = VT::Key::KEY_PAD_0;
	m_win32KeyMap[VK_NUMPAD1] = VT::Key::KEY_PAD_1;
	m_win32KeyMap[VK_NUMPAD2] = VT::Key::KEY_PAD_2;
	m_win32KeyMap[VK_NUMPAD3] = VT::Key::KEY_PAD_3;
	m_win32KeyMap[VK_NUMPAD4] = VT::Key::KEY_PAD_4;
	m_win32KeyMap[VK_NUMPAD5] = VT::Key::KEY_PAD_5;
	m_win32KeyMap[VK_NUMPAD6] = VT::Key::KEY_PAD_6;
	m_win32KeyMap[VK_NUMPAD7] = VT::Key::KEY_PAD_7;
	m_win32KeyMap[VK_NUMPAD8] = VT::Key::KEY_PAD_8;
	m_win32KeyMap[VK_NUMPAD9] = VT::Key::KEY_PAD_9;
	m_win32KeyMap[VK_OEM_PERIOD] = VT::Key::DOT;
	m_win32KeyMap[VK_OEM_COMMA] = VT::Key::COMMA;
	m_win32KeyMap[VK_DIVIDE] = VT::Key::DIVIDE;
	m_win32KeyMap[VK_MULTIPLY] = VT::Key::MULTIPLE;
	m_win32KeyMap[VK_SUBTRACT] = VT::Key::MINUS;
	m_win32KeyMap[VK_ADD] = VT::Key::PLUS;
	m_win32KeyMap[VK_RETURN] = VT::Key::ENTER;
	m_win32KeyMap[VK_UP] = VT::Key::ARROW_UP;
	m_win32KeyMap[VK_DOWN] = VT::Key::ARROW_DOWN;
	m_win32KeyMap[VK_RIGHT] = VT::Key::ARROW_RIGHT;
	m_win32KeyMap[VK_LEFT] = VT::Key::ARROW_LEFT;
	m_win32KeyMap[VK_INSERT] = VT::Key::INSERT;
	m_win32KeyMap[VK_HOME] = VT::Key::HOME;
	m_win32KeyMap[VK_END] = VT::Key::END;
	m_win32KeyMap[VK_PRIOR] = VT::Key::PAGE_UP;
	m_win32KeyMap[VK_NEXT] = VT::Key::PAGE_DOWN;
	m_win32KeyMap[VK_F1] = VT::Key::F1;
	m_win32KeyMap[VK_F2] = VT::Key::F2;
	m_win32KeyMap[VK_F3] = VT::Key::F3;
	m_win32KeyMap[VK_F4] = VT::Key::F4;
	m_win32KeyMap[VK_F5] = VT::Key::F5;
	m_win32KeyMap[VK_F6] = VT::Key::F6;
	m_win32KeyMap[VK_F7] = VT::Key::F7;
	m_win32KeyMap[VK_F8] = VT::Key::F8;
	m_win32KeyMap[VK_F9] = VT::Key::F9;
	m_win32KeyMap[VK_F10] = VT::Key::F10;
	m_win32KeyMap[VK_F11] = VT::Key::F11;
	m_win32KeyMap[VK_F12] = VT::Key::F12;
	m_win32KeyMap[VK_F13] = VT::Key::F13;
	m_win32KeyMap[VK_F14] = VT::Key::F14;
	m_win32KeyMap[VK_F15] = VT::Key::F15;
	m_win32KeyMap[0x30] = VT::Key::ALPHA_0;
	m_win32KeyMap[0x31] = VT::Key::ALPHA_1;
	m_win32KeyMap[0x32] = VT::Key::ALPHA_2;
	m_win32KeyMap[0x33] = VT::Key::ALPHA_3;
	m_win32KeyMap[0x34] = VT::Key::ALPHA_4;
	m_win32KeyMap[0x35] = VT::Key::ALPHA_5;
	m_win32KeyMap[0x36] = VT::Key::ALPHA_6;
	m_win32KeyMap[0x37] = VT::Key::ALPHA_7;
	m_win32KeyMap[0x38] = VT::Key::ALPHA_8;
	m_win32KeyMap[0x39] = VT::Key::ALPHA_9;
	m_win32KeyMap[0x41] = VT::Key::A;
	m_win32KeyMap[0x42] = VT::Key::B;
	m_win32KeyMap[0x43] = VT::Key::C;
	m_win32KeyMap[0x44] = VT::Key::D;
	m_win32KeyMap[0x45] = VT::Key::E;
	m_win32KeyMap[0x46] = VT::Key::F;
	m_win32KeyMap[0x47] = VT::Key::G;
	m_win32KeyMap[0x48] = VT::Key::H;
	m_win32KeyMap[0x49] = VT::Key::I;
	m_win32KeyMap[0x4A] = VT::Key::J;
	m_win32KeyMap[0x4B] = VT::Key::K;
	m_win32KeyMap[0x4C] = VT::Key::L;
	m_win32KeyMap[0x4D] = VT::Key::M;
	m_win32KeyMap[0x4E] = VT::Key::N;
	m_win32KeyMap[0x4F] = VT::Key::O;
	m_win32KeyMap[0x50] = VT::Key::P;
	m_win32KeyMap[0x51] = VT::Key::Q;
	m_win32KeyMap[0x52] = VT::Key::R;
	m_win32KeyMap[0x53] = VT::Key::S;
	m_win32KeyMap[0x54] = VT::Key::T;
	m_win32KeyMap[0x55] = VT::Key::U;
	m_win32KeyMap[0x56] = VT::Key::V;
	m_win32KeyMap[0x57] = VT::Key::W;
	m_win32KeyMap[0x58] = VT::Key::X;
	m_win32KeyMap[0x59] = VT::Key::Y;
	m_win32KeyMap[0x5A] = VT::Key::Z;
	m_win32KeyMap[VK_OEM_1] = VT::Key::COLONS;
	m_win32KeyMap[VK_OEM_4] = VT::Key::BRACKETS_RIGHT;
	m_win32KeyMap[VK_OEM_6] = VT::Key::BRACKETS_LEFT;
	m_win32KeyMap[VK_OEM_5] = VT::Key::BACK_SLASH;
	m_win32KeyMap[VK_OEM_7] = VT::Key::QUOTES;
	m_win32KeyMap[VK_OEM_2] = VT::Key::QUESTION;
	m_win32KeyMap[VK_OEM_3] = VT::Key::TILDE;
	m_win32KeyMap[VK_NUMLOCK] = VT::Key::NUM_LOCK;
	m_win32KeyMap[VK_CAPITAL] = VT::Key::CAPS_LOCK;
	m_win32KeyMap[VK_SCROLL] = VT::Key::SCROLL_LOCK;
	m_win32KeyMap[VK_RSHIFT] = VT::Key::SHIFT_RIGHT;
	m_win32KeyMap[VK_LSHIFT] = VT::Key::SHIFT_LEFT;
	m_win32KeyMap[VK_RCONTROL] = VT::Key::CONTROL_RIGHT;
	m_win32KeyMap[VK_LCONTROL] = VT::Key::CONTROL_LEFT;
	m_win32KeyMap[VK_RMENU] = VT::Key::ALT_RIGHT;
	m_win32KeyMap[VK_LMENU] = VT::Key::ALT_LEFT;
	m_win32KeyMap[VK_RWIN] = VT::Key::SYSTEM_RIGHT;
	m_win32KeyMap[VK_LWIN] = VT::Key::SYSTEM_LEFT;
	m_win32KeyMap[VK_HELP] = VT::Key::HELP;
	m_win32KeyMap[VK_PRINT] = VT::Key::PRINT;
	m_win32KeyMap[VK_SNAPSHOT] = VT::Key::SNAPSHOT;
}

bool VT_WIN32::Win32InputSystem::init()
{
	m_keys = new uint8_t[KeyMapSize];
	memset(m_keys, 0, KeyMapSize * sizeof(uint8_t));

	fillVTKeyMaps();
	fillWin32KeyMaps();

	return true;
}

void VT_WIN32::Win32InputSystem::release()
{
	m_vtKeyMap = KeyMapVTContainer();
	m_win32KeyMap = KeyMapWin32Container();

	VT_SAFE_DESTROY_ARRAY(m_keys);
}

void VT_WIN32::Win32InputSystem::update()
{
	GetKeyboardState(m_keys);

	POINT mousePos;
	GetCursorPos(&mousePos);

	m_prevMousePos = m_mousePos;
	m_mousePos = VT::Vector2UInt16(static_cast<uint16_t>(mousePos.x), static_cast<uint16_t>(mousePos.y));
	m_mouseMovementDelta = VT::Vector2Int16(static_cast<int16_t>(m_mousePos.m_x - m_prevMousePos.m_x), static_cast<int16_t>(m_mousePos.m_y - m_prevMousePos.m_y));
}

bool VT_WIN32::Win32InputSystem::isKeyDown(VT::Key key) const
{
	uint8_t index = m_vtKeyMap.at(key);
	return (m_keys[index] & 0x80);
}

bool VT_WIN32::Win32InputSystem::isKeyUp(VT::Key key) const
{
	uint8_t index = m_vtKeyMap.at(key);
	return !(m_keys[index] & 0x80);
}

bool VT_WIN32::Win32InputSystem::isMouseMoved() const
{
	return ((m_mousePos.m_x != m_prevMousePos.m_x) || (m_mousePos.m_y != m_prevMousePos.m_y));
}

VT::Key VT_WIN32::Win32InputSystem::convertKeyWin32ToVt(uint8_t win32Key)
{
	return m_win32KeyMap[win32Key];
}
