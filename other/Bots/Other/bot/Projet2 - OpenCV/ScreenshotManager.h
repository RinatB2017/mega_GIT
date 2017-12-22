#ifndef SCREENSHOT_MANAGER_H
#define SCREENSHOT_MANAGER_H

#include <string>
#include <thread>
#include <atomic>
#include <Windows.h>

class ScreenshotManager
{
private:
	HWND window_game;
	HDC window_dc;
	HDC memory_dc;
	int screen_width;
	int screen_height;
	HBITMAP bitmap;
	BITMAPINFO bitmap_info;
	BITMAPFILEHEADER bitmap_file_header;
	std::string file_path;
	unsigned char* data;
	std::thread thread;
	std::atomic<bool> running;

	void create_bitmap_file();
public:
	ScreenshotManager(const std::string& window_class);
	~ScreenshotManager();

	HWND get_handle_window(const std::string& window_class) const;
	bool get_running() const;
	void set_running(bool running);
};

#endif