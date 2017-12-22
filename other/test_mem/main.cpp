
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <QCoreApplication>
#include <QFileInfo>
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

        return fd;
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

#if 0
    QFileInfo info1("/dev/kmem");
    qDebug() << info1.size();
#else
    qDebug() << open_memdev();
#endif

    return a.exec();
}
