
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <QDebug>

int32_t open_memdev()
{
        int32_t fd;

        fd = open("/dev/kmem", O_RDONLY);
        if (fd < 0)
        {
                printf("Failed to open /dev/kmem : %s\n", strerror(errno));
                return -EINVAL;
        }

        printf("OK\n");
        return fd;
}

int main(int argc, char *argv[])
{
#if 0
    QFileInfo info1("/dev/kmem");
    qDebug() << info1.size();
#else
    qDebug() << open_memdev();
#endif

    Q_UNUSED(argc);
    Q_UNUSED(argv);
    return 0;
}
