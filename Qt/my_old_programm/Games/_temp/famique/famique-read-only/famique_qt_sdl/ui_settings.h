/********************************************************************************
** Form generated from reading UI file 'settings.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETTINGS_H
#define UI_SETTINGS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Settings
{
public:
    QTabWidget *tabWidget;
    QWidget *tab;
    QGroupBox *groupBox_2;
    QCheckBox *checkBox;
    QGroupBox *groupBox_3;
    QCheckBox *checkBox_2;
    QCheckBox *enable_ppppu;
    QWidget *tab_2;
    QGroupBox *groupBox;
    QLabel *keymessage;
    QLabel *labelA;
    QLabel *labelB;
    QLabel *labelStart;
    QLabel *labelSelect;
    QLabel *labelUp;
    QLabel *labelDown;
    QLabel *labelLeft;
    QLabel *labelRight;
    QPushButton *redefine_keys_button;

    void setupUi(QDialog *Settings)
    {
        if (Settings->objectName().isEmpty())
            Settings->setObjectName(QStringLiteral("Settings"));
        Settings->resize(582, 342);
        tabWidget = new QTabWidget(Settings);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setGeometry(QRect(0, 0, 581, 341));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        groupBox_2 = new QGroupBox(tab);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        groupBox_2->setGeometry(QRect(0, 0, 131, 241));
        checkBox = new QCheckBox(groupBox_2);
        checkBox->setObjectName(QStringLiteral("checkBox"));
        checkBox->setGeometry(QRect(10, 30, 111, 20));
        groupBox_3 = new QGroupBox(tab);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        groupBox_3->setGeometry(QRect(150, 0, 271, 241));
        checkBox_2 = new QCheckBox(groupBox_3);
        checkBox_2->setObjectName(QStringLiteral("checkBox_2"));
        checkBox_2->setGeometry(QRect(10, 30, 251, 20));
        enable_ppppu = new QCheckBox(groupBox_3);
        enable_ppppu->setObjectName(QStringLiteral("enable_ppppu"));
        enable_ppppu->setGeometry(QRect(10, 80, 251, 20));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 0, 241, 231));
        keymessage = new QLabel(groupBox);
        keymessage->setObjectName(QStringLiteral("keymessage"));
        keymessage->setGeometry(QRect(10, 199, 221, 31));
        QFont font;
        font.setPointSize(15);
        font.setBold(true);
        font.setWeight(75);
        keymessage->setFont(font);
        labelA = new QLabel(groupBox);
        labelA->setObjectName(QStringLiteral("labelA"));
        labelA->setGeometry(QRect(10, 20, 91, 16));
        labelB = new QLabel(groupBox);
        labelB->setObjectName(QStringLiteral("labelB"));
        labelB->setGeometry(QRect(10, 40, 71, 16));
        labelStart = new QLabel(groupBox);
        labelStart->setObjectName(QStringLiteral("labelStart"));
        labelStart->setGeometry(QRect(10, 60, 81, 16));
        labelSelect = new QLabel(groupBox);
        labelSelect->setObjectName(QStringLiteral("labelSelect"));
        labelSelect->setGeometry(QRect(10, 80, 141, 16));
        labelUp = new QLabel(groupBox);
        labelUp->setObjectName(QStringLiteral("labelUp"));
        labelUp->setGeometry(QRect(10, 100, 81, 16));
        labelDown = new QLabel(groupBox);
        labelDown->setObjectName(QStringLiteral("labelDown"));
        labelDown->setGeometry(QRect(10, 120, 91, 16));
        labelLeft = new QLabel(groupBox);
        labelLeft->setObjectName(QStringLiteral("labelLeft"));
        labelLeft->setGeometry(QRect(10, 140, 81, 16));
        labelRight = new QLabel(groupBox);
        labelRight->setObjectName(QStringLiteral("labelRight"));
        labelRight->setGeometry(QRect(10, 160, 91, 16));
        redefine_keys_button = new QPushButton(tab_2);
        redefine_keys_button->setObjectName(QStringLiteral("redefine_keys_button"));
        redefine_keys_button->setGeometry(QRect(100, 250, 101, 23));
        tabWidget->addTab(tab_2, QString());

        retranslateUi(Settings);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(Settings);
    } // setupUi

    void retranslateUi(QDialog *Settings)
    {
        Settings->setWindowTitle(QString());
        groupBox_2->setTitle(QApplication::translate("Settings", "General", 0));
        checkBox->setText(QApplication::translate("Settings", "Enable Sound", 0));
        groupBox_3->setTitle(QApplication::translate("Settings", "Emulation", 0));
        checkBox_2->setText(QApplication::translate("Settings", "Enable undocumented instructions", 0));
#ifndef QT_NO_WHATSTHIS
        enable_ppppu->setWhatsThis(QApplication::translate("Settings", "Enables Per-pixel based PPU emulation, which may or may not lead to better accuracy/compability.\n"
"\n"
"If a game is failing, try switching PPU emulation method.", 0));
#endif // QT_NO_WHATSTHIS
        enable_ppppu->setText(QApplication::translate("Settings", "Enable Per-pixel PPU emulation", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("Settings", "General", 0));
        groupBox->setTitle(QApplication::translate("Settings", "Controller configuration [Pad 1]", 0));
        keymessage->setText(QString());
        labelA->setText(QApplication::translate("Settings", "A: ", 0));
        labelB->setText(QApplication::translate("Settings", "B:", 0));
        labelStart->setText(QApplication::translate("Settings", "Start:", 0));
        labelSelect->setText(QApplication::translate("Settings", "Select:", 0));
        labelUp->setText(QApplication::translate("Settings", "Up:", 0));
        labelDown->setText(QApplication::translate("Settings", "Down:", 0));
        labelLeft->setText(QApplication::translate("Settings", "Left:", 0));
        labelRight->setText(QApplication::translate("Settings", "Right:", 0));
        redefine_keys_button->setText(QApplication::translate("Settings", "Redfine keys", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("Settings", "Controller", 0));
    } // retranslateUi

};

namespace Ui {
    class Settings: public Ui_Settings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETTINGS_H
