/********************************************************************************
** Form generated from reading UI file 'remoteselector.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_REMOTESELECTOR_H
#define UI_REMOTESELECTOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QListWidget>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>

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
            RemoteSelector->setObjectName(QString::fromUtf8("RemoteSelector"));
        RemoteSelector->resize(400, 300);
        verticalLayout = new QVBoxLayout(RemoteSelector);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        status = new QLabel(RemoteSelector);
        status->setObjectName(QString::fromUtf8("status"));

        verticalLayout->addWidget(status);

        remoteDevices = new QListWidget(RemoteSelector);
        remoteDevices->setObjectName(QString::fromUtf8("remoteDevices"));

        verticalLayout->addWidget(remoteDevices);

        cancelButton = new QPushButton(RemoteSelector);
        cancelButton->setObjectName(QString::fromUtf8("cancelButton"));

        verticalLayout->addWidget(cancelButton);


        retranslateUi(RemoteSelector);

        QMetaObject::connectSlotsByName(RemoteSelector);
    } // setupUi

    void retranslateUi(QDialog *RemoteSelector)
    {
        RemoteSelector->setWindowTitle(QApplication::translate("RemoteSelector", "Available chat services", 0, QApplication::UnicodeUTF8));
        status->setText(QApplication::translate("RemoteSelector", "Scanning...", 0, QApplication::UnicodeUTF8));
        cancelButton->setText(QApplication::translate("RemoteSelector", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RemoteSelector: public Ui_RemoteSelector {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_REMOTESELECTOR_H
