#include <linux/serial.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
       
/* Include definition for RS485 ioctls: TIOCGRS485 and TIOCSRS485 */
#include <sys/ioctl.h>

int main(void)
{
    printf("RS-485\n");
    /* Open your specific device (e.g., /dev/mydevice): */
    char *device = "/dev/ttyUSB1"; 
    printf("Open %s...\n", device);
    int fd = open(device, O_RDWR);
    if (fd < 0) {
        printf("Cannot open %s\n", device);
        printf("ERROR: %s\n", strerror(errno));
        return -1;
    }

    struct serial_rs485 rs485conf;

    /* Enable RS485 mode: */
    rs485conf.flags |= SER_RS485_ENABLED;

    /* Set logical level for RTS pin equal to 1 when sending: */
    rs485conf.flags |= SER_RS485_RTS_ON_SEND;
    /* or, set logical level for RTS pin equal to 0 when sending: */
    rs485conf.flags &= ~(SER_RS485_RTS_ON_SEND);

    /* Set logical level for RTS pin equal to 1 after sending: */
    rs485conf.flags |= SER_RS485_RTS_AFTER_SEND;
    /* or, set logical level for RTS pin equal to 0 after sending: */
    rs485conf.flags &= ~(SER_RS485_RTS_AFTER_SEND);

    /* Set rts delay before send, if needed: */
    rs485conf.delay_rts_before_send = 0; //TODO

    /* Set rts delay after send, if needed: */
    rs485conf.delay_rts_after_send = 0; //TODO

    /* Set this flag if you want to receive data even whilst sending data */
    rs485conf.flags |= SER_RS485_RX_DURING_TX;

    if (ioctl(fd, TIOCSRS485, &rs485conf) < 0) {
        printf("ERROR: ioctl < 0 errno %d\n", errno);
        printf("ERROR: %s\n", strerror(errno));
        return -1;
    }

    /* Use read() and write() syscalls here... */

    /* Close the device when finished: */
    if (close(fd) < 0) {
        printf("Cannot close\n");
        printf("ERROR: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}
