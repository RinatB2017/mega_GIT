#ifndef VIDEOTEST_H_
#define VIDEOTEST_H_

#include <QtWidgets/QMainWindow>
#include "ui_videotest.h"

class VideoTest: public QMainWindow
{
    Q_OBJECT

    public:
        VideoTest(QWidget * p_parent = 0);
        ~VideoTest();

    public slots:
        void sourceChanged();

    private:
        struct Private;
        Private * d;
        Ui::VideoTestClass ui;
};

#endif
