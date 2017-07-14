/********************************************************************************
** Form generated from reading UI file 'debugdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUGDIALOG_H
#define UI_DEBUGDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_DebugDialog
{
public:
    QGroupBox *groupBox;
    QPlainTextEdit *disassembly_view;
    QPushButton *add_breakpoint_button;
    QPushButton *remove_breakpointButton;
    QListWidget *bp_table_widget;
    QPlainTextEdit *mem_view;
    QGroupBox *groupBox_3;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *X_field;
    QLineEdit *A_field;
    QLabel *label_3;
    QLineEdit *Y_field;
    QLineEdit *SP_field;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *SR_field;
    QCheckBox *chk_N;
    QCheckBox *chk_V;
    QCheckBox *chk_U;
    QCheckBox *chk_B;
    QCheckBox *chk_D;
    QCheckBox *chk_I;
    QCheckBox *chk_Z;
    QCheckBox *chk_C;
    QGroupBox *groupBox_2;
    QPushButton *pushButton;
    QLineEdit *PC_field;
    QPushButton *pushButton_2;
    QPushButton *STEPbutton;
    QPushButton *pushButton_4;
    QPushButton *resetButton;
    QLabel *scanline_lbl;
    QLabel *NMI_label;
    QLabel *BRK_label;

    void setupUi(QDialog *DebugDialog)
    {
        if (DebugDialog->objectName().isEmpty())
            DebugDialog->setObjectName(QStringLiteral("DebugDialog"));
        DebugDialog->resize(400, 500);
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(DebugDialog->sizePolicy().hasHeightForWidth());
        DebugDialog->setSizePolicy(sizePolicy);
        DebugDialog->setMinimumSize(QSize(400, 500));
        DebugDialog->setMaximumSize(QSize(400, 500));
        groupBox = new QGroupBox(DebugDialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 200, 391, 291));
        disassembly_view = new QPlainTextEdit(groupBox);
        disassembly_view->setObjectName(QStringLiteral("disassembly_view"));
        disassembly_view->setGeometry(QRect(190, 20, 201, 171));
        disassembly_view->setAcceptDrops(false);
        disassembly_view->setUndoRedoEnabled(false);
        disassembly_view->setLineWrapMode(QPlainTextEdit::NoWrap);
        disassembly_view->setReadOnly(false);
        disassembly_view->setTextInteractionFlags(Qt::TextEditorInteraction);
        disassembly_view->setBackgroundVisible(false);
        disassembly_view->setCenterOnScroll(true);
        add_breakpoint_button = new QPushButton(groupBox);
        add_breakpoint_button->setObjectName(QStringLiteral("add_breakpoint_button"));
        add_breakpoint_button->setGeometry(QRect(300, 220, 81, 23));
        remove_breakpointButton = new QPushButton(groupBox);
        remove_breakpointButton->setObjectName(QStringLiteral("remove_breakpointButton"));
        remove_breakpointButton->setGeometry(QRect(300, 246, 81, 23));
        bp_table_widget = new QListWidget(groupBox);
        bp_table_widget->setObjectName(QStringLiteral("bp_table_widget"));
        bp_table_widget->setGeometry(QRect(10, 200, 271, 81));
        mem_view = new QPlainTextEdit(groupBox);
        mem_view->setObjectName(QStringLiteral("mem_view"));
        mem_view->setGeometry(QRect(0, 20, 191, 171));
        QFont font;
        font.setFamily(QStringLiteral("MS Shell Dlg 2"));
        font.setPointSize(7);
        mem_view->setFont(font);
        groupBox_3 = new QGroupBox(DebugDialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(0, 0, 231, 111));
        label = new QLabel(groupBox_3);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 20, 16, 16));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(50, 20, 16, 16));
        X_field = new QLineEdit(groupBox_3);
        X_field->setObjectName(QStringLiteral("X_field"));
        X_field->setGeometry(QRect(20, 20, 21, 16));
        A_field = new QLineEdit(groupBox_3);
        A_field->setObjectName(QStringLiteral("A_field"));
        A_field->setGeometry(QRect(70, 20, 21, 16));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(10, 40, 16, 16));
        Y_field = new QLineEdit(groupBox_3);
        Y_field->setObjectName(QStringLiteral("Y_field"));
        Y_field->setGeometry(QRect(20, 40, 21, 16));
        SP_field = new QLineEdit(groupBox_3);
        SP_field->setObjectName(QStringLiteral("SP_field"));
        SP_field->setGeometry(QRect(70, 40, 21, 16));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(47, 40, 16, 16));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(7, 60, 16, 16));
        SR_field = new QLineEdit(groupBox_3);
        SR_field->setObjectName(QStringLiteral("SR_field"));
        SR_field->setGeometry(QRect(30, 60, 21, 16));
        chk_N = new QCheckBox(groupBox_3);
        chk_N->setObjectName(QStringLiteral("chk_N"));
        chk_N->setGeometry(QRect(4, 90, 30, 17));
        chk_N->setCheckable(true);
        chk_V = new QCheckBox(groupBox_3);
        chk_V->setObjectName(QStringLiteral("chk_V"));
        chk_V->setGeometry(QRect(31, 90, 31, 17));
        chk_V->setCheckable(true);
        chk_U = new QCheckBox(groupBox_3);
        chk_U->setObjectName(QStringLiteral("chk_U"));
        chk_U->setEnabled(false);
        chk_U->setGeometry(QRect(58, 90, 31, 17));
        chk_U->setCheckable(false);
        chk_B = new QCheckBox(groupBox_3);
        chk_B->setObjectName(QStringLiteral("chk_B"));
        chk_B->setEnabled(false);
        chk_B->setGeometry(QRect(85, 90, 31, 17));
        chk_B->setCheckable(false);
        chk_D = new QCheckBox(groupBox_3);
        chk_D->setObjectName(QStringLiteral("chk_D"));
        chk_D->setGeometry(QRect(112, 90, 31, 17));
        chk_D->setCheckable(true);
        chk_I = new QCheckBox(groupBox_3);
        chk_I->setObjectName(QStringLiteral("chk_I"));
        chk_I->setGeometry(QRect(139, 90, 31, 17));
        chk_I->setCheckable(true);
        chk_Z = new QCheckBox(groupBox_3);
        chk_Z->setObjectName(QStringLiteral("chk_Z"));
        chk_Z->setGeometry(QRect(168, 90, 30, 17));
        chk_Z->setCheckable(true);
        chk_C = new QCheckBox(groupBox_3);
        chk_C->setObjectName(QStringLiteral("chk_C"));
        chk_C->setGeometry(QRect(194, 90, 31, 17));
        chk_C->setCheckable(true);
        groupBox_2 = new QGroupBox(DebugDialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(230, 0, 161, 141));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        pushButton->setGeometry(QRect(60, 110, 75, 23));
        PC_field = new QLineEdit(groupBox_2);
        PC_field->setObjectName(QStringLiteral("PC_field"));
        PC_field->setGeometry(QRect(10, 110, 41, 20));
        PC_field->setMaxLength(4);
        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QStringLiteral("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 10, 41, 23));
        STEPbutton = new QPushButton(groupBox_2);
        STEPbutton->setObjectName(QStringLiteral("STEPbutton"));
        STEPbutton->setGeometry(QRect(60, 10, 41, 23));
        pushButton_4 = new QPushButton(groupBox_2);
        pushButton_4->setObjectName(QStringLiteral("pushButton_4"));
        pushButton_4->setGeometry(QRect(110, 10, 41, 23));
        resetButton = new QPushButton(groupBox_2);
        resetButton->setObjectName(QStringLiteral("resetButton"));
        resetButton->setGeometry(QRect(60, 40, 41, 23));
        scanline_lbl = new QLabel(DebugDialog);
        scanline_lbl->setObjectName(QStringLiteral("scanline_lbl"));
        scanline_lbl->setGeometry(QRect(10, 116, 61, 16));
        NMI_label = new QLabel(DebugDialog);
        NMI_label->setObjectName(QStringLiteral("NMI_label"));
        NMI_label->setGeometry(QRect(10, 140, 111, 16));
        BRK_label = new QLabel(DebugDialog);
        BRK_label->setObjectName(QStringLiteral("BRK_label"));
        BRK_label->setGeometry(QRect(10, 160, 131, 16));

        retranslateUi(DebugDialog);

        QMetaObject::connectSlotsByName(DebugDialog);
    } // setupUi

    void retranslateUi(QDialog *DebugDialog)
    {
        DebugDialog->setWindowTitle(QApplication::translate("DebugDialog", "CPU tracer", 0));
        groupBox->setTitle(QApplication::translate("DebugDialog", "Tracer", 0));
        add_breakpoint_button->setText(QApplication::translate("DebugDialog", "Add breakpoint", 0));
        remove_breakpointButton->setText(QApplication::translate("DebugDialog", "Remove BP", 0));
        groupBox_3->setTitle(QApplication::translate("DebugDialog", "Registers", 0));
        label->setText(QApplication::translate("DebugDialog", "X", 0));
        label_2->setText(QApplication::translate("DebugDialog", "A", 0));
        label_3->setText(QApplication::translate("DebugDialog", "Y", 0));
        label_4->setText(QApplication::translate("DebugDialog", "SP", 0));
        label_5->setText(QApplication::translate("DebugDialog", "SR", 0));
        chk_N->setText(QApplication::translate("DebugDialog", "N", 0));
        chk_V->setText(QApplication::translate("DebugDialog", "V", 0));
        chk_U->setText(QApplication::translate("DebugDialog", "U", 0));
        chk_B->setText(QApplication::translate("DebugDialog", "B", 0));
        chk_D->setText(QApplication::translate("DebugDialog", "D", 0));
        chk_I->setText(QApplication::translate("DebugDialog", "I", 0));
        chk_Z->setText(QApplication::translate("DebugDialog", "Z", 0));
        chk_C->setText(QApplication::translate("DebugDialog", "C", 0));
        groupBox_2->setTitle(QString());
        pushButton->setText(QApplication::translate("DebugDialog", "Goto", 0));
        pushButton_2->setText(QApplication::translate("DebugDialog", "Run", 0));
        STEPbutton->setText(QApplication::translate("DebugDialog", "Step", 0));
        pushButton_4->setText(QApplication::translate("DebugDialog", "Halt", 0));
        resetButton->setText(QApplication::translate("DebugDialog", "Reset", 0));
        scanline_lbl->setText(QApplication::translate("DebugDialog", "Scanline", 0));
        NMI_label->setText(QString());
        BRK_label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class DebugDialog: public Ui_DebugDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGDIALOG_H
