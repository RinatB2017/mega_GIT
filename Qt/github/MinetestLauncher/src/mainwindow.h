#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QDir>
#include "createdialog.h"
#include "datamanager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void newProfile();
    void deleteSelected();
    void playSelected();

private:
    Ui::MainWindow *ui;
    DataManager *dataManager = new DataManager(QDir::homePath() + "/.minetest/minetestlauncher.conf");
    ProfileManager *profileManager = new ProfileManager();
};

#endif // MAINWINDOW_H
