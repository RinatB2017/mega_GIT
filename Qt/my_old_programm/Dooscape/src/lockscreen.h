#ifndef LOCKSCREEN_H
#define LOCKSCREEN_H

#include <QtWidgets>

class LockScreen : public QFrame
{
    Q_OBJECT
    public:
        explicit LockScreen(QWidget *parent = 0);

    signals:
        void canUnlock();

    public slots:
        void launch();

    protected:
        void resizeEvent(QResizeEvent *event);

    private slots:
        void applyStyle();

        void showIndication();
        void unlock();

    private:
        QWidget *widgetInfo;
        QSettings *settings;
        QLineEdit *editPass;
        QLabel *lblText;

};

#endif // LOCKSCREEN_H
