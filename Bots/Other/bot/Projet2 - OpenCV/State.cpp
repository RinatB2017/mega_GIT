#include "State.h"

State::State(const MouseEvent& mouse_event)
{
	this->mouse_event = mouse_event;

	try
	{
		object_detector.find_contours("images\\screenshot.bmp", cv::Point(mouse_event.x, mouse_event.y));
		object_detector.template_match("images\\screenshot.bmp");
	}

	catch (const std::exception& exception)
	{
		throw;
	}

	// Seed the random number generator.
	std::random_device random_device;
	random_engine.seed(random_device());
}

State::~State() { }

void State::execute()
{
	// Check if there are some matches left.
	if (object_detector.get_matches().empty())
	{
		try
		{
			object_detector.template_match("images\\screenshot.bmp");
		}

		catch (const std::exception& exception)
		{
			throw;
		}
	}

	// Choose a random cursor position inside the match.
	std::uniform_int_distribution<int> x(object_detector.get_matches().front().x, object_detector.get_matches().front().x + object_detector.get_matches().front().width);
	std::uniform_int_distribution<int> y(object_detector.get_matches().front().y, object_detector.get_matches().front().y + object_detector.get_matches().front().height);

	virtual_mouse.set_cursor_position(x(random_engine), y(random_engine));

	// Remove the last match.
	object_detector.remove_match();

	switch (mouse_event.type)
	{
	case InputType::LEFT_BUTTON:
		virtual_mouse.generate_left_click();
		break;
	case InputType::RIGHT_BUTTON:
		virtual_mouse.generate_right_click();
		break;
	case InputType::MIDDLE_BUTTON:
		// Not implemented.
		break;
	default:
		// We don't care about other input types.
		break;
	}

	Sleep(4000);
}