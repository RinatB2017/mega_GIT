#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QImage>
#include "emulator.h"

#include "../emulator/input.h"


namespace Ui {
    class Settings;
}

class Settings : public QDialog {
    Q_OBJECT
public:
    explicit Settings(QWidget *parent = 0);
    ~Settings();

    void SetEmuPtr(Emulator *ptr) { emu = ptr; new_input=emu->GetInputHandle()->GetController(0);}

    void ShowCurrentKeymap();
protected:
    void changeEvent(QEvent *e);
    void keyPressEvent(QKeyEvent *e);
    void keyReleaseEvent(QKeyEvent *e);

private:
    Ui::Settings *ui;
    Emulator *emu;

    bool redef_keys;
    bool keypress;
    byte counter;
    controller_bindings *new_input;
    QImage *controller_bg;
private slots:
    void on_redefine_keys_button_clicked();


};

#endif // SETTINGS_H
