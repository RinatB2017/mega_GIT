/********************************************************************************
** Form generated from reading UI file 'debug.ui'
**
** Created: Sat 23. Jan 06:54:41 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUG_H
#define UI_DEBUG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QListView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_debugDialog
{
public:
    QGroupBox *groupBox;
    QListView *listView;
    QLabel *label_6;
    QGroupBox *groupBox_2;
    QPushButton *pushButton;
    QLineEdit *lineEdit;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QPushButton *pushButton_4;
    QGroupBox *groupBox_3;
    QLabel *label;
    QLabel *label_2;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLabel *label_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit_5;
    QLabel *label_4;
    QLabel *label_5;
    QLineEdit *lineEdit_6;
    QCheckBox *checkBox;
    QCheckBox *checkBox_2;
    QCheckBox *checkBox_3;
    QCheckBox *checkBox_4;
    QCheckBox *checkBox_5;
    QCheckBox *checkBox_6;
    QCheckBox *checkBox_7;

    void setupUi(QDialog *debugDialog)
    {
        if (debugDialog->objectName().isEmpty())
            debugDialog->setObjectName(QString::fromUtf8("debugDialog"));
        debugDialog->resize(400, 300);
        groupBox = new QGroupBox(debugDialog);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 140, 381, 151));
        listView = new QListView(groupBox);
        listView->setObjectName(QString::fromUtf8("listView"));
        listView->setGeometry(QRect(210, 30, 161, 121));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(210, 10, 61, 16));
        groupBox_2 = new QGroupBox(debugDialog);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(220, 0, 171, 141));
        pushButton = new QPushButton(groupBox_2);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(90, 110, 75, 23));
        lineEdit = new QLineEdit(groupBox_2);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(40, 110, 41, 20));
        pushButton_2 = new QPushButton(groupBox_2);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(10, 10, 41, 23));
        pushButton_3 = new QPushButton(groupBox_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(60, 10, 41, 23));
        pushButton_4 = new QPushButton(groupBox_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(110, 10, 41, 23));
        groupBox_3 = new QGroupBox(debugDialog);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(0, 0, 221, 111));
        label = new QLabel(groupBox_3);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 20, 16, 16));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(50, 20, 16, 16));
        lineEdit_2 = new QLineEdit(groupBox_3);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(20, 20, 21, 16));
        lineEdit_3 = new QLineEdit(groupBox_3);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setGeometry(QRect(60, 20, 21, 16));
        label_3 = new QLabel(groupBox_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 40, 16, 16));
        lineEdit_4 = new QLineEdit(groupBox_3);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setGeometry(QRect(20, 40, 21, 16));
        lineEdit_5 = new QLineEdit(groupBox_3);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setGeometry(QRect(60, 40, 21, 16));
        label_4 = new QLabel(groupBox_3);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(47, 40, 16, 16));
        label_5 = new QLabel(groupBox_3);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(7, 60, 16, 16));
        lineEdit_6 = new QLineEdit(groupBox_3);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setGeometry(QRect(20, 60, 21, 16));
        checkBox = new QCheckBox(groupBox_3);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(10, 90, 30, 17));
        checkBox_2 = new QCheckBox(groupBox_3);
        checkBox_2->setObjectName(QString::fromUtf8("checkBox_2"));
        checkBox_2->setGeometry(QRect(40, 90, 31, 17));
        checkBox_3 = new QCheckBox(groupBox_3);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setGeometry(QRect(70, 90, 31, 17));
        checkBox_4 = new QCheckBox(groupBox_3);
        checkBox_4->setObjectName(QString::fromUtf8("checkBox_4"));
        checkBox_4->setGeometry(QRect(100, 90, 31, 17));
        checkBox_5 = new QCheckBox(groupBox_3);
        checkBox_5->setObjectName(QString::fromUtf8("checkBox_5"));
        checkBox_5->setGeometry(QRect(130, 90, 31, 17));
        checkBox_6 = new QCheckBox(groupBox_3);
        checkBox_6->setObjectName(QString::fromUtf8("checkBox_6"));
        checkBox_6->setGeometry(QRect(160, 90, 31, 17));
        checkBox_7 = new QCheckBox(groupBox_3);
        checkBox_7->setObjectName(QString::fromUtf8("checkBox_7"));
        checkBox_7->setGeometry(QRect(190, 90, 31, 17));

        retranslateUi(debugDialog);

        QMetaObject::connectSlotsByName(debugDialog);
    } // setupUi

    void retranslateUi(QDialog *debugDialog)
    {
        debugDialog->setWindowTitle(QApplication::translate("debugDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("debugDialog", "Tracer", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("debugDialog", "CPU Tracer", 0, QApplication::UnicodeUTF8));
        groupBox_2->setTitle(QString());
        pushButton->setText(QApplication::translate("debugDialog", "Goto", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("debugDialog", "Run", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("debugDialog", "Step", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("debugDialog", "Halt", 0, QApplication::UnicodeUTF8));
        groupBox_3->setTitle(QApplication::translate("debugDialog", "Registers", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("debugDialog", "X", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("debugDialog", "A", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("debugDialog", "Y", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("debugDialog", "SP", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("debugDialog", "SR", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("debugDialog", "V", 0, QApplication::UnicodeUTF8));
        checkBox_2->setText(QApplication::translate("debugDialog", "B", 0, QApplication::UnicodeUTF8));
        checkBox_3->setText(QApplication::translate("debugDialog", "I", 0, QApplication::UnicodeUTF8));
        checkBox_4->setText(QApplication::translate("debugDialog", "U", 0, QApplication::UnicodeUTF8));
        checkBox_5->setText(QApplication::translate("debugDialog", "Z", 0, QApplication::UnicodeUTF8));
        checkBox_6->setText(QApplication::translate("debugDialog", "N", 0, QApplication::UnicodeUTF8));
        checkBox_7->setText(QApplication::translate("debugDialog", "D", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class debugDialog: public Ui_debugDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUG_H
