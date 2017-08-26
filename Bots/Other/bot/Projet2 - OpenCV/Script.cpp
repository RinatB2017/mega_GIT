#include "Script.h"

Script::Script() { }

Script::~Script() { }

void Script::create()
{
	MouseHook::get_instance().get_messages();

	// Check if some mouse events were detected.
	if (!MouseHook::get_instance().mouse_events.empty())
	{
		for (auto iterator = MouseHook::get_instance().mouse_events.begin(); iterator != MouseHook::get_instance().mouse_events.end(); ++iterator)
		{
			try
			{
				// Create a new state for each event.
				states.push_back(State(*iterator));
			}

			catch (const std::exception& exception)
			{
				throw;
			}
		}
	}

	else
	{
		throw std::runtime_error("No events detected.");
	}
}

void Script::load()
{

}

void Script::save() const
{

}

void Script::execute()
{
	// Check if the script is valid.
	if (!states.empty())
	{
		for (auto iterator = states.begin(); iterator != states.end(); ++iterator)
		{
			try
			{
				(*iterator).execute();
			}

			catch (const std::exception& exception)
			{
				throw;
			}
		}
	}

	else
	{
		throw std::runtime_error("Invalid script.");
	}
}