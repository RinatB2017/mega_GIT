#ifndef VIRTUAL_MOUSE_H
#define VIRTUAL_MOUSE_H

#include <stdexcept>
#include <vector>
#include <ctime>
#include <string>
#include <fstream>
#include "windows.h"

class VirtualMouse
{
private:
	INPUT mouse_event;
	int screen_width;
	int screen_height;

	std::vector<POINT> record_mouse_movement(int seconds) const;
public:
	VirtualMouse();
	~VirtualMouse();

	void generate_left_click();
	void generate_right_click();
	void generate_middle_click(bool send_key_down_event, bool send_key_up_event);
	POINT get_cursor_position() const;
	void set_cursor_position(int x, int y);
	void create_mouse_movement_record(int seconds, const std::string& file_path) const;
};

#endif