#include "VirtualMouse.h"

VirtualMouse::VirtualMouse()
{
	// Specify that it's a mouse event.
	mouse_event.type = INPUT_MOUSE;

	// Specify zero as time stamp for the event, the system will provide its own time stamp.
	mouse_event.mi.time = 0;
	mouse_event.mi.dwExtraInfo = 0;
	mouse_event.mi.mouseData = 0;

	screen_width = GetSystemMetrics(SM_CXSCREEN);
	screen_height = GetSystemMetrics(SM_CYSCREEN);
}

VirtualMouse::~VirtualMouse() { }

// Generate a mouse left click based on hardware.
void VirtualMouse::generate_left_click()
{
	mouse_event.mi.dx = 0;
	mouse_event.mi.dy = 0;

	mouse_event.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
	SendInput(1, &mouse_event, sizeof(mouse_event));

	mouse_event.mi.dwFlags = MOUSEEVENTF_LEFTUP;
	SendInput(1, &mouse_event, sizeof(mouse_event));
}

// Generate a mouse right click based on hardware.
void VirtualMouse::generate_right_click()
{
	mouse_event.mi.dx = 0;
	mouse_event.mi.dy = 0;

	mouse_event.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput(1, &mouse_event, sizeof(mouse_event));

	mouse_event.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
	SendInput(1, &mouse_event, sizeof(mouse_event));
}

// Generate a mouse middle click based on hardware.
void VirtualMouse::generate_middle_click(bool send_key_down_event = true, bool send_key_up_event = true)
{
	mouse_event.mi.dx = 0;
	mouse_event.mi.dy = 0;

	if (send_key_down_event)
	{
		mouse_event.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
		SendInput(1, &mouse_event, sizeof(mouse_event));
	}

	if (send_key_up_event)
	{
		mouse_event.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
		SendInput(1, &mouse_event, sizeof(mouse_event));
	}
}

// Get the cursor position.
POINT VirtualMouse::get_cursor_position() const
{
	POINT point;
	GetCursorPos(&point);

	return point;
}

// Set the cursor position.
void VirtualMouse::set_cursor_position(int x, int y)
{
	mouse_event.mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE;

	// Check if the coordinates are within the screen's boundaries.
	if (x >= 0 && x < screen_width && y >= 0 && y < screen_height)
	{
		// Get the screen width.
		mouse_event.mi.dx = x * (65535 / screen_width);

		// Get the screen height.
		mouse_event.mi.dy = y * (65535 / screen_height);
	
		// SendInput returns the number of mouse events that were
		// successfully inserted into the mouse input stream.
		SendInput(1, &mouse_event, sizeof(mouse_event));
	}

	else
	{
		throw std::runtime_error("Invalid mouse position.");
	}
}

void VirtualMouse::create_mouse_movement_record(int seconds, const std::string& file_path) const
{
	// Check if the file path is not empty.
	if (file_path != "")
	{
		std::vector<POINT> points = record_mouse_movement(seconds);

		// Check if the vector of positions is not empty.
		if (points.size() > 0)
		{
			std::ofstream file;

			// Create a new file.
			file.open(file_path, std::ios_base::in | std::ios_base::out | std::ios_base::trunc);

			// If the file opens.
			if (!file.fail())
			{
				// Write every cursor positions.
				for (int i = 0; i < points.size(); ++i)
				{
					file << "(" << points[i].x << ", " << points[i].y << ");" << '\n';
				}

				// Close the file.
				file.close();
			}

			else
			{
				throw std::runtime_error("Invalid mouse movement record.");
			}
		}

		else
		{
			throw std::runtime_error("Invalid vector of mouse positions.");
		}
	}

	else
	{
		throw std::runtime_error("Invalid file path.");
	}
}

// Record all mouse coordinates for the specified time.
std::vector<POINT> VirtualMouse::record_mouse_movement(int seconds) const
{
	// Check if the time is valid.
	if (seconds > 0)
	{
		std::vector<POINT> points;
		std::clock_t timer = std::clock();

		while (std::clock() - timer < seconds * 1000)
		{
			points.push_back(get_cursor_position());
		}

		return points;
	}

	else
	{
		throw std::runtime_error("Invalid number of seconds.");
	}
}