#ifndef GUI_MAINWINDOW_H
#define GUI_MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "debugdialog.h"
#include "ppu_viewer_dialog.h"
#include "emulator.h"
#include "sdlwidget.h"
#include "settings.h"

namespace Ui {
    class GUI_MainWindow;
}

class GUI_MainWindow : public QMainWindow {
    Q_OBJECT
public:
    GUI_MainWindow(QWidget *parent = 0);
    ~GUI_MainWindow();

    void SetEmulatorPtr(Emulator *ptr);
    void ShowDebugDialog();
    void Start();
protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);
    void paintEvent(QPaintEvent *e);
    void closeEvent(QCloseEvent *e);





private:
    Emulator *emu;
    Ui::GUI_MainWindow *ui;
    DebugDialog *debug_dialog;
    PPU_viewer_dialog *ppu_dialog;
    Settings *settings_dialog;

    QLabel *file_name; //used for the status bar
    QTimer *frame_limiter;
    QTimer *FPSTimer;

    QSDLScreenWidget *renderer;




private slots:
    void on_actionDump_stac_triggered();
    void on_actionGo_fullscreen_triggered();
    void on_actionConfigure_triggered();
    void on_actionSoft_reset_triggered();
    void on_actionEnableSound_triggered();
    void on_action_Halt_CPU_triggered();
    void on_actionTurn_on_triggered();
    void on_actionPPU_triggered();
    void on_actionReset_wrappers_triggered();
    void on_action_Start_CPU_triggered();
    void on_action_Exit_triggered();
    void on_actionCPU_triggered();
    void on_action_Load_ROM_triggered();
    void updateWindowTitle();
};

#endif // GUI_MAINWINDOW_H
