#include "MouseEvent.h"

MouseEvent::MouseEvent() { }

MouseEvent::MouseEvent(InputType type, int x, int y)
{
	this->type = type;
	this->x = x;
	this->y = y;
}

MouseEvent::~MouseEvent() { }