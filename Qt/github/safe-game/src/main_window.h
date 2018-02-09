#pragma once

#include <QMainWindow>

class QComboBox;
class QFrame;
class QLabel;
class QLayout;
class QTimer;

namespace safe
{
    class History;

    class MainWindow
        : public QMainWindow
    {
        Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);
        ~MainWindow();

    public slots:
        void onPlayerWon();

    private slots:
        void onStartNewGame();
        void onUpdateTime();
        void onShowHallOfFame();
        void onShowAbout();

    private:
        void showTime();

        void createMenu();

    private:
        static constexpr int MinSize = 4;
        static constexpr int MaxSize = 10;

        QTimer* updateTime_;

        QLabel* time_;

        QLayout* placeForSafe_;
        QComboBox* size_;

        int secondsFromStart_ = 0;

        History* history_;
    };
}
