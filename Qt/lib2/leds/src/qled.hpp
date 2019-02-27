//--------------------------------------------------------------------------------
#ifndef QLED_H
#define QLED_H
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
class QLed : public QWidget
{
    Q_OBJECT
public:
    explicit QLed(unsigned int width,
                  unsigned int height,
                  const QString &tooltip,
                  const QColor &color_off = Qt::gray,
                  const QColor &color_on = Qt::red,
                  QWidget *parent = nullptr);
    void setState(bool state); 

private:
    QColor color_off;
    QColor color_on;

    QColor current_color = color_off;

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif
