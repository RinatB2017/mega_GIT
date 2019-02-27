#pragma once
#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H

#include <QDialog>
//#include <Emulator.h>
#include <breakpoint_dialog.h>
#include <QModelIndex>
#include <QListWidgetItem>

class Emulator;
namespace Ui {
    class DebugDialog;
}

class DebugDialog : public QDialog {
    Q_OBJECT
public:
    DebugDialog(QWidget *parent = 0);
    ~DebugDialog();
    void SetEmulatorPtr(Emulator *ptr) { emu = ptr; bp_dlg->SetEmuPtr(ptr); }

    void PopulateDisassembly();
    void UpdateDebugStats();

    void highlightbreakpoint();

protected:
    void changeEvent(QEvent *e);
    void paintEvent(QPaintEvent *);

private:
    Ui::DebugDialog *ui;
    Emulator *emu;

    bool disable_redraw;
    CPU_SNAPSHOT debug_snapshot;
    breakpoint_dialog *bp_dlg;





private slots:
    void on_resetButton_clicked();
    void on_bp_table_widget_itemClicked(QListWidgetItem* item);
    void on_remove_breakpointButton_clicked();
    void on_PC_field_selectionChanged();
    void on_PC_field_returnPressed();
    void on_pushButton_4_clicked();
    void on_pushButton_2_clicked();
    void on_add_breakpoint_button_clicked();

    void on_pushButton_clicked();
    void on_STEPbutton_clicked();
};

#endif // DEBUGDIALOG_H
