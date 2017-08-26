#ifndef VIRTUAL_KEYBOARD_H
#define VIRTUAL_KEYBOARD_H

#include <stdexcept>
#include <Windows.h>

class VirtualKeyboard
{
private:
	INPUT keyboard_event;
public:
	VirtualKeyboard();
	~VirtualKeyboard();

	void generate_key(int key_code, bool send_key_down_event, bool send_key_up_event);
};

#endif