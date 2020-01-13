/********************************************************************************
** Form generated from reading UI file 'connectdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONNECTDIALOG_H
#define UI_CONNECTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include "bitratebox.h"

QT_BEGIN_NAMESPACE

class Ui_ConnectDialog
{
public:
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QGroupBox *selectPluginBox;
    QGridLayout *gridLayout;
    QComboBox *pluginListBox;
    QGroupBox *configurationBox;
    QGridLayout *gridLayout_4;
    QLabel *rawFilterLabel;
    QLineEdit *rawFilterEdit;
    QLabel *errorFilterLabel;
    QLineEdit *errorFilterEdit;
    QLabel *loopbackLabel;
    QComboBox *loopbackBox;
    QLabel *receiveOwnLabel;
    QComboBox *receiveOwnBox;
    QLabel *bitrateLabel;
    BitRateBox *bitrateBox;
    QLabel *canFdLabel;
    QComboBox *canFdBox;
    QLabel *dataBitrateLabel;
    BitRateBox *dataBitrateBox;
    QGroupBox *specifyInterfaceNameBox;
    QGridLayout *gridLayout_3;
    QComboBox *interfaceListBox;
    QCheckBox *useConfigurationBox;
    QGroupBox *interfacePropertiesBox;
    QVBoxLayout *verticalLayout;
    QCheckBox *isFlexibleDataRateCapable;
    QCheckBox *isVirtual;

    void setupUi(QDialog *ConnectDialog)
    {
        if (ConnectDialog->objectName().isEmpty())
            ConnectDialog->setObjectName(QString::fromUtf8("ConnectDialog"));
        ConnectDialog->resize(441, 341);
        gridLayout_2 = new QGridLayout(ConnectDialog);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(96, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        cancelButton = new QPushButton(ConnectDialog);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));
        cancelButton->setAutoDefault(false);

        horizontalLayout->addWidget(cancelButton);

        okButton = new QPushButton(ConnectDialog);
        okButton->setObjectName(QString::fromUtf8("okButton"));
        okButton->setAutoDefault(false);

        horizontalLayout->addWidget(okButton);


        gridLayout_2->addLayout(horizontalLayout, 5, 0, 1, 2);

        selectPluginBox = new QGroupBox(ConnectDialog);
        selectPluginBox->setObjectName(QString::fromUtf8("selectPluginBox"));
        gridLayout = new QGridLayout(selectPluginBox);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        pluginListBox = new QComboBox(selectPluginBox);
        pluginListBox->setObjectName(QString::fromUtf8("pluginListBox"));

        gridLayout->addWidget(pluginListBox, 0, 0, 1, 1);


        gridLayout_2->addWidget(selectPluginBox, 0, 0, 1, 1);

        configurationBox = new QGroupBox(ConnectDialog);
        configurationBox->setObjectName(QString::fromUtf8("configurationBox"));
        configurationBox->setEnabled(false);
        gridLayout_4 = new QGridLayout(configurationBox);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        rawFilterLabel = new QLabel(configurationBox);
        rawFilterLabel->setObjectName(QString::fromUtf8("rawFilterLabel"));

        gridLayout_4->addWidget(rawFilterLabel, 0, 0, 1, 1);

        rawFilterEdit = new QLineEdit(configurationBox);
        rawFilterEdit->setObjectName(QString::fromUtf8("rawFilterEdit"));
        rawFilterEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(rawFilterEdit, 0, 1, 1, 1);

        errorFilterLabel = new QLabel(configurationBox);
        errorFilterLabel->setObjectName(QString::fromUtf8("errorFilterLabel"));

        gridLayout_4->addWidget(errorFilterLabel, 1, 0, 1, 1);

        errorFilterEdit = new QLineEdit(configurationBox);
        errorFilterEdit->setObjectName(QString::fromUtf8("errorFilterEdit"));
        errorFilterEdit->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_4->addWidget(errorFilterEdit, 1, 1, 1, 1);

        loopbackLabel = new QLabel(configurationBox);
        loopbackLabel->setObjectName(QString::fromUtf8("loopbackLabel"));

        gridLayout_4->addWidget(loopbackLabel, 2, 0, 1, 1);

        loopbackBox = new QComboBox(configurationBox);
        loopbackBox->setObjectName(QString::fromUtf8("loopbackBox"));

        gridLayout_4->addWidget(loopbackBox, 2, 1, 1, 1);

        receiveOwnLabel = new QLabel(configurationBox);
        receiveOwnLabel->setObjectName(QString::fromUtf8("receiveOwnLabel"));

        gridLayout_4->addWidget(receiveOwnLabel, 3, 0, 1, 1);

        receiveOwnBox = new QComboBox(configurationBox);
        receiveOwnBox->setObjectName(QString::fromUtf8("receiveOwnBox"));

        gridLayout_4->addWidget(receiveOwnBox, 3, 1, 1, 1);

        bitrateLabel = new QLabel(configurationBox);
        bitrateLabel->setObjectName(QString::fromUtf8("bitrateLabel"));

        gridLayout_4->addWidget(bitrateLabel, 4, 0, 1, 1);

        bitrateBox = new BitRateBox(configurationBox);
        bitrateBox->setObjectName(QString::fromUtf8("bitrateBox"));

        gridLayout_4->addWidget(bitrateBox, 4, 1, 1, 1);

        canFdLabel = new QLabel(configurationBox);
        canFdLabel->setObjectName(QString::fromUtf8("canFdLabel"));

        gridLayout_4->addWidget(canFdLabel, 5, 0, 1, 1);

        canFdBox = new QComboBox(configurationBox);
        canFdBox->setObjectName(QString::fromUtf8("canFdBox"));

        gridLayout_4->addWidget(canFdBox, 5, 1, 1, 1);

        dataBitrateLabel = new QLabel(configurationBox);
        dataBitrateLabel->setObjectName(QString::fromUtf8("dataBitrateLabel"));

        gridLayout_4->addWidget(dataBitrateLabel, 6, 0, 1, 1);

        dataBitrateBox = new BitRateBox(configurationBox);
        dataBitrateBox->setObjectName(QString::fromUtf8("dataBitrateBox"));

        gridLayout_4->addWidget(dataBitrateBox, 6, 1, 1, 1);


        gridLayout_2->addWidget(configurationBox, 0, 1, 5, 1);

        specifyInterfaceNameBox = new QGroupBox(ConnectDialog);
        specifyInterfaceNameBox->setObjectName(QString::fromUtf8("specifyInterfaceNameBox"));
        gridLayout_3 = new QGridLayout(specifyInterfaceNameBox);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        interfaceListBox = new QComboBox(specifyInterfaceNameBox);
        interfaceListBox->setObjectName(QString::fromUtf8("interfaceListBox"));
        interfaceListBox->setEditable(true);

        gridLayout_3->addWidget(interfaceListBox, 0, 0, 1, 1);


        gridLayout_2->addWidget(specifyInterfaceNameBox, 1, 0, 1, 1);

        useConfigurationBox = new QCheckBox(ConnectDialog);
        useConfigurationBox->setObjectName(QString::fromUtf8("useConfigurationBox"));

        gridLayout_2->addWidget(useConfigurationBox, 4, 0, 1, 1);

        interfacePropertiesBox = new QGroupBox(ConnectDialog);
        interfacePropertiesBox->setObjectName(QString::fromUtf8("interfacePropertiesBox"));
        interfacePropertiesBox->setEnabled(false);
        verticalLayout = new QVBoxLayout(interfacePropertiesBox);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        isFlexibleDataRateCapable = new QCheckBox(interfacePropertiesBox);
        isFlexibleDataRateCapable->setObjectName(QString::fromUtf8("isFlexibleDataRateCapable"));

        verticalLayout->addWidget(isFlexibleDataRateCapable);

        isVirtual = new QCheckBox(interfacePropertiesBox);
        isVirtual->setObjectName(QString::fromUtf8("isVirtual"));

        verticalLayout->addWidget(isVirtual);


        gridLayout_2->addWidget(interfacePropertiesBox, 2, 0, 1, 1);


        retranslateUi(ConnectDialog);

        okButton->setDefault(true);


        QMetaObject::connectSlotsByName(ConnectDialog);
    } // setupUi

    void retranslateUi(QDialog *ConnectDialog)
    {
        ConnectDialog->setWindowTitle(QCoreApplication::translate("ConnectDialog", "Connect", nullptr));
        cancelButton->setText(QCoreApplication::translate("ConnectDialog", "Cancel", nullptr));
        okButton->setText(QCoreApplication::translate("ConnectDialog", "OK", nullptr));
        selectPluginBox->setTitle(QCoreApplication::translate("ConnectDialog", "Select CAN plugin", nullptr));
        configurationBox->setTitle(QCoreApplication::translate("ConnectDialog", "Specify Configuration", nullptr));
        rawFilterLabel->setText(QCoreApplication::translate("ConnectDialog", "RAW Filter", nullptr));
        errorFilterLabel->setText(QCoreApplication::translate("ConnectDialog", "Error Filter", nullptr));
        errorFilterEdit->setPlaceholderText(QCoreApplication::translate("ConnectDialog", "FrameError bits", nullptr));
        loopbackLabel->setText(QCoreApplication::translate("ConnectDialog", "Loopback", nullptr));
        receiveOwnLabel->setText(QCoreApplication::translate("ConnectDialog", "Receive Own", nullptr));
        bitrateLabel->setText(QCoreApplication::translate("ConnectDialog", "Bitrate", nullptr));
        canFdLabel->setText(QCoreApplication::translate("ConnectDialog", "CAN FD", nullptr));
        dataBitrateLabel->setText(QCoreApplication::translate("ConnectDialog", "Data Bitrate", nullptr));
        specifyInterfaceNameBox->setTitle(QCoreApplication::translate("ConnectDialog", "Specify CAN interface name", nullptr));
        useConfigurationBox->setText(QCoreApplication::translate("ConnectDialog", "Custom configuration", nullptr));
        interfacePropertiesBox->setTitle(QCoreApplication::translate("ConnectDialog", "CAN interface properties", nullptr));
        isFlexibleDataRateCapable->setText(QCoreApplication::translate("ConnectDialog", "Flexible Data Rate", nullptr));
        isVirtual->setText(QCoreApplication::translate("ConnectDialog", "Virtual", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ConnectDialog: public Ui_ConnectDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONNECTDIALOG_H
