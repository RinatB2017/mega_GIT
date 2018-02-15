/********************************************************************************
** Form generated from reading UI file 'log_options.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOG_OPTIONS_H
#define UI_LOG_OPTIONS_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Log_options
{
public:
    QVBoxLayout *verticalLayout_4;
    QFrame *frame;
    QVBoxLayout *verticalLayout;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout_2;
    QCheckBox *cb_readOnly;
    QCheckBox *cb_acceptRichText;
    QCheckBox *cb_NoCRLF;
    QCheckBox *cb_add_DateTime;
    QCheckBox *cb_Color;
    QCheckBox *cb_ErrorAsMessage;
    QCheckBox *cb_Text_Is_Windows;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_3;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QComboBox *cb_CodecForCStrings;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Log_options)
    {
        if (Log_options->objectName().isEmpty())
            Log_options->setObjectName(QStringLiteral("Log_options"));
        Log_options->resize(297, 448);
        verticalLayout_4 = new QVBoxLayout(Log_options);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        frame = new QFrame(Log_options);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout = new QVBoxLayout(frame);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout_2 = new QVBoxLayout(groupBox);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        cb_readOnly = new QCheckBox(groupBox);
        cb_readOnly->setObjectName(QStringLiteral("cb_readOnly"));

        verticalLayout_2->addWidget(cb_readOnly);

        cb_acceptRichText = new QCheckBox(groupBox);
        cb_acceptRichText->setObjectName(QStringLiteral("cb_acceptRichText"));

        verticalLayout_2->addWidget(cb_acceptRichText);

        cb_NoCRLF = new QCheckBox(groupBox);
        cb_NoCRLF->setObjectName(QStringLiteral("cb_NoCRLF"));

        verticalLayout_2->addWidget(cb_NoCRLF);

        cb_add_DateTime = new QCheckBox(groupBox);
        cb_add_DateTime->setObjectName(QStringLiteral("cb_add_DateTime"));

        verticalLayout_2->addWidget(cb_add_DateTime);

        cb_Color = new QCheckBox(groupBox);
        cb_Color->setObjectName(QStringLiteral("cb_Color"));

        verticalLayout_2->addWidget(cb_Color);

        cb_ErrorAsMessage = new QCheckBox(groupBox);
        cb_ErrorAsMessage->setObjectName(QStringLiteral("cb_ErrorAsMessage"));

        verticalLayout_2->addWidget(cb_ErrorAsMessage);

        cb_Text_Is_Windows = new QCheckBox(groupBox);
        cb_Text_Is_Windows->setObjectName(QStringLiteral("cb_Text_Is_Windows"));

        verticalLayout_2->addWidget(cb_Text_Is_Windows);


        verticalLayout->addWidget(groupBox);


        verticalLayout_4->addWidget(frame);

        frame_2 = new QFrame(Log_options);
        frame_2->setObjectName(QStringLiteral("frame_2"));
        frame_2->setFrameShape(QFrame::StyledPanel);
        frame_2->setFrameShadow(QFrame::Raised);
        verticalLayout_3 = new QVBoxLayout(frame_2);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        groupBox_2 = new QGroupBox(frame_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_5 = new QVBoxLayout(groupBox_2);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        cb_CodecForCStrings = new QComboBox(groupBox_2);
        cb_CodecForCStrings->setObjectName(QStringLiteral("cb_CodecForCStrings"));

        horizontalLayout_2->addWidget(cb_CodecForCStrings);


        verticalLayout_5->addLayout(horizontalLayout_2);


        verticalLayout_3->addWidget(groupBox_2);


        verticalLayout_4->addWidget(frame_2);

        buttonBox = new QDialogButtonBox(Log_options);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        buttonBox->setCenterButtons(true);

        verticalLayout_4->addWidget(buttonBox);


        retranslateUi(Log_options);

        QMetaObject::connectSlotsByName(Log_options);
    } // setupUi

    void retranslateUi(QDialog *Log_options)
    {
        Log_options->setWindowTitle(QApplication::translate("Log_options", "Form", nullptr));
        groupBox->setTitle(QApplication::translate("Log_options", "Flags", nullptr));
        cb_readOnly->setText(QApplication::translate("Log_options", "readOnly", nullptr));
        cb_acceptRichText->setText(QApplication::translate("Log_options", "acceptRichText", nullptr));
        cb_NoCRLF->setText(QApplication::translate("Log_options", "no CRLF", nullptr));
        cb_add_DateTime->setText(QApplication::translate("Log_options", "add DateTime", nullptr));
        cb_Color->setText(QApplication::translate("Log_options", "color", nullptr));
        cb_ErrorAsMessage->setText(QApplication::translate("Log_options", "Error as Message", nullptr));
        cb_Text_Is_Windows->setText(QApplication::translate("Log_options", "is windows", nullptr));
        groupBox_2->setTitle(QApplication::translate("Log_options", "Codecs", nullptr));
        label_2->setText(QApplication::translate("Log_options", "CodecForCStrings", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Log_options: public Ui_Log_options {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOG_OPTIONS_H
