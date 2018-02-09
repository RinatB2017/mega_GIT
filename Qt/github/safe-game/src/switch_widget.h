#pragma once

#include <QPushButton>

#include "orientation.h"

class QVariantAnimation;

namespace safe
{
    class SwitchWidget
        : public QPushButton
    {
        Q_OBJECT
    public:
        explicit SwitchWidget(const QPixmap& image, int row, int column, Orientation orient, QWidget* parent);

        Orientation orientation() const;
        void changeOrientation();

    signals:
        void orientationChanged(Orientation orient);
        void clicked(int row, int column);

    protected:
        void paintEvent(QPaintEvent* event);

    private slots:
        void onAnimationValueChanged(const QVariant& value);
        void onAnimationDone();

    private:
        int orientationToDegrees(Orientation orient) const;

    private:
        const QPixmap& originalImage_;
        const int imageSize_;

        const int row_;
        const int column_;

        Orientation orient_;

        int angleDegrees_;

        static constexpr int DurationOfAnimationMsec = 350;

        QVariantAnimation* animation_;
    };
}
