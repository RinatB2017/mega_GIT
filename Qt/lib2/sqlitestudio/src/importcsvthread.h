#ifndef IMPORTCSVTHREAD_H
#define IMPORTCSVTHREAD_H

#include <QThread>

class ImportCSVThread : public QThread
{
    Q_OBJECT
public:
    explicit ImportCSVThread(QObject *parent = 0);

signals:

public slots:

};

#endif // IMPORTCSVTHREAD_H
