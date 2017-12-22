#include "ScreenshotManager.h"

ScreenshotManager::ScreenshotManager(const std::string& window_class)
{
	window_game = get_handle_window(window_class);

	if (window_game)
	{
		SetForegroundWindow(window_game);

		// Get the physical device context of the specified window.
		window_dc = GetDC(window_game);

		// Create a new memory device context for the bitmap.
		// In order to use the memory device context, a bitmap of the correct width and height must be selected.
		memory_dc = CreateCompatibleDC(window_dc);

		if (memory_dc)
		{
			screen_width = GetSystemMetrics(SM_CXSCREEN);
			screen_height = GetSystemMetrics(SM_CYSCREEN);

			// Create a new bitmap. It uses the current device context in order to create a color bitmap.
			bitmap = CreateCompatibleBitmap(window_dc, screen_width, screen_height);

			if (bitmap)
			{
				bitmap_info = BITMAPINFO();
				bitmap_file_header = BITMAPFILEHEADER();
				file_path = "images\\screenshot.bmp";

				// Assign the device context to the bitmap. The bitmap will now retain its colors.
				SelectObject(memory_dc, bitmap);

				bitmap_info.bmiHeader.biSize = sizeof(bitmap_info.bmiHeader);
				bitmap_info.bmiHeader.biWidth = screen_width;
				bitmap_info.bmiHeader.biHeight = screen_height;
				bitmap_info.bmiHeader.biPlanes = 1;
				bitmap_info.bmiHeader.biBitCount = 24;
				bitmap_info.bmiHeader.biCompression = BI_RGB;
				bitmap_info.bmiHeader.biSizeImage = screen_width * screen_height * 3;
				bitmap_info.bmiHeader.biXPelsPerMeter = 0;
				bitmap_info.bmiHeader.biYPelsPerMeter = 0;
				bitmap_info.bmiHeader.biClrUsed = 0;
				bitmap_info.bmiHeader.biClrImportant = 0;

				// Allocate the buffer for the data.
				data = new unsigned char[bitmap_info.bmiHeader.biSizeImage];

				// Offset to where the actual bitmap bits start.
				bitmap_file_header.bfOffBits = sizeof(bitmap_file_header) + sizeof(bitmap_info.bmiHeader); 
    
				// Specify the size of the file.
				bitmap_file_header.bfSize = bitmap_info.bmiHeader.biSizeImage + sizeof(bitmap_file_header) + sizeof(bitmap_info.bmiHeader);
    
				// Specify that the file is a bitmap.
				bitmap_file_header.bfType = 0x4D42;

				running = true;
				thread = std::thread(&ScreenshotManager::create_bitmap_file, this);
			}

			else
			{
				throw std::runtime_error("Invalid bitmap.");
			}
		}

		else
		{
			throw std::runtime_error("Invalid memory device context.");
		}
	}

	else
	{
		throw std::runtime_error("Invalid window handle.");
	}
}

ScreenshotManager::~ScreenshotManager()
{
	DeleteObject(bitmap);
	DeleteDC(memory_dc);
	ReleaseDC(window_game, window_dc);

	delete [] data;

	try
	{
		thread.join();
	}

	catch (const std::exception& exception)
	{
		throw std::runtime_error("Unjoinable thread.");
	}
}

HWND ScreenshotManager::get_handle_window(const std::string& window_class) const
{
	// Retrieve a handle to a window whose class name match the specified string. The function also searches child windows.
	HWND handle_window = FindWindowEx(0, 0, window_class.c_str(), 0);

	// Check if the handle is valid.
	if (handle_window)
	{
		return handle_window;
	}

	else
	{
		throw std::runtime_error("Invalid window class.");
	}
}

bool ScreenshotManager::get_running() const
{
	return running;
}

void ScreenshotManager::set_running(bool running)
{
	this->running = running;
}

void ScreenshotManager::create_bitmap_file()
{
	while (running)
	{
		unsigned long number_bits_written = 0;

		// Perform a bit-block transfer of the color data from the 
		// specified source device context into a destination device context.
		if (BitBlt(memory_dc, 0, 0, screen_width, screen_height, window_dc, 0, 0, SRCCOPY))
		{
			// Get the data. If the function returns a non-zero value, it succeeded.
			// Otherwise, set the pointer to 0 because it failed.
			GetDIBits(window_dc, bitmap, 0, bitmap_info.bmiHeader.biHeight, data, &(bitmap_info), DIB_RGB_COLORS);

			// Create a new file where the screen will be put.
			HANDLE file = CreateFile(file_path.c_str(), GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

			WriteFile(file, &bitmap_file_header, sizeof(bitmap_file_header), &number_bits_written, 0);
			WriteFile(file, &bitmap_info.bmiHeader, sizeof(bitmap_info.bmiHeader), &number_bits_written, 0);
			WriteFile(file, data, bitmap_info.bmiHeader.biSizeImage, &number_bits_written, 0);

			CloseHandle(file);

			Sleep(10000);	// TODO: replace with a better method.
		}

		else
		{
			throw std::runtime_error("Bit-block transfer of the color data has failed.");
		}
	}
}