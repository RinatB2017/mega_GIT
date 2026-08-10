//--------------------------------------------------------------------------------
#ifndef FRAMEBOX_H
#define FRAMEBOX_H
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
class QVBoxLayout;
class QHBoxLayout;
class QLayout;
class QLabel;
//--------------------------------------------------------------------------------
class FrameBox : public QFrame
{
    Q_OBJECT

public:
    explicit FrameBox(QWidget *parent = nullptr);
    ~FrameBox();

public slots:
    void addWidget(QWidget *widget);
    void addLayout(QLayout *layout);

private:
    QVBoxLayout *vbox;
    QHBoxLayout *topbox;
    QHBoxLayout *middlebox;
    QHBoxLayout *bottombox;
    QVBoxLayout *workbox;

    QLabel *label1;
    QLabel *label2;
    QLabel *label3;
    QLabel *label4;

    QFrame *spaceframe;
    QFrame *spaceframe2;

    QPixmap p1;
    QPixmap p2;
    QPixmap p3;
    QPixmap p4;

    void loadStyle(const QString &name);

signals:
    void log(const QString &);

protected:
#if 0
    virtual void paintEvent ( QPaintEvent * event );
#endif
};
//--------------------------------------------------------------------------------
#endif
