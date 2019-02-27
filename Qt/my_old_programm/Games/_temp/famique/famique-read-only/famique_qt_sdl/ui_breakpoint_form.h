/********************************************************************************
** Form generated from reading UI file 'breakpoint_form.ui'
**
** Created: Tue 2. Feb 05:12:23 2010
**      by: Qt User Interface Compiler version 4.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BREAKPOINT_FORM_H
#define UI_BREAKPOINT_FORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_bp_dialog
{
public:
    QCheckBox *checkBox;
    QPushButton *pushButton;
    QPushButton *pushButton_2;

    void setupUi(QGroupBox *bp_dialog)
    {
        if (bp_dialog->objectName().isEmpty())
            bp_dialog->setObjectName(QString::fromUtf8("bp_dialog"));
        bp_dialog->resize(240, 320);
        checkBox = new QCheckBox(bp_dialog);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(20, 290, 70, 17));
        pushButton = new QPushButton(bp_dialog);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(100, 290, 41, 23));
        pushButton_2 = new QPushButton(bp_dialog);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(149, 290, 41, 23));

        retranslateUi(bp_dialog);

        QMetaObject::connectSlotsByName(bp_dialog);
    } // setupUi

    void retranslateUi(QGroupBox *bp_dialog)
    {
        bp_dialog->setWindowTitle(QApplication::translate("bp_dialog", "GroupBox", 0, QApplication::UnicodeUTF8));
        bp_dialog->setTitle(QApplication::translate("bp_dialog", "GroupBox", 0, QApplication::UnicodeUTF8));
        checkBox->setText(QApplication::translate("bp_dialog", "Enabled", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("bp_dialog", "OK", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("bp_dialog", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class bp_dialog: public Ui_bp_dialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BREAKPOINT_FORM_H
