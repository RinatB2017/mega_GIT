/********************************************************************************
** Form generated from reading UI file 'mainbox.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINBOX_H
#define UI_MAINBOX_H

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

class Ui_MainBox
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QPushButton *btn_kpat;
    QPushButton *btn_kmines;
    QLineEdit *le_programm;
    QPushButton *btn_find_programm;
    QVBoxLayout *camera_layout;

    void setupUi(QWidget *MainBox)
    {
        if (MainBox->objectName().isEmpty())
            MainBox->setObjectName(QStringLiteral("MainBox"));
        MainBox->resize(416, 57);
        verticalLayout = new QVBoxLayout(MainBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btn_kpat = new QPushButton(MainBox);
        btn_kpat->setObjectName(QStringLiteral("btn_kpat"));

        horizontalLayout->addWidget(btn_kpat);

        btn_kmines = new QPushButton(MainBox);
        btn_kmines->setObjectName(QStringLiteral("btn_kmines"));

        horizontalLayout->addWidget(btn_kmines);

        le_programm = new QLineEdit(MainBox);
        le_programm->setObjectName(QStringLiteral("le_programm"));

        horizontalLayout->addWidget(le_programm);

        btn_find_programm = new QPushButton(MainBox);
        btn_find_programm->setObjectName(QStringLiteral("btn_find_programm"));

        horizontalLayout->addWidget(btn_find_programm);


        verticalLayout->addLayout(horizontalLayout);

        camera_layout = new QVBoxLayout();
        camera_layout->setObjectName(QStringLiteral("camera_layout"));

        verticalLayout->addLayout(camera_layout);


        retranslateUi(MainBox);

        QMetaObject::connectSlotsByName(MainBox);
    } // setupUi

    void retranslateUi(QWidget *MainBox)
    {
        MainBox->setWindowTitle(QApplication::translate("MainBox", "Dialog", nullptr));
        btn_kpat->setText(QApplication::translate("MainBox", "KPat", nullptr));
        btn_kmines->setText(QApplication::translate("MainBox", "KMines", nullptr));
        btn_find_programm->setText(QApplication::translate("MainBox", "\320\235\320\260\320\271\321\202\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainBox: public Ui_MainBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINBOX_H
