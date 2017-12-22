#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    image = QImage(":/Tulips.png");
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));

    // Отслеживаем изменение положения бегунка
    connect(ui->lightnessSlider, &QSlider::valueChanged, this, &Widget::setLightness);      // Яркость
    connect(ui->saturationSlider, &QSlider::valueChanged, this, &Widget::setSaturation);    // Насыщенность

    // Если яркость или насущенность измененились, то изенеяем изображение
    connect(this, &Widget::lightnessChanged, this, &Widget::changeImageLihtness);
    connect(this, &Widget::saturationChanged, this, &Widget::changeImageSaturation);
}

Widget::~Widget()
{
    delete ui;
}

int Widget::lightness() const
{
    return m_lightness;
}

int Widget::saturation() const
{
    return m_saturation;
}

void Widget::setLightness(int lightness)
{
    if (m_lightness == lightness)
        return;

    m_lightness = lightness;
    emit lightnessChanged(lightness);
}

void Widget::setSaturation(int saturation)
{
    if (m_saturation == saturation)
        return;

    m_saturation = saturation;
    emit saturationChanged(saturation);
}

void Widget::changeImageLihtness(int value)
{
    auto width = image.width();
    auto height = image.height();

    /* Проходим по каждому пикселю в картинке и меняем его яркость.
     * Для этого используем класс QColor и цветовую модель hsl.
     * Существует несколько цветовых моделей:
     * RGB, CMYK, HSV, HSL - QColor поддерживает их все
     * Но для изменения яркости необходимо менять компоненту L в модели HSL.
     * - это наиболее простой вариант
     * */
    for (auto x = 0; x < width; x++)
    {
        for (auto y = 0; y < height; y++)
        {
            QColor color = image.pixelColor(x, y);
            color.setHslF(color.hslHueF(),
                          color.hslSaturationF(),
                          (qreal)value/100);
            image.setPixelColor(x, y, color);
        }
    }
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
}

void Widget::changeImageSaturation(int value)
{
    auto width = image.width();
    auto height = image.height();

    /* Проходим по каждому пикселю в картинке и меняем его яркость.
     * Для этого используем класс QColor и цветовую модель hsl.
     * Существует несколько цветовых моделей:
     * RGB, CMYK, HSV, HSL - QColor поддерживает их все
     * Но для изменения насыщенности необходимо менять компоненту S в модели HSL.
     * - это наиболее простой вариант
     * */
    for (auto x = 0; x < width; x++)
    {
        for (auto y = 0; y < height; y++)
        {
            QColor color = image.pixelColor(x, y);
            color.setHslF(color.hslHueF(),
                          (qreal)value/100,
                          color.lightnessF());
            image.setPixelColor(x, y, color);
        }
    }
    ui->imageLabel->setPixmap(QPixmap::fromImage(image));
}
