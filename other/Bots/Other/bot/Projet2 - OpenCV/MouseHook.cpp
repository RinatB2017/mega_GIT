#include "MouseHook.h"

MouseHook::MouseHook() { }

MouseHook::~MouseHook() { }

// Return an instance of this singleton class.
MouseHook& MouseHook::get_instance()
{
	static MouseHook instance;

	// Check if the hook was already installed.
	if (!instance.mouse_hook)
	{
		instance.install_hook();
	}

	return instance;
}

// This function is necessary for the windows API callback function. Do not use outside of this class.
HHOOK& MouseHook::get_mouse_hook()
{
	return mouse_hook;
}

// Peek through the message list in order to trigger our hook.
void MouseHook::get_messages()
{
	MSG message;

	while (std::clock() - timer < 5000)
	{
		// Check to see if any messages are waiting in the queue.
		if (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			// Translate the message and dispatch it to WindowProc.
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	uninstall_hook(); 
}

// Install a global, low level hook to monitor the system for mouse events.
void MouseHook::install_hook()
{
	timer = std::clock();
	mouse_hook = SetWindowsHookEx(WH_MOUSE_LL, hook_callback, 0, 0);
}

// Uninstall the hook.
void MouseHook::uninstall_hook()
{
	UnhookWindowsHookEx(mouse_hook);
}

// This function is called every time a mouse event is created.
LRESULT WINAPI hook_callback(int code, WPARAM message_id, LPARAM message)
{
	// Message is a pointer to a mouse event structure.
	MSLLHOOKSTRUCT* mouse_event = (MSLLHOOKSTRUCT*)message;
	
	// If code is less than zero, the hook procedure must pass the message to the CallNextHookEx function without
	// further processing and should return the value returned by CallNextHookEx.
	if (code == HC_ACTION)
	{
		switch (message_id)
		{
		case WM_LBUTTONDOWN:
			MouseHook::get_instance().mouse_events.push_back(MouseEvent(InputType::LEFT_BUTTON, mouse_event->pt.x, mouse_event->pt.y));
			break;
		case WM_RBUTTONDOWN:
			MouseHook::get_instance().mouse_events.push_back(MouseEvent(InputType::RIGHT_BUTTON, mouse_event->pt.x, mouse_event->pt.y));
			break;
		case WM_MBUTTONDOWN:
			MouseHook::get_instance().mouse_events.push_back(MouseEvent(InputType::MIDDLE_BUTTON, mouse_event->pt.x, mouse_event->pt.y));
			break;
		default:
			// We don't care about other messages.
			break;
		}
	}
	
	// Calling CallNextHookEx is optional, but it is highly recommended; otherwise, other applications that
	// have installed hooks will not receive hook notifications and may behave incorrectly as a result.
	return CallNextHookEx(MouseHook::get_instance().get_mouse_hook(), code, message_id, message);
}