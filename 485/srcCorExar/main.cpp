/*******************************************************************************
* Correct device USB-to-COM. Only for 0x04e2 vendor (Exar corp).
*
* Tested ID: 04e2:1411 (Product: XR21B1411)
*
* Copyright (c) 2016 Kuzminov Sergey <arm-skif@yandex.ru>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation; either version 2 of the License, or (at your option)
* any later version.
*******************************************************************************/

#include <iostream>
#include </usr/include/libusb-1.0/libusb.h>

using namespace std;

//------------------------------------------------------------------------------
#define XR_SET_REG  0
#define XR_GETN_REG 1

#define CDC_ACM_FLOW_CONTROL    0x216
#define CDC_ACM_GPIO_MODE       0x217
#define CDC_ACM_GPIO_DIRECTION  0x218
#define CDC_ACM_GPIO_INT_MASK   0x219

/*
#define FLOW_CONTROL            0xC06
#define GPIO_MODE               0xC0C
#define GPIO_DIRECTION          0xC0D
#define GPIO_INT_MASK           0xC11

#define GPIO_SET                0xC0E
#define PIN_PULLUP_EN           0xC14
#define PIN_PULLDOWN_EN         0xC15
*/

//------------------------------------------------------------------------------
static int dev_set_reg(libusb_device_handle *device_handle, uint16_t wIndex, uint16_t 	wValue)
{
    uint8_t 	bmRequestType = LIBUSB_ENDPOINT_OUT | LIBUSB_REQUEST_TYPE_VENDOR;
    uint8_t 	bRequest      = XR_SET_REG;
    uint16_t 	wLength       = 0;
    uint16_t    *data         = 0;
    unsigned int timeout      = 5000;

    return libusb_control_transfer( device_handle, bmRequestType, bRequest, wValue, wIndex, (unsigned char*)data, wLength, timeout );
}
//------------------------------------------------------------------------------
static int dev_get_reg(libusb_device_handle *device_handle, uint16_t wIndex, uint16_t *data)
{
    uint8_t 	bmRequestType = LIBUSB_ENDPOINT_IN | LIBUSB_REQUEST_TYPE_VENDOR;
    uint8_t 	bRequest      = XR_GETN_REG;
    uint16_t 	wValue        = 0;
    uint16_t 	wLength       = 2;
    unsigned int timeout      = 5000;

    return libusb_control_transfer( device_handle, bmRequestType, bRequest, wValue, wIndex, (unsigned char*)data, wLength, timeout );
}
//------------------------------------------------------------------------------

int main(int argc, char** argv)
{
    if (argc !=3)
    {
        cout << "Correct device USB-to-COM. Only for 0x04e2 vendor (Exar corp)." << endl <<
                "Usage: corExar BUS DEVICE" << endl;
    }
    else
    {
        int usbBus = atoi(argv[1]);
        int usbDev = atoi(argv[2]);
        int vendor = 0x04e2;

        bool bFreeList = false;

        libusb_context *ctx;
        if (libusb_init(&ctx)<0)
        {
            cout << "Error: Initializing libusb" << endl;
            return -1;
        }

        libusb_device **device_list;
        ssize_t device_count = libusb_get_device_list(ctx, &device_list);

        for(ssize_t i = 0; i < device_count; ++i)
        {
            libusb_device *dev = device_list[i];

            if ( libusb_get_bus_number(dev) == usbBus )
                if ( libusb_get_device_address(dev) == usbDev )
                {
                    libusb_device_descriptor device_descriptor;

                    if ( libusb_get_device_descriptor(dev, &device_descriptor) >=0 )
                        if ( device_descriptor.idVendor == vendor )
                        {
                            libusb_device_handle *device_handle;

                            if ( libusb_open(dev,&device_handle) >=0 )
                            {
                                bFreeList = true;
                                libusb_free_device_list(device_list, 1);

                                //-----------------------------------------------------
                                cout << "Found exar." << endl;

                                if ( (dev_set_reg(device_handle, CDC_ACM_FLOW_CONTROL,    0) >= 0) &&
                                     (dev_set_reg(device_handle, CDC_ACM_GPIO_MODE,      11) >= 0) &&
                                     (dev_set_reg(device_handle, CDC_ACM_GPIO_DIRECTION, 40) >= 0) &&
                                     (dev_set_reg(device_handle, CDC_ACM_GPIO_INT_MASK,   0) >= 0) )
                                    cout << "Correction." << endl;
                                else cout << "Failed." << endl;
                                //-----------------------------------------------------

                                libusb_close(device_handle);
                            }
                        }
                }
        }

        if (!bFreeList)
        {
            libusb_free_device_list(device_list, 1);
            cout << "Exar not found." << endl;
        }
        libusb_exit(ctx);
    }

    return 0;
}

