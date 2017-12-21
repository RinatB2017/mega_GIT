//--------------------------------------------------------------------------------
#ifndef QLED_H
#define QLED_H
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
class QLed : public QFrame
{
    Q_OBJECT
public:
    explicit QLed(unsigned int width,
                  unsigned int height,
                  const QString &tooltip,
                  const QColor &color = Qt::black,
                  QWidget *parent = 0);
    void setState(bool state);

signals:

public slots:    

private:
    QString ledColor;

};
//--------------------------------------------------------------------------------
#endif
