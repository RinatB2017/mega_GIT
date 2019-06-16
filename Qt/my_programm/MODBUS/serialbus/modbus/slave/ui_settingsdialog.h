/********************************************************************************
** Form generated from reading UI file 'settingsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGSDIALOG_H
#define UI_SETTINGSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>

QT_BEGIN_NAMESPACE

class Ui_SettingsDialog
{
public:
    QGridLayout *gridLayout;
    QSpacerItem *verticalSpacer;
    QPushButton *applyButton;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_2;
    QLabel *label_2;
    QComboBox *parityCombo;
    QLabel *label_3;
    QComboBox *baudCombo;
    QLabel *label_4;
    QComboBox *dataBitsCombo;
    QLabel *label_5;
    QComboBox *stopBitsCombo;

    void setupUi(QDialog *SettingsDialog)
    {
        if (SettingsDialog->objectName().isEmpty())
            SettingsDialog->setObjectName(QString::fromUtf8("SettingsDialog"));
        SettingsDialog->resize(239, 256);
        gridLayout = new QGridLayout(SettingsDialog);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        verticalSpacer = new QSpacerItem(20, 43, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout->addItem(verticalSpacer, 1, 1, 1, 1);

        applyButton = new QPushButton(SettingsDialog);
        applyButton->setObjectName(QString::fromUtf8("applyButton"));

        gridLayout->addWidget(applyButton, 2, 1, 1, 1);

        groupBox = new QGroupBox(SettingsDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        gridLayout_2 = new QGridLayout(groupBox);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        gridLayout_2->addWidget(label_2, 0, 0, 1, 1);

        parityCombo = new QComboBox(groupBox);
        parityCombo->addItem(QString());
        parityCombo->addItem(QString());
        parityCombo->addItem(QString());
        parityCombo->addItem(QString());
        parityCombo->addItem(QString());
        parityCombo->setObjectName(QString::fromUtf8("parityCombo"));

        gridLayout_2->addWidget(parityCombo, 0, 1, 1, 1);

        label_3 = new QLabel(groupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 1, 0, 1, 1);

        baudCombo = new QComboBox(groupBox);
        baudCombo->addItem(QString());
        baudCombo->addItem(QString());
        baudCombo->addItem(QString());
        baudCombo->addItem(QString());
        baudCombo->addItem(QString());
        baudCombo->addItem(QString());
        baudCombo->addItem(QString());
        baudCombo->addItem(QString());
        baudCombo->setObjectName(QString::fromUtf8("baudCombo"));

        gridLayout_2->addWidget(baudCombo, 1, 1, 1, 1);

        label_4 = new QLabel(groupBox);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_2->addWidget(label_4, 2, 0, 1, 1);

        dataBitsCombo = new QComboBox(groupBox);
        dataBitsCombo->addItem(QString());
        dataBitsCombo->addItem(QString());
        dataBitsCombo->addItem(QString());
        dataBitsCombo->addItem(QString());
        dataBitsCombo->setObjectName(QString::fromUtf8("dataBitsCombo"));

        gridLayout_2->addWidget(dataBitsCombo, 2, 1, 1, 1);

        label_5 = new QLabel(groupBox);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_2->addWidget(label_5, 3, 0, 1, 1);

        stopBitsCombo = new QComboBox(groupBox);
        stopBitsCombo->addItem(QString());
        stopBitsCombo->addItem(QString());
        stopBitsCombo->addItem(QString());
        stopBitsCombo->setObjectName(QString::fromUtf8("stopBitsCombo"));

        gridLayout_2->addWidget(stopBitsCombo, 3, 1, 1, 1);


        gridLayout->addWidget(groupBox, 0, 0, 1, 2);


        retranslateUi(SettingsDialog);

        QMetaObject::connectSlotsByName(SettingsDialog);
    } // setupUi

    void retranslateUi(QDialog *SettingsDialog)
    {
        SettingsDialog->setWindowTitle(QApplication::translate("SettingsDialog", "Modbus Settings", nullptr));
        applyButton->setText(QApplication::translate("SettingsDialog", "Apply", nullptr));
        groupBox->setTitle(QApplication::translate("SettingsDialog", "Serial Parameters", nullptr));
        label_2->setText(QApplication::translate("SettingsDialog", "Parity:", nullptr));
        parityCombo->setItemText(0, QApplication::translate("SettingsDialog", "No", nullptr));
        parityCombo->setItemText(1, QApplication::translate("SettingsDialog", "Even", nullptr));
        parityCombo->setItemText(2, QApplication::translate("SettingsDialog", "Odd", nullptr));
        parityCombo->setItemText(3, QApplication::translate("SettingsDialog", "Space", nullptr));
        parityCombo->setItemText(4, QApplication::translate("SettingsDialog", "Mark", nullptr));

        label_3->setText(QApplication::translate("SettingsDialog", "Baud Rate:", nullptr));
        baudCombo->setItemText(0, QApplication::translate("SettingsDialog", "1200", nullptr));
        baudCombo->setItemText(1, QApplication::translate("SettingsDialog", "2400", nullptr));
        baudCombo->setItemText(2, QApplication::translate("SettingsDialog", "4800", nullptr));
        baudCombo->setItemText(3, QApplication::translate("SettingsDialog", "9600", nullptr));
        baudCombo->setItemText(4, QApplication::translate("SettingsDialog", "19200", nullptr));
        baudCombo->setItemText(5, QApplication::translate("SettingsDialog", "38400", nullptr));
        baudCombo->setItemText(6, QApplication::translate("SettingsDialog", "57600", nullptr));
        baudCombo->setItemText(7, QApplication::translate("SettingsDialog", "115200", nullptr));

        label_4->setText(QApplication::translate("SettingsDialog", "Data Bits:", nullptr));
        dataBitsCombo->setItemText(0, QApplication::translate("SettingsDialog", "5", nullptr));
        dataBitsCombo->setItemText(1, QApplication::translate("SettingsDialog", "6", nullptr));
        dataBitsCombo->setItemText(2, QApplication::translate("SettingsDialog", "7", nullptr));
        dataBitsCombo->setItemText(3, QApplication::translate("SettingsDialog", "8", nullptr));

        label_5->setText(QApplication::translate("SettingsDialog", "Stop Bits:", nullptr));
        stopBitsCombo->setItemText(0, QApplication::translate("SettingsDialog", "1", nullptr));
        stopBitsCombo->setItemText(1, QApplication::translate("SettingsDialog", "3", nullptr));
        stopBitsCombo->setItemText(2, QApplication::translate("SettingsDialog", "2", nullptr));

    } // retranslateUi

};

namespace Ui {
    class SettingsDialog: public Ui_SettingsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGSDIALOG_H
