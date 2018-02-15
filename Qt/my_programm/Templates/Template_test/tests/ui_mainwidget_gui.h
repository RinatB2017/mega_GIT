/********************************************************************************
** Form generated from reading UI file 'mainwidget_gui.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWIDGET_GUI_H
#define UI_MAINWIDGET_GUI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWidget_GUI
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QPushButton *btn_0;
    QLineEdit *le_0;
    QHBoxLayout *horizontalLayout_2;

    void setupUi(QWidget *MainWidget_GUI)
    {
        if (MainWidget_GUI->objectName().isEmpty())
            MainWidget_GUI->setObjectName(QStringLiteral("MainWidget_GUI"));
        MainWidget_GUI->resize(491, 50);
        verticalLayout = new QVBoxLayout(MainWidget_GUI);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        btn_0 = new QPushButton(MainWidget_GUI);
        btn_0->setObjectName(QStringLiteral("btn_0"));

        horizontalLayout_3->addWidget(btn_0);

        le_0 = new QLineEdit(MainWidget_GUI);
        le_0->setObjectName(QStringLiteral("le_0"));

        horizontalLayout_3->addWidget(le_0);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));

        horizontalLayout_3->addLayout(horizontalLayout_2);


        verticalLayout->addLayout(horizontalLayout_3);


        retranslateUi(MainWidget_GUI);

        QMetaObject::connectSlotsByName(MainWidget_GUI);
    } // setupUi

    void retranslateUi(QWidget *MainWidget_GUI)
    {
        MainWidget_GUI->setWindowTitle(QApplication::translate("MainWidget_GUI", "Dialog", nullptr));
        btn_0->setText(QApplication::translate("MainWidget_GUI", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWidget_GUI: public Ui_MainWidget_GUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWIDGET_GUI_H
