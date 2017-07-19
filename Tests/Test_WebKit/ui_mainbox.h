/********************************************************************************
** Form generated from reading UI file 'mainbox.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINBOX_H
#define UI_MAINBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainBox
{
public:
    QVBoxLayout *verticalLayout;
    QVBoxLayout *web_layout;

    void setupUi(QWidget *MainBox)
    {
        if (MainBox->objectName().isEmpty())
            MainBox->setObjectName(QStringLiteral("MainBox"));
        MainBox->resize(114, 16);
        verticalLayout = new QVBoxLayout(MainBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        web_layout = new QVBoxLayout();
        web_layout->setObjectName(QStringLiteral("web_layout"));

        verticalLayout->addLayout(web_layout);


        retranslateUi(MainBox);

        QMetaObject::connectSlotsByName(MainBox);
    } // setupUi

    void retranslateUi(QWidget *MainBox)
    {
        MainBox->setWindowTitle(QApplication::translate("MainBox", "Dialog", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainBox: public Ui_MainBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINBOX_H
