
#ifndef SLEEPER_H
#define SLEEPER_H

#include <QThread>

class Sleeper : QThread
{
    public:
	static void sleep( unsigned long secs ) { QThread::sleep( secs ); }
	static void msleep( unsigned long msecs ) { QThread::msleep( msecs ); }
	static void usleep( unsigned long usecs ) { QThread::usleep( usecs ); }
};

#endif	// _SLEEPER_H
