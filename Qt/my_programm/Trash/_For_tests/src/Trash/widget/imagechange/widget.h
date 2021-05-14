#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QImage>
#include <QPixmap>
#include <QColor>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int lightness READ lightness WRITE setLightness NOTIFY lightnessChanged)
    Q_PROPERTY(int saturation READ saturation WRITE setSaturation NOTIFY saturationChanged)

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    int lightness() const;
    int saturation() const;

public slots:
    void setLightness(int lightness);
    void setSaturation(int saturation);

signals:
    void lightnessChanged(int lightness);
    void saturationChanged(int saturation);

private slots:
    void changeImageLihtness(int value);
    void changeImageSaturation(int value);

private:
    Ui::Widget *ui;
    QImage image;
    int m_lightness;
    int m_saturation;
};

#endif // WIDGET_H
