#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <opencv2/imgproc.hpp>

#include <QWidget>
#include <QSettings>

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget( QWidget* parent = nullptr );
    ~MainWidget();

private slots:
    void onLoad();
    void refreshHSV();

private:
    Ui::MainWidget* ui;
    cv::Mat m_mat;

    QString m_lastLoadPath;
    QSettings m_settings;

};

#endif // MAINWIDGET_H
