/********************************************************************************
** Form generated from reading UI file 'device.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICE_H
#define UI_DEVICE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_DeviceDiscovery
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *list;
    QGroupBox *groupBox;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *power;
    QCheckBox *discoverable;
    QCheckBox *inquiryType;
    QHBoxLayout *horizontalLayout;
    QPushButton *scan;
    QPushButton *clear;
    QPushButton *quit;

    void setupUi(QDialog *DeviceDiscovery)
    {
        if (DeviceDiscovery->objectName().isEmpty())
            DeviceDiscovery->setObjectName(QStringLiteral("DeviceDiscovery"));
        DeviceDiscovery->resize(400, 411);
        verticalLayout = new QVBoxLayout(DeviceDiscovery);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        list = new QListWidget(DeviceDiscovery);
        list->setObjectName(QStringLiteral("list"));

        verticalLayout->addWidget(list);

        groupBox = new QGroupBox(DeviceDiscovery);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        horizontalLayout_2 = new QHBoxLayout(groupBox);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        power = new QCheckBox(groupBox);
        power->setObjectName(QStringLiteral("power"));
        power->setChecked(true);

        horizontalLayout_2->addWidget(power);

        discoverable = new QCheckBox(groupBox);
        discoverable->setObjectName(QStringLiteral("discoverable"));
        discoverable->setChecked(true);

        horizontalLayout_2->addWidget(discoverable);


        verticalLayout->addWidget(groupBox);

        inquiryType = new QCheckBox(DeviceDiscovery);
        inquiryType->setObjectName(QStringLiteral("inquiryType"));
        inquiryType->setChecked(true);

        verticalLayout->addWidget(inquiryType);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        scan = new QPushButton(DeviceDiscovery);
        scan->setObjectName(QStringLiteral("scan"));

        horizontalLayout->addWidget(scan);

        clear = new QPushButton(DeviceDiscovery);
        clear->setObjectName(QStringLiteral("clear"));

        horizontalLayout->addWidget(clear);

        quit = new QPushButton(DeviceDiscovery);
        quit->setObjectName(QStringLiteral("quit"));

        horizontalLayout->addWidget(quit);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(DeviceDiscovery);
        QObject::connect(quit, SIGNAL(clicked()), DeviceDiscovery, SLOT(accept()));
        QObject::connect(clear, SIGNAL(clicked()), list, SLOT(clear()));

        QMetaObject::connectSlotsByName(DeviceDiscovery);
    } // setupUi

    void retranslateUi(QDialog *DeviceDiscovery)
    {
        DeviceDiscovery->setWindowTitle(QApplication::translate("DeviceDiscovery", "Bluetooth Scanner", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("DeviceDiscovery", "Local Device", Q_NULLPTR));
        power->setText(QApplication::translate("DeviceDiscovery", "Bluetooth Powered On", Q_NULLPTR));
        discoverable->setText(QApplication::translate("DeviceDiscovery", "Discoverable", Q_NULLPTR));
        inquiryType->setText(QApplication::translate("DeviceDiscovery", "General Unlimited Inquiry", Q_NULLPTR));
        scan->setText(QApplication::translate("DeviceDiscovery", "Scan", Q_NULLPTR));
        clear->setText(QApplication::translate("DeviceDiscovery", "Clear", Q_NULLPTR));
        quit->setText(QApplication::translate("DeviceDiscovery", "Quit", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class DeviceDiscovery: public Ui_DeviceDiscovery {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICE_H
