/********************************************************************************
** Form generated from reading UI file 'breakpoint_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BREAKPOINT_DIALOG_H
#define UI_BREAKPOINT_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_breakpoint_dialog
{
public:
    QPushButton *OK_Button;
    QPushButton *Cancel_Button;
    QCheckBox *BP_Enabled_chkbx;
    QGroupBox *groupBox;
    QRadioButton *radioExec;
    QLineEdit *memory_access_adress;
    QRadioButton *radioExec_2;
    QGroupBox *groupBox_2;
    QLineEdit *opcode_linedit;
    QRadioButton *radioNMI;
    QRadioButton *radioOpcode;
    QRadioButton *radioIRQ;
    QRadioButton *radioBRK;

    void setupUi(QWidget *breakpoint_dialog)
    {
        if (breakpoint_dialog->objectName().isEmpty())
            breakpoint_dialog->setObjectName(QStringLiteral("breakpoint_dialog"));
        breakpoint_dialog->resize(179, 228);
        OK_Button = new QPushButton(breakpoint_dialog);
        OK_Button->setObjectName(QStringLiteral("OK_Button"));
        OK_Button->setGeometry(QRect(100, 170, 75, 23));
        Cancel_Button = new QPushButton(breakpoint_dialog);
        Cancel_Button->setObjectName(QStringLiteral("Cancel_Button"));
        Cancel_Button->setGeometry(QRect(100, 200, 75, 23));
        BP_Enabled_chkbx = new QCheckBox(breakpoint_dialog);
        BP_Enabled_chkbx->setObjectName(QStringLiteral("BP_Enabled_chkbx"));
        BP_Enabled_chkbx->setGeometry(QRect(10, 170, 61, 17));
        BP_Enabled_chkbx->setChecked(true);
        groupBox = new QGroupBox(breakpoint_dialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 0, 151, 81));
        radioExec = new QRadioButton(groupBox);
        radioExec->setObjectName(QStringLiteral("radioExec"));
        radioExec->setGeometry(QRect(0, 20, 82, 17));
        memory_access_adress = new QLineEdit(groupBox);
        memory_access_adress->setObjectName(QStringLiteral("memory_access_adress"));
        memory_access_adress->setGeometry(QRect(100, 20, 41, 20));
        radioExec_2 = new QRadioButton(groupBox);
        radioExec_2->setObjectName(QStringLiteral("radioExec_2"));
        radioExec_2->setGeometry(QRect(0, 40, 82, 17));
        groupBox_2 = new QGroupBox(breakpoint_dialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(0, 80, 151, 81));
        opcode_linedit = new QLineEdit(groupBox_2);
        opcode_linedit->setObjectName(QStringLiteral("opcode_linedit"));
        opcode_linedit->setGeometry(QRect(120, 40, 21, 20));
        radioNMI = new QRadioButton(groupBox_2);
        radioNMI->setObjectName(QStringLiteral("radioNMI"));
        radioNMI->setGeometry(QRect(10, 20, 41, 17));
        radioOpcode = new QRadioButton(groupBox_2);
        radioOpcode->setObjectName(QStringLiteral("radioOpcode"));
        radioOpcode->setGeometry(QRect(60, 40, 61, 17));
        radioIRQ = new QRadioButton(groupBox_2);
        radioIRQ->setObjectName(QStringLiteral("radioIRQ"));
        radioIRQ->setGeometry(QRect(60, 20, 41, 17));
        radioBRK = new QRadioButton(groupBox_2);
        radioBRK->setObjectName(QStringLiteral("radioBRK"));
        radioBRK->setGeometry(QRect(10, 40, 41, 17));

        retranslateUi(breakpoint_dialog);

        QMetaObject::connectSlotsByName(breakpoint_dialog);
    } // setupUi

    void retranslateUi(QWidget *breakpoint_dialog)
    {
        breakpoint_dialog->setWindowTitle(QApplication::translate("breakpoint_dialog", "Breakpoints", 0));
        OK_Button->setText(QApplication::translate("breakpoint_dialog", "&OK", 0));
        Cancel_Button->setText(QApplication::translate("breakpoint_dialog", "&Cancel", 0));
        BP_Enabled_chkbx->setText(QApplication::translate("breakpoint_dialog", "Enabled", 0));
        groupBox->setTitle(QApplication::translate("breakpoint_dialog", "Memory access", 0));
        radioExec->setText(QApplication::translate("breakpoint_dialog", "Exec", 0));
        radioExec_2->setText(QApplication::translate("breakpoint_dialog", "Read from", 0));
        groupBox_2->setTitle(QApplication::translate("breakpoint_dialog", "GroupBox", 0));
        radioNMI->setText(QApplication::translate("breakpoint_dialog", "NMI", 0));
        radioOpcode->setText(QApplication::translate("breakpoint_dialog", "opcode", 0));
        radioIRQ->setText(QApplication::translate("breakpoint_dialog", "IRQ", 0));
        radioBRK->setText(QApplication::translate("breakpoint_dialog", "BRK", 0));
    } // retranslateUi

};

namespace Ui {
    class breakpoint_dialog: public Ui_breakpoint_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BREAKPOINT_DIALOG_H
