#include "switch_widget.h"

#include <QHBoxLayout>
#include <QPainter>
#include <QVariantAnimation>

safe::SwitchWidget::SwitchWidget(const QPixmap& image,
                                 int row, int column,
                                 Orientation orient,
                                 QWidget* parent)
    : QPushButton(parent)
    , originalImage_(image)
    , imageSize_(image.width())
    , row_(row)
    , column_(column)
    , orient_(orient)
    , angleDegrees_(orientationToDegrees(orient))
{
    setFixedSize(imageSize_, imageSize_);

    animation_ = new QVariantAnimation(this);
    animation_->setDuration(DurationOfAnimationMsec);
    connect(animation_, &QVariantAnimation::valueChanged, this, &SwitchWidget::onAnimationValueChanged);
    connect(animation_, &QVariantAnimation::finished, this, &SwitchWidget::onAnimationDone);

    connect(this, &QPushButton::clicked,
        [this]()
        {
            emit clicked(row_, column_);
        });
}

void safe::SwitchWidget::paintEvent(QPaintEvent* /*event*/)
{
    QTransform transform;
    transform.translate(imageSize_ / 2, imageSize_ / 2);
    transform.rotate(angleDegrees_);

    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.setTransform(transform);
    painter.drawPixmap(-imageSize_ / 2, -imageSize_ / 2, originalImage_);
}

safe::Orientation safe::SwitchWidget::orientation() const
{
    return orient_;
}

void safe::SwitchWidget::changeOrientation()
{
    const auto newOrient = orient_ == Vertical
        ? Horizontal
        : Vertical;

    animation_->setStartValue(orientationToDegrees(orient_));
    animation_->setEndValue(orientationToDegrees(newOrient));

    orient_ = newOrient;

    animation_->start();
}

void safe::SwitchWidget::onAnimationValueChanged(const QVariant& value)
{
    angleDegrees_ = value.toInt();
    update();
}

void safe::SwitchWidget::onAnimationDone()
{
    emit orientationChanged(orient_);
}

int safe::SwitchWidget::orientationToDegrees(Orientation orient) const
{
    return orient == Vertical
        ? 0
        : 90;
}
