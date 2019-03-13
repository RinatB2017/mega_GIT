#ifndef IMPORTCSVTHREAD_H
#define IMPORTCSVTHREAD_H

#include <QThread>

class ImportCSVThread : public QThread
{
    Q_OBJECT
public:
    explicit ImportCSVThread(QObject *parent = nullptr);

signals:

public slots:

};

#endif // IMPORTCSVTHREAD_H
