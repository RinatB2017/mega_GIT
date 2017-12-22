#ifndef STATE_H
#define STATE_H

#include <random>
#include "ObjectDetector.h"
#include "MouseEvent.h"
#include "VirtualKeyboard.h"
#include "VirtualMouse.h"

class State
{
private:
	ObjectDetector object_detector;
	MouseEvent mouse_event;
	VirtualKeyboard virtual_keyboard;
	VirtualMouse virtual_mouse;
	std::mt19937 random_engine;
public:
	State(const MouseEvent& mouse_event);
	~State();

	void execute();
};

#endif