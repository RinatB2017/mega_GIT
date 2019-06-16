/********************************************************************************
** Form generated from reading UI file 'sendframebox.ui'
**
** Created by: Qt User Interface Compiler version 5.12.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDFRAMEBOX_H
#define UI_SENDFRAMEBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SendFrameBox
{
public:
    QVBoxLayout *verticalLayout_4;
    QGroupBox *frameTypeBox;
    QHBoxLayout *horizontalLayout;
    QRadioButton *dataFrame;
    QRadioButton *remoteFrame;
    QRadioButton *errorFrame;
    QGroupBox *frameOptionsBox;
    QHBoxLayout *horizontalLayout_2;
    QCheckBox *extendedFormatBox;
    QCheckBox *flexibleDataRateBox;
    QCheckBox *bitrateSwitchBox;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout;
    QLabel *frameIdLabel;
    QLineEdit *frameIdEdit;
    QVBoxLayout *verticalLayout_2;
    QLabel *payloadLabel;
    QLineEdit *payloadEdit;
    QVBoxLayout *verticalLayout_3;
    QSpacerItem *verticalSpacer;
    QPushButton *sendButton;

    void setupUi(QGroupBox *SendFrameBox)
    {
        if (SendFrameBox->objectName().isEmpty())
            SendFrameBox->setObjectName(QString::fromUtf8("SendFrameBox"));
        SendFrameBox->resize(485, 206);
        verticalLayout_4 = new QVBoxLayout(SendFrameBox);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        frameTypeBox = new QGroupBox(SendFrameBox);
        frameTypeBox->setObjectName(QString::fromUtf8("frameTypeBox"));
        frameTypeBox->setCheckable(false);
        horizontalLayout = new QHBoxLayout(frameTypeBox);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(-1, 0, -1, 0);
        dataFrame = new QRadioButton(frameTypeBox);
        dataFrame->setObjectName(QString::fromUtf8("dataFrame"));
        dataFrame->setChecked(true);

        horizontalLayout->addWidget(dataFrame);

        remoteFrame = new QRadioButton(frameTypeBox);
        remoteFrame->setObjectName(QString::fromUtf8("remoteFrame"));

        horizontalLayout->addWidget(remoteFrame);

        errorFrame = new QRadioButton(frameTypeBox);
        errorFrame->setObjectName(QString::fromUtf8("errorFrame"));

        horizontalLayout->addWidget(errorFrame);


        verticalLayout_4->addWidget(frameTypeBox);

        frameOptionsBox = new QGroupBox(SendFrameBox);
        frameOptionsBox->setObjectName(QString::fromUtf8("frameOptionsBox"));
        horizontalLayout_2 = new QHBoxLayout(frameOptionsBox);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(-1, 0, -1, 0);
        extendedFormatBox = new QCheckBox(frameOptionsBox);
        extendedFormatBox->setObjectName(QString::fromUtf8("extendedFormatBox"));

        horizontalLayout_2->addWidget(extendedFormatBox);

        flexibleDataRateBox = new QCheckBox(frameOptionsBox);
        flexibleDataRateBox->setObjectName(QString::fromUtf8("flexibleDataRateBox"));

        horizontalLayout_2->addWidget(flexibleDataRateBox);

        bitrateSwitchBox = new QCheckBox(frameOptionsBox);
        bitrateSwitchBox->setObjectName(QString::fromUtf8("bitrateSwitchBox"));
        bitrateSwitchBox->setEnabled(false);

        horizontalLayout_2->addWidget(bitrateSwitchBox);


        verticalLayout_4->addWidget(frameOptionsBox);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        frameIdLabel = new QLabel(SendFrameBox);
        frameIdLabel->setObjectName(QString::fromUtf8("frameIdLabel"));

        verticalLayout->addWidget(frameIdLabel);

        frameIdEdit = new QLineEdit(SendFrameBox);
        frameIdEdit->setObjectName(QString::fromUtf8("frameIdEdit"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(1);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frameIdEdit->sizePolicy().hasHeightForWidth());
        frameIdEdit->setSizePolicy(sizePolicy);
        frameIdEdit->setClearButtonEnabled(true);

        verticalLayout->addWidget(frameIdEdit);


        horizontalLayout_3->addLayout(verticalLayout);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        payloadLabel = new QLabel(SendFrameBox);
        payloadLabel->setObjectName(QString::fromUtf8("payloadLabel"));

        verticalLayout_2->addWidget(payloadLabel);

        payloadEdit = new QLineEdit(SendFrameBox);
        payloadEdit->setObjectName(QString::fromUtf8("payloadEdit"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(2);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(payloadEdit->sizePolicy().hasHeightForWidth());
        payloadEdit->setSizePolicy(sizePolicy1);
        payloadEdit->setClearButtonEnabled(true);

        verticalLayout_2->addWidget(payloadEdit);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer);

        sendButton = new QPushButton(SendFrameBox);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));

        verticalLayout_3->addWidget(sendButton);


        horizontalLayout_3->addLayout(verticalLayout_3);


        verticalLayout_4->addLayout(horizontalLayout_3);

#ifndef QT_NO_SHORTCUT
        frameIdLabel->setBuddy(frameIdEdit);
        payloadLabel->setBuddy(payloadEdit);
#endif // QT_NO_SHORTCUT

        retranslateUi(SendFrameBox);

        QMetaObject::connectSlotsByName(SendFrameBox);
    } // setupUi

    void retranslateUi(QGroupBox *SendFrameBox)
    {
        SendFrameBox->setWindowTitle(QApplication::translate("SendFrameBox", "Dialog", nullptr));
        SendFrameBox->setTitle(QString());
        frameTypeBox->setTitle(QApplication::translate("SendFrameBox", "Frame Type", nullptr));
#ifndef QT_NO_TOOLTIP
        dataFrame->setToolTip(QApplication::translate("SendFrameBox", "Sends a CAN data frame.", nullptr));
#endif // QT_NO_TOOLTIP
        dataFrame->setText(QApplication::translate("SendFrameBox", "D&ata Frame", nullptr));
#ifndef QT_NO_TOOLTIP
        remoteFrame->setToolTip(QApplication::translate("SendFrameBox", "Sends a CAN remote request frame.", nullptr));
#endif // QT_NO_TOOLTIP
        remoteFrame->setText(QApplication::translate("SendFrameBox", "Re&mote Request Frame", nullptr));
#ifndef QT_NO_TOOLTIP
        errorFrame->setToolTip(QApplication::translate("SendFrameBox", "Sends an error frame.", nullptr));
#endif // QT_NO_TOOLTIP
        errorFrame->setText(QApplication::translate("SendFrameBox", "&Error Frame", nullptr));
        frameOptionsBox->setTitle(QApplication::translate("SendFrameBox", "Frame Options", nullptr));
#ifndef QT_NO_TOOLTIP
        extendedFormatBox->setToolTip(QApplication::translate("SendFrameBox", "Allows extended frames with 29 bit identifier.", nullptr));
#endif // QT_NO_TOOLTIP
        extendedFormatBox->setText(QApplication::translate("SendFrameBox", "E&xtended Format", nullptr));
#ifndef QT_NO_TOOLTIP
        flexibleDataRateBox->setToolTip(QApplication::translate("SendFrameBox", "Allows up to 64 byte payload data.", nullptr));
#endif // QT_NO_TOOLTIP
        flexibleDataRateBox->setText(QApplication::translate("SendFrameBox", "&Flexible Data-Rate", nullptr));
#ifndef QT_NO_TOOLTIP
        bitrateSwitchBox->setToolTip(QApplication::translate("SendFrameBox", "Sends payload at higher data rate.", nullptr));
#endif // QT_NO_TOOLTIP
        bitrateSwitchBox->setText(QApplication::translate("SendFrameBox", "&Bitrate Switch", nullptr));
        frameIdLabel->setText(QApplication::translate("SendFrameBox", "Frame &ID (hex)", nullptr));
        frameIdEdit->setPlaceholderText(QApplication::translate("SendFrameBox", "123", nullptr));
        payloadLabel->setText(QApplication::translate("SendFrameBox", "&Payload (hex)", nullptr));
        payloadEdit->setPlaceholderText(QApplication::translate("SendFrameBox", "12 34 AB CE", nullptr));
        sendButton->setText(QApplication::translate("SendFrameBox", "&Send", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SendFrameBox: public Ui_SendFrameBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDFRAMEBOX_H
