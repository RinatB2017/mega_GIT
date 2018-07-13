/********************************************************************************
** Form generated from reading UI file 'service.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERVICE_H
#define UI_SERVICE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_ServiceDiscovery
{
public:
    QVBoxLayout *verticalLayout;
    QListWidget *list;
    QLabel *status;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *close;

    void setupUi(QDialog *ServiceDiscovery)
    {
        if (ServiceDiscovery->objectName().isEmpty())
            ServiceDiscovery->setObjectName(QStringLiteral("ServiceDiscovery"));
        ServiceDiscovery->resize(400, 300);
        verticalLayout = new QVBoxLayout(ServiceDiscovery);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        list = new QListWidget(ServiceDiscovery);
        list->setObjectName(QStringLiteral("list"));

        verticalLayout->addWidget(list);

        status = new QLabel(ServiceDiscovery);
        status->setObjectName(QStringLiteral("status"));

        verticalLayout->addWidget(status);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        close = new QPushButton(ServiceDiscovery);
        close->setObjectName(QStringLiteral("close"));

        horizontalLayout->addWidget(close);


        verticalLayout->addLayout(horizontalLayout);

        QWidget::setTabOrder(list, close);

        retranslateUi(ServiceDiscovery);
        QObject::connect(close, SIGNAL(clicked()), ServiceDiscovery, SLOT(accept()));

        QMetaObject::connectSlotsByName(ServiceDiscovery);
    } // setupUi

    void retranslateUi(QDialog *ServiceDiscovery)
    {
        ServiceDiscovery->setWindowTitle(QApplication::translate("ServiceDiscovery", "Available Services", Q_NULLPTR));
        status->setText(QApplication::translate("ServiceDiscovery", "Querying...", Q_NULLPTR));
        close->setText(QApplication::translate("ServiceDiscovery", "Close", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ServiceDiscovery: public Ui_ServiceDiscovery {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERVICE_H
