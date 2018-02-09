#pragma once

#include <QFrame>
#include <QPixmap>

#include "orientation.h"

class QGridLayout;

namespace safe
{
    class History;

    class SafeWidget
        : public QFrame
    {
        Q_OBJECT
    public:
        SafeWidget(History* history, int size, QWidget* parent = nullptr);
        ~SafeWidget();

        void doSwitch(int row, int column);

    public slots:
        void onOrientationChanged(Orientation orient);
        void onSwitchClicked(int row, int column);

    signals:
        void playerWon();

    private slots:
        void onTryToSwitchNext();

    private:
        void checkWinCondition();
        void changeSwitchOrientation(int row, int column);
        bool isValidCoord(int row, int column) const;

    private:
        History* history_;

        QPixmap image_;

        const int size_;
        const int totalSwitches_;
        int horizontalSwitches_ = 0;
        int animatedSwitches_ = 0;

        bool gameFinished_ = false;

        QGridLayout* switches_;

        static constexpr int timeoutToAnimateNextMsec = 100;

        struct Lines
        {
            int row;
            int column;

            int up;
            int down;
            int left;
            int right;
        };

        Lines lines_ = {};
    };
}
