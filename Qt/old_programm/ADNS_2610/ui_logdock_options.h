/********************************************************************************
** Form generated from reading UI file 'logdock_options.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOGDOCK_OPTIONS_H
#define UI_LOGDOCK_OPTIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_LogDock_options
{
public:
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_2;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_4;
    QCheckBox *cb_readonly;
    QCheckBox *checkBox_10;
    QCheckBox *checkBox_11;
    QCheckBox *checkBox_12;
    QCheckBox *checkBox_13;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *cb_CodecForCStrings;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *verticalLayout_6;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QCheckBox *cb_EMERG;
    QCheckBox *cb_ALERT;
    QCheckBox *cb_CRIT;
    QCheckBox *cb_ERR;
    QCheckBox *cb_WARN;
    QCheckBox *cb_NOTICE;
    QCheckBox *cb_INFO;
    QCheckBox *cb_DEBUG;
    QSpacerItem *verticalSpacer_3;
    QVBoxLayout *verticalLayout_3;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;
    QSpacerItem *verticalSpacer_2;

    void setupUi(QDialog *LogDock_options)
    {
        if (LogDock_options->objectName().isEmpty())
            LogDock_options->setObjectName(QStringLiteral("LogDock_options"));
        LogDock_options->resize(591, 356);
        horizontalLayout = new QHBoxLayout(LogDock_options);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        groupBox_2 = new QGroupBox(LogDock_options);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_4 = new QVBoxLayout(groupBox_2);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        cb_readonly = new QCheckBox(groupBox_2);
        cb_readonly->setObjectName(QStringLiteral("cb_readonly"));

        verticalLayout_4->addWidget(cb_readonly);

        checkBox_10 = new QCheckBox(groupBox_2);
        checkBox_10->setObjectName(QStringLiteral("checkBox_10"));

        verticalLayout_4->addWidget(checkBox_10);

        checkBox_11 = new QCheckBox(groupBox_2);
        checkBox_11->setObjectName(QStringLiteral("checkBox_11"));

        verticalLayout_4->addWidget(checkBox_11);

        checkBox_12 = new QCheckBox(groupBox_2);
        checkBox_12->setObjectName(QStringLiteral("checkBox_12"));

        verticalLayout_4->addWidget(checkBox_12);

        checkBox_13 = new QCheckBox(groupBox_2);
        checkBox_13->setObjectName(QStringLiteral("checkBox_13"));

        verticalLayout_4->addWidget(checkBox_13);


        verticalLayout_2->addWidget(groupBox_2);

        groupBox_3 = new QGroupBox(LogDock_options);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(groupBox_3);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        cb_CodecForCStrings = new QComboBox(groupBox_3);
        cb_CodecForCStrings->setObjectName(QStringLiteral("cb_CodecForCStrings"));

        horizontalLayout_2->addWidget(cb_CodecForCStrings);


        verticalLayout_5->addLayout(horizontalLayout_2);


        verticalLayout_2->addWidget(groupBox_3);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);


        horizontalLayout->addLayout(verticalLayout_2);

        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        groupBox = new QGroupBox(LogDock_options);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        cb_EMERG = new QCheckBox(groupBox);
        cb_EMERG->setObjectName(QStringLiteral("cb_EMERG"));

        verticalLayout->addWidget(cb_EMERG);

        cb_ALERT = new QCheckBox(groupBox);
        cb_ALERT->setObjectName(QStringLiteral("cb_ALERT"));

        verticalLayout->addWidget(cb_ALERT);

        cb_CRIT = new QCheckBox(groupBox);
        cb_CRIT->setObjectName(QStringLiteral("cb_CRIT"));

        verticalLayout->addWidget(cb_CRIT);

        cb_ERR = new QCheckBox(groupBox);
        cb_ERR->setObjectName(QStringLiteral("cb_ERR"));

        verticalLayout->addWidget(cb_ERR);

        cb_WARN = new QCheckBox(groupBox);
        cb_WARN->setObjectName(QStringLiteral("cb_WARN"));

        verticalLayout->addWidget(cb_WARN);

        cb_NOTICE = new QCheckBox(groupBox);
        cb_NOTICE->setObjectName(QStringLiteral("cb_NOTICE"));

        verticalLayout->addWidget(cb_NOTICE);

        cb_INFO = new QCheckBox(groupBox);
        cb_INFO->setObjectName(QStringLiteral("cb_INFO"));

        verticalLayout->addWidget(cb_INFO);

        cb_DEBUG = new QCheckBox(groupBox);
        cb_DEBUG->setObjectName(QStringLiteral("cb_DEBUG"));

        verticalLayout->addWidget(cb_DEBUG);


        verticalLayout_6->addWidget(groupBox);

        verticalSpacer_3 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_3);


        horizontalLayout->addLayout(verticalLayout_6);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        btn_ok = new QPushButton(LogDock_options);
        btn_ok->setObjectName(QStringLiteral("btn_ok"));

        verticalLayout_3->addWidget(btn_ok);

        btn_cancel = new QPushButton(LogDock_options);
        btn_cancel->setObjectName(QStringLiteral("btn_cancel"));

        verticalLayout_3->addWidget(btn_cancel);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_2);


        horizontalLayout->addLayout(verticalLayout_3);


        retranslateUi(LogDock_options);

        QMetaObject::connectSlotsByName(LogDock_options);
    } // setupUi

    void retranslateUi(QDialog *LogDock_options)
    {
        groupBox_2->setTitle(QApplication::translate("LogDock_options", "TextEdit", Q_NULLPTR));
        cb_readonly->setText(QApplication::translate("LogDock_options", "\321\202\320\276\320\273\321\214\320\272\320\276 \320\264\320\273\321\217 \321\207\321\202\320\265\320\275\320\270\321\217", Q_NULLPTR));
        checkBox_10->setText(QApplication::translate("LogDock_options", "\321\206\320\262\320\265\321\202\320\275\321\213\320\265 \320\273\320\276\320\263\320\270", Q_NULLPTR));
        checkBox_11->setText(QApplication::translate("LogDock_options", "\320\275\320\265 \320\264\320\276\320\261\320\260\320\262\320\273\321\217\321\202\321\214 CRLF", Q_NULLPTR));
        checkBox_12->setText(QApplication::translate("LogDock_options", "\320\264\320\276\320\261\320\260\320\262\320\273\321\217\321\202\321\214 \320\264\320\260\321\202\321\203 \320\270 \320\262\321\200\320\265\320\274\321\217", Q_NULLPTR));
        checkBox_13->setText(QApplication::translate("LogDock_options", "\320\276\321\202\320\276\320\261\321\200\320\260\320\266\320\260\321\202\321\214 \320\276\321\210\320\270\320\261\320\272\320\270, \320\272\320\260\320\272 MsgBox", Q_NULLPTR));
        groupBox_3->setTitle(QApplication::translate("LogDock_options", "Codecs", Q_NULLPTR));
        label_2->setText(QApplication::translate("LogDock_options", "CodecForCStrings", Q_NULLPTR));
        groupBox->setTitle(QApplication::translate("LogDock_options", "\320\236\321\202\320\276\320\261\321\200\320\260\320\266\320\260\321\202\321\214 \320\273\320\276\320\263\320\270:", Q_NULLPTR));
        cb_EMERG->setText(QApplication::translate("LogDock_options", "EMERG", Q_NULLPTR));
        cb_ALERT->setText(QApplication::translate("LogDock_options", "ALERT", Q_NULLPTR));
        cb_CRIT->setText(QApplication::translate("LogDock_options", "CRIT", Q_NULLPTR));
        cb_ERR->setText(QApplication::translate("LogDock_options", "ERR", Q_NULLPTR));
        cb_WARN->setText(QApplication::translate("LogDock_options", "WARN", Q_NULLPTR));
        cb_NOTICE->setText(QApplication::translate("LogDock_options", "NOTICE", Q_NULLPTR));
        cb_INFO->setText(QApplication::translate("LogDock_options", "INFO", Q_NULLPTR));
        cb_DEBUG->setText(QApplication::translate("LogDock_options", "DEBUG", Q_NULLPTR));
        btn_ok->setText(QApplication::translate("LogDock_options", "OK", Q_NULLPTR));
        btn_cancel->setText(QApplication::translate("LogDock_options", "CANCEL", Q_NULLPTR));
        Q_UNUSED(LogDock_options);
    } // retranslateUi

};

namespace Ui {
    class LogDock_options: public Ui_LogDock_options {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOGDOCK_OPTIONS_H
