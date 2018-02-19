#include "addressbookmainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName( "stanislav888@github.com" ); // Using as settings folder in registry
    AddressBookMainWindow w;
    w.show();

    return a.exec();
}
