/********************************************************************************
** Form generated from reading UI file 'ppu_viewer_dialog.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PPU_VIEWER_DIALOG_H
#define UI_PPU_VIEWER_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_PPU_viewer_dialog
{
public:
    QGroupBox *groupBox;
    QGroupBox *groupBox_2;
    QRadioButton *radio2000;
    QRadioButton *radio2400;
    QRadioButton *radio2800;
    QRadioButton *radio2C00;
    QGroupBox *groupBox_3;

    void setupUi(QDialog *PPU_viewer_dialog)
    {
        if (PPU_viewer_dialog->objectName().isEmpty())
            PPU_viewer_dialog->setObjectName(QStringLiteral("PPU_viewer_dialog"));
        PPU_viewer_dialog->resize(512, 480);
        groupBox = new QGroupBox(PPU_viewer_dialog);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(0, 0, 260, 271));
        groupBox_2 = new QGroupBox(PPU_viewer_dialog);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(260, 0, 71, 141));
        radio2000 = new QRadioButton(groupBox_2);
        radio2000->setObjectName(QStringLiteral("radio2000"));
        radio2000->setGeometry(QRect(0, 20, 61, 17));
        radio2400 = new QRadioButton(groupBox_2);
        radio2400->setObjectName(QStringLiteral("radio2400"));
        radio2400->setGeometry(QRect(0, 40, 61, 17));
        radio2800 = new QRadioButton(groupBox_2);
        radio2800->setObjectName(QStringLiteral("radio2800"));
        radio2800->setGeometry(QRect(0, 60, 61, 17));
        radio2C00 = new QRadioButton(groupBox_2);
        radio2C00->setObjectName(QStringLiteral("radio2C00"));
        radio2C00->setGeometry(QRect(0, 80, 71, 17));
        groupBox_3 = new QGroupBox(PPU_viewer_dialog);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(10, 280, 461, 101));

        retranslateUi(PPU_viewer_dialog);

        QMetaObject::connectSlotsByName(PPU_viewer_dialog);
    } // setupUi

    void retranslateUi(QDialog *PPU_viewer_dialog)
    {
        PPU_viewer_dialog->setWindowTitle(QApplication::translate("PPU_viewer_dialog", "PPU viewer", 0));
        groupBox->setTitle(QApplication::translate("PPU_viewer_dialog", "Nametable viewer", 0));
        groupBox_2->setTitle(QApplication::translate("PPU_viewer_dialog", "Nametable", 0));
        radio2000->setText(QApplication::translate("PPU_viewer_dialog", "$2000", 0));
        radio2400->setText(QApplication::translate("PPU_viewer_dialog", "$2400", 0));
        radio2800->setText(QApplication::translate("PPU_viewer_dialog", "$2800", 0));
        radio2C00->setText(QApplication::translate("PPU_viewer_dialog", "$2C00", 0));
        groupBox_3->setTitle(QApplication::translate("PPU_viewer_dialog", "OAM Contents [N/A]", 0));
    } // retranslateUi

};

namespace Ui {
    class PPU_viewer_dialog: public Ui_PPU_viewer_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PPU_VIEWER_DIALOG_H
