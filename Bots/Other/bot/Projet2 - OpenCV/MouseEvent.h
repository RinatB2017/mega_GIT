#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

enum class InputType {LEFT_BUTTON, RIGHT_BUTTON, MIDDLE_BUTTON};

class MouseEvent
{
private:
public:
	InputType type;
	int x;
	int y;

	MouseEvent();
	MouseEvent(InputType type, int x, int y);
	~MouseEvent();
};

#endif