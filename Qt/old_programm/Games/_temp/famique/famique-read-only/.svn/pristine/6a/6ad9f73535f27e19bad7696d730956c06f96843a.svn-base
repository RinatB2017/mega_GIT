#ifndef PPU_VIEWER_DIALOG_H
#define PPU_VIEWER_DIALOG_H

#include <QDialog>
#include <QImage>
#include <QTimer>

class Emulator;

namespace Ui {
    class PPU_viewer_dialog;
}

class PPU_viewer_dialog : public QDialog {
    Q_OBJECT
public:
    PPU_viewer_dialog(QWidget *parent = 0);
    ~PPU_viewer_dialog();
    void SetEmuPtr(Emulator *ptr) { emu = ptr; }

protected:
    void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent *e);
    void showEvent(QShowEvent *event);
    void closeEvent(QCloseEvent *event);

private:
    Ui::PPU_viewer_dialog *ui;
    void DrawNametables(int nt);
    void DrawOAM();
    Emulator *emu;

    QImage *nametable;
    QTimer *timer;
    int nametable_selected;



private slots:
    void on_radio2C00_clicked();
    void on_radio2800_clicked();
    void on_radio2400_clicked();
    void on_radio2000_clicked();
};

#endif // PPU_VIEWER_DIALOG_H
