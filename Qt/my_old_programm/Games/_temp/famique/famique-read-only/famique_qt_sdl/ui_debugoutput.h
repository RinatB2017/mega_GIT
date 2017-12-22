/********************************************************************************
** Form generated from reading UI file 'debugoutput.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEBUGOUTPUT_H
#define UI_DEBUGOUTPUT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPlainTextEdit>

QT_BEGIN_NAMESPACE

class Ui_DebugOutput
{
public:
    QPlainTextEdit *OutputTextField;

    void setupUi(QDialog *DebugOutput)
    {
        if (DebugOutput->objectName().isEmpty())
            DebugOutput->setObjectName(QStringLiteral("DebugOutput"));
        DebugOutput->resize(214, 358);
        OutputTextField = new QPlainTextEdit(DebugOutput);
        OutputTextField->setObjectName(QStringLiteral("OutputTextField"));
        OutputTextField->setGeometry(QRect(0, 0, 214, 358));
        QFont font;
        font.setFamily(QStringLiteral("Tahoma"));
        font.setPointSize(7);
        font.setBold(true);
        font.setWeight(75);
        font.setStyleStrategy(QFont::PreferAntialias);
        OutputTextField->setFont(font);

        retranslateUi(DebugOutput);

        QMetaObject::connectSlotsByName(DebugOutput);
    } // setupUi

    void retranslateUi(QDialog *DebugOutput)
    {
        DebugOutput->setWindowTitle(QApplication::translate("DebugOutput", "Output :-c", 0));
    } // retranslateUi

};

namespace Ui {
    class DebugOutput: public Ui_DebugOutput {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEBUGOUTPUT_H
