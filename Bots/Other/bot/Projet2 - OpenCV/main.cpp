#include <iostream>
#include "ScreenshotManager.h"
#include "Script.h"

// TODO: MouseEvent, create abstract class Event to support KeyboardEvent?
// TODO: replace all if (example.data) into if (!example.empty())
// TODO: replace ctime with chrono.
// TODO: create a cursor path algorithm (so the cursor doesn't get teleported into the correct position).
// TODO: create a more accurate detection algorithm.
// TODO: replace saving bitmap possible? transform directly into cv::Mat?
// TODO: images printing out of sync with script.
int main()
{
	const std::string GAME = "MozillaWindowClass";

	ScreenshotManager screenshot_manager(GAME);
	Script script;

	// Give the window focus for the keyboard and mouse.
	HWND game_window = screenshot_manager.get_handle_window(GAME);
	SetForegroundWindow(game_window);

	// Check if we are creating a new script.
	if (true)
	{
		try
		{
			script.create();
		}

		catch (const std::exception& exception)
		{
			std::cout << "Exception raised - creating script: " << exception.what() << '\n';
			std::cin.get();
		}
	}

	//else
	//{
	//	try
	//	{
	//		script.load();
	//	}

	//	catch (const std::exception& exception)
	//	{
	//		std::cout << "Exception raised - loading script: " << exception.what() << '\n';
	//		std::cin.get();
	//	}
	//}

	while (true)
	{
		try
		{
			script.execute();
		}

		catch (const std::exception& exception)
		{
			std::cout << "Exception raised - executing script: " << exception.what() << '\n';
			std::cin.get();
		}
	}

	cv::waitKey(0);

	return 0;
}