#pragma once
#include <QObject>

class Worker : public QObject {
    Q_OBJECT
public:
    explicit Worker(QObject *parent = nullptr);

public slots:
    void doHeavyWork();

signals:
    void progressChanged(int value);
    void finished();
};
