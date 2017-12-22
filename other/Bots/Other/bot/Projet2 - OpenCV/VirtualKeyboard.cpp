#include "VirtualKeyboard.h"

VirtualKeyboard::VirtualKeyboard()
{
	// Specify that it's a keyboard event.
	keyboard_event.type = INPUT_KEYBOARD;

	// Specify that it's not a virtual-key code.
	keyboard_event.ki.wVk = 0;

	// Specify zero as time stamp for the event, the system will provide its own time stamp.
	keyboard_event.ki.time = 0;
	keyboard_event.ki.dwExtraInfo = 0;
}

VirtualKeyboard::~VirtualKeyboard() { }

// Generate a key based on hardware using a virtual key code.
void VirtualKeyboard::generate_key(int key_code, bool send_key_down_event = true, bool send_key_up_event = true)
{
	// Check if the key code is valid.
	if (key_code > 0x01 && key_code < 0xFE)
	{
		// Specify the hardware scan code.
		keyboard_event.ki.wScan = MapVirtualKey(key_code, MAPVK_VK_TO_VSC);

		if (send_key_down_event)
		{
			// Specify that it's a KeyDown event.
			keyboard_event.ki.dwFlags = KEYEVENTF_SCANCODE;
		
			// Generate the KeyDown event.
			SendInput(1, &keyboard_event, sizeof(keyboard_event));
		}

		if (send_key_up_event)
		{
			// Specify that it's a KeyUp event.
			keyboard_event.ki.dwFlags = KEYEVENTF_SCANCODE | KEYEVENTF_KEYUP;

			// Generate the KeyUp event.
			SendInput(1, &keyboard_event, sizeof(keyboard_event));
		}
	}

	else
	{
		throw std::runtime_error("Invalid virtual key code.");
	}
}