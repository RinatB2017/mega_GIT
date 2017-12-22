#ifndef MOUSE_HOOK_H
#define MOUSE_HOOK_H

#include <vector>
#include <ctime>
#include <Windows.h>
#include "MouseEvent.h"

class MouseHook
{
private:
	HHOOK mouse_hook;
	std::clock_t timer;

	MouseHook();
	~MouseHook();

	void install_hook();
	void uninstall_hook();

	// Don't implement these functions.
	MouseHook(MouseHook const& mouse_hook);
    void operator=(MouseHook const& mouse_hook);
public:
	// It is public in order to satisfy the requirements of the windows API callback function.
	std::vector<MouseEvent> mouse_events;

	static MouseHook& get_instance();
	HHOOK& get_mouse_hook();
	void get_messages();
};

LRESULT WINAPI hook_callback(int code, WPARAM message_id, LPARAM message);

#endif