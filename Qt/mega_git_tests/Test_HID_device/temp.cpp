//  #define VID 0xC251
//  #define PID 0x1701

#define REPORT_LENGTH   256

#include <stdlib.h>
#include <unistd.h>

#include <QDir>
#include <QLibrary>

#include "hidapi.h"

typedef struct hid_device_info* (*HID_ENUMERATE)(unsigned short, unsigned short);
typedef void (*HID_FREE_ENUMARATION)(struct hid_device_info*);
typedef hid_device* (*HID_OPEN)(unsigned short, unsigned short, wchar_t*);
typedef int (*HID_GET_MANUFACTURER_STRING)(hid_device *, wchar_t *, size_t);
typedef int (*HID_GET_PRODUCT_STRING)(hid_device*, wchar_t*, size_t);
typedef int (*HID_GET_SERIAL_NUMBER_STRING)(hid_device*, wchar_t*, size_t);
typedef int (*HID_SET_NONBLOCKING)(hid_device*, int);
typedef int (*HID_SEND_FEATURE_REPORT)(hid_device*, const unsigned char*, size_t);
typedef int (*HID_GET_FEATURE_REPORT)(hid_device*, unsigned char*, size_t);

class hidControl
{
private:
    HID_OPEN hid_open;
    HID_SET_NONBLOCKING hid_set_nonblocking;
    HID_SEND_FEATURE_REPORT hid_send_feature_report;
    HID_GET_FEATURE_REPORT hid_get_feature_report;

public:
    hidControl()
    {}

    ~hidControl()
    {}

    unsigned char buf[REPORT_LENGTH];
    bool deviceFound;
    hid_device *handle;

    // Инициализация HID
    int init(unsigned int VID, unsigned int PID)
    {
        // Пробуем подключить библиотеку
        QDir curdir;
        QLibrary lib(curdir.absolutePath() + "/hidapi");

        lib.load();

        if (!lib.isLoaded())
            return -1;

        hid_open = (HID_OPEN)lib.resolve("hid_open");
        hid_set_nonblocking = (HID_SET_NONBLOCKING)lib.resolve("hid_set_nonblocking");
        hid_send_feature_report = (HID_SEND_FEATURE_REPORT)lib.resolve("hid_send_feature_report");
        hid_get_feature_report = (HID_GET_FEATURE_REPORT)lib.resolve("hid_get_feature_report");

        deviceFound = false;

        handle = hid_open(VID, PID, NULL);

        if (handle)
        {
            deviceFound = true;
            hid_set_nonblocking(handle, 1);
        }
        else
            return -1;

        return 0;
    }

    int write()
    {
        buf[0] = 0x00;  // Идентификатор посылки всегда 0х00
        return hid_send_feature_report(handle, buf, REPORT_LENGTH);
    }

    int read()
    {
        buf[0] = 0x00;  // Идентификатор посылки всегда 0х00
        hid_send_feature_report(handle, buf, REPORT_LENGTH);
        usleep(10000);
        return hid_get_feature_report(handle, buf, REPORT_LENGTH);
    }
};


int main(int argc, char *argv[])
{
    hidControl *hid;
    hid = new hidControl();
    if (argc < 5)
    {
        printf("usage:\nusb.exe [VID] [PID] -r [CMD]\n");
        printf("or\nusb.exe [VID] [PID] -w [CMD]\n");
        printf("VID and PID must be in dec format, not hex\n");

        return -1;
    }
    if (hid->init(atoi(argv[1]), atoi(argv[2])) < 0)
        return -2;

    if (!strcmp(argv[4], "-r"))
    {
        int cmd = atoi(argv[4]);
        hid->buf[1] = cmd;
        hid->read();
        printf("readed data, cmd %d\n", argv[4]);
        printf("%s\n", hid->buf);
        return 1;
    }

    if (!strcmp(argv[4], "-w"))
    {
        strcpy((char*)hid->buf, (char*)argv[4]);
        hid->write();
        printf("writted data, cmd %s\n", argv[4]);
        return 2;
    }
    return 0;
}
