
#include <stddef.h>
#include <stdint.h>

#define HID_API_EXPORT /**< API export macro */
#define HID_API_CALL /**< API call macro */

#define HID_API_EXPORT_CALL HID_API_EXPORT HID_API_CALL /**< API export and call macro*/

#define HANDLE  int
#define BOOL    bool
#define USHORT  unsigned short
#define DWORD   int
#define ULONG_PTR unsigned int *
#define PVOID void *

typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        };
        PVOID  Pointer;
    };
    HANDLE    hEvent;
} OVERLAPPED, *LPOVERLAPPED;

struct hid_device {
    HANDLE device_handle;
    BOOL blocking;
    USHORT output_report_length;
    size_t input_report_length;
    void *last_error_str;
    DWORD last_error_num;
    BOOL read_pending;
    char *read_buf;
    OVERLAPPED ol;
};

/** hidapi info structure */
struct hid_device_info {
    /** Platform-specific device path */
    char *path;
    /** Device Vendor ID */
    unsigned short vendor_id;
    /** Device Product ID */
    unsigned short product_id;
    /** Serial Number */
    wchar_t *serial_number;
    /** Device Release Number in binary-coded decimal,
        also known as Device Version Number */
    unsigned short release_number;
    /** Manufacturer String */
    wchar_t *manufacturer_string;
    /** Product string */
    wchar_t *product_string;
    /** Usage Page for this Device/Interface
        (Windows/Mac only). */
    unsigned short usage_page;
    /** Usage for this Device/Interface
        (Windows/Mac only).*/
    unsigned short usage;
    /** The USB interface which this logical device
        represents. Valid on both Linux implementations
        in all cases, and valid on the Windows implementation
        only if the device contains more than one interface. */
    int interface_number;

    /** Pointer to the next device */
    struct hid_device_info *next;
};

struct hid_device_info HID_API_EXPORT * HID_API_CALL hid_enumerate(unsigned short vendor_id, unsigned short product_id)
{
    //hid_device_info i;
    return 0;
}

void  hid_free_enumeration(struct hid_device_info *devs)
{

}

hid_device * hid_open(unsigned short vendor_id, unsigned short product_id, const wchar_t *serial_number)
{
    //hid_device *hd;
    return 0;
}

int HID_API_EXPORT_CALL hid_get_manufacturer_string(hid_device *device, wchar_t *string, size_t maxlen)
{

}
