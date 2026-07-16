#include "worker.h"
#include <QThread>
#include <QDebug>

Worker::Worker(QObject *parent)
    : QObject(parent)
{
    // Конструктор пустой, родителя не присваиваем
}

void Worker::doHeavyWork()
{
    qDebug() << "Рабочий поток запущен. ID:" << QThread::currentThreadId();

    // Наш тяжелый непрерывный цикл
    for (int i = 0; i <= 500; ++i)
    {
        // Главная проверка безопасности: не нажал ли пользователь "Стоп"?
        if (QThread::currentThread()->isInterruptionRequested())
        {
            qDebug() << "Поток перехватил запрос на прерывание. Выходим...";
            break; // Корректно прерываем вычисления
        }

        // Имитация полезной нагрузки (выполняется на 100% мощности потока)
        QThread::msleep(50);

        // Отправляем данные в главный поток интерфейса
        emit progressChanged(i);
    }

    qDebug() << "Цикл в потоке завершен полностью.";
    emit finished(); // Сигнализируем главному окну, что мы закончили
}
