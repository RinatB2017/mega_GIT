/********************************************************************************
** Form generated from reading UI file 'remoteselector.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOTESELECTOR_H
#define UI_REMOTESELECTOR_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_RemoteSelector
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *status;
    QListWidget *remoteDevices;
    QPushButton *cancelButton;

    void setupUi(QDialog *RemoteSelector)
    {
        if (RemoteSelector->objectName().isEmpty())
            RemoteSelector->setObjectName(QStringLiteral("RemoteSelector"));
        RemoteSelector->resize(400, 300);
        verticalLayout = new QVBoxLayout(RemoteSelector);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        status = new QLabel(RemoteSelector);
        status->setObjectName(QStringLiteral("status"));

        verticalLayout->addWidget(status);

        remoteDevices = new QListWidget(RemoteSelector);
        remoteDevices->setObjectName(QStringLiteral("remoteDevices"));

        verticalLayout->addWidget(remoteDevices);

        cancelButton = new QPushButton(RemoteSelector);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        verticalLayout->addWidget(cancelButton);


        retranslateUi(RemoteSelector);

        QMetaObject::connectSlotsByName(RemoteSelector);
    } // setupUi

    void retranslateUi(QDialog *RemoteSelector)
    {
        RemoteSelector->setWindowTitle(QApplication::translate("RemoteSelector", "Available chat services", nullptr));
        status->setText(QApplication::translate("RemoteSelector", "Scanning...", nullptr));
        cancelButton->setText(QApplication::translate("RemoteSelector", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class RemoteSelector: public Ui_RemoteSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOTESELECTOR_H
