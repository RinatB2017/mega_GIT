//============================================================================
// Name        : usb_testing.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
using namespace std;

//#define VID 0x0483
//#define PID 0x5710

#define VID 0x08bb
#define PID 0x2704

#define DEV_INTF 0    // номер интерфейса

#define USB_DEBUG_LEVEL 3

#include <libusb-1.0/libusb.h>

#define DATA_SIZE 4

#define EP_IN 0x81
#define TIMEOUT 1000

void interrupt_transfer_loop(libusb_device_handle *handle);
void bulk_transfer_loop(libusb_device_handle *handle);

int main()
{
    cout << std::hex;

    libusb_init(NULL);   // инициализация

    libusb_set_debug(NULL, USB_DEBUG_LEVEL);  // уровень вывода отладочных сообщений

    libusb_device_handle *handle = libusb_open_device_with_vid_pid(NULL, VID, PID);
    if (handle == NULL)
    {
        cout << "Устройство не подключено\n";
        return 1;
    }
    else
    {
        cout << "Устройство найдено\n";
    }

    if (libusb_kernel_driver_active(handle,DEV_INTF))
    {
        libusb_detach_kernel_driver(handle, DEV_INTF);
    }

    if (libusb_claim_interface(handle,  DEV_INTF) < 0)
    {
        cout << "Ошибка захвата интерфейса\n";
        return 2;
    }

    interrupt_transfer_loop(handle);
    bulk_transfer_loop(handle);

    libusb_attach_kernel_driver(handle, DEV_INTF);
    libusb_close(handle);
    libusb_exit(NULL);

    return 0;
}

void interrupt_transfer_loop(libusb_device_handle *handle)
{
    cout << "Цикл считывания (interrupt).\n";
    unsigned char buf[DATA_SIZE];
    int ret;
    int i=0xFF;

    int cc=0;

    struct timeval start, end;
    long mtime, seconds, useconds;
    gettimeofday(&start, NULL);

    while (i--)
    {
        int returned = libusb_interrupt_transfer(handle, EP_IN, buf, DATA_SIZE, &ret, TIMEOUT);

        if (returned >= 0)
        {
            for (short n=0; n < DATA_SIZE; n++)
            {
                cout << "buf["<< n << "] = " << (int)buf[n] << endl;
            }
        }
        else
        {
            cout << libusb_error_name(returned);
        }
        cc++;
    }

    cout << "Считано: " << cc << endl;

    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    cout << "Прошло: " << mtime << " мс\n";
}

void bulk_transfer_loop(libusb_device_handle *handle)
{
    /// TODO:
    /// write bulk transfer code for ucontroller & this section
    cout << "Цикл считывания (bulk).\n";

    unsigned char buf[DATA_SIZE];
    int act_len = 0;

    int cc=0;
    int i=0xff;

    struct timeval start, end;
    long mtime, seconds, useconds;
    gettimeofday(&start, NULL);

    while(i--)
    {
        int returned = libusb_bulk_transfer(handle, EP_IN, buf, DATA_SIZE,&act_len, TIMEOUT);

        // parce transfer errors
        if (returned >= 0)
        {
            for (short n=0; n < DATA_SIZE; n++)
            {
                cout << "buf["<< n << "] = " << (int)buf[n] << endl;
            }
        }
        else
        {
            cout << libusb_error_name(returned);
        }
        cc++;
    }

    cout << "Считано: " << cc << endl;

    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

    cout << "Прошло: " << mtime << " мс\n";
}
