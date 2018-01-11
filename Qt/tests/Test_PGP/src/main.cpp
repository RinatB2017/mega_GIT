#include <QtCrypto>
#include <QApplication>
#include "CryptWin.h"
int main(int argc, char *argv[])
{
	QCA::Initializer init;
	QApplication a(argc, argv);
	CryptWindow w;
	w.show();
	return a.exec();
}
