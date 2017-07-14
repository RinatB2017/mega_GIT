/********************************************************************************
** Form generated from reading UI file 'serialbox.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SERIALBOX_H
#define UI_SERIALBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_SerialBox
{
public:
    QHBoxLayout *horizontalLayout;
    QFrame *frame;
    QVBoxLayout *verticalLayout_2;
    QLabel *captionBox;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label_5;
    QLabel *label_4;
    QLabel *label;
    QLabel *label_6;
    QComboBox *BaudBox;
    QLabel *label_2;
    QComboBox *FlowBox;
    QComboBox *PortBox;
    QComboBox *ParityBox;
    QComboBox *StopBitsBox;
    QComboBox *DataBitsBox;
    QLabel *label_3;
    QPushButton *btn_default;
    QVBoxLayout *buttons_layout;
    QVBoxLayout *layout_SEND;
    QHBoxLayout *layout_status;
    QVBoxLayout *layout_bottom_LOG;
    QSpacerItem *verticalSpacer;
    QVBoxLayout *layout_power;
    QPushButton *btn_power;
    QSpacerItem *verticalSpacer_2;
    QVBoxLayout *layout_right_LOG;

    void setupUi(QFrame *SerialBox)
    {
        if (SerialBox->objectName().isEmpty())
            SerialBox->setObjectName(QStringLiteral("SerialBox"));
        SerialBox->resize(257, 322);
        horizontalLayout = new QHBoxLayout(SerialBox);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(SerialBox);
        frame->setObjectName(QStringLiteral("frame"));
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_2 = new QVBoxLayout(frame);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        captionBox = new QLabel(frame);
        captionBox->setObjectName(QStringLiteral("captionBox"));
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        captionBox->setFont(font);
        captionBox->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(captionBox);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        label_5 = new QLabel(frame);
        label_5->setObjectName(QStringLiteral("label_5"));

        gridLayout->addWidget(label_5, 5, 0, 1, 1);

        label_4 = new QLabel(frame);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        label = new QLabel(frame);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        label_6 = new QLabel(frame);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout->addWidget(label_6, 4, 0, 1, 1);

        BaudBox = new QComboBox(frame);
        BaudBox->setObjectName(QStringLiteral("BaudBox"));

        gridLayout->addWidget(BaudBox, 1, 1, 1, 1);

        label_2 = new QLabel(frame);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        FlowBox = new QComboBox(frame);
        FlowBox->setObjectName(QStringLiteral("FlowBox"));

        gridLayout->addWidget(FlowBox, 5, 1, 1, 1);

        PortBox = new QComboBox(frame);
        PortBox->setObjectName(QStringLiteral("PortBox"));

        gridLayout->addWidget(PortBox, 0, 1, 1, 1);

        ParityBox = new QComboBox(frame);
        ParityBox->setObjectName(QStringLiteral("ParityBox"));

        gridLayout->addWidget(ParityBox, 3, 1, 1, 1);

        StopBitsBox = new QComboBox(frame);
        StopBitsBox->setObjectName(QStringLiteral("StopBitsBox"));

        gridLayout->addWidget(StopBitsBox, 4, 1, 1, 1);

        DataBitsBox = new QComboBox(frame);
        DataBitsBox->setObjectName(QStringLiteral("DataBitsBox"));

        gridLayout->addWidget(DataBitsBox, 2, 1, 1, 1);

        label_3 = new QLabel(frame);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);


        verticalLayout->addLayout(gridLayout);

        btn_default = new QPushButton(frame);
        btn_default->setObjectName(QStringLiteral("btn_default"));

        verticalLayout->addWidget(btn_default);

        buttons_layout = new QVBoxLayout();
        buttons_layout->setObjectName(QStringLiteral("buttons_layout"));

        verticalLayout->addLayout(buttons_layout);

        layout_SEND = new QVBoxLayout();
        layout_SEND->setObjectName(QStringLiteral("layout_SEND"));

        verticalLayout->addLayout(layout_SEND);

        layout_status = new QHBoxLayout();
        layout_status->setObjectName(QStringLiteral("layout_status"));

        verticalLayout->addLayout(layout_status);

        layout_bottom_LOG = new QVBoxLayout();
        layout_bottom_LOG->setObjectName(QStringLiteral("layout_bottom_LOG"));

        verticalLayout->addLayout(layout_bottom_LOG);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addLayout(verticalLayout);

        layout_power = new QVBoxLayout();
        layout_power->setObjectName(QStringLiteral("layout_power"));
        btn_power = new QPushButton(frame);
        btn_power->setObjectName(QStringLiteral("btn_power"));
        btn_power->setMinimumSize(QSize(33, 70));
        btn_power->setMaximumSize(QSize(33, 70));
        btn_power->setStyleSheet(QLatin1String("QPushButton#btn_power {\n"
"	image: url(:/power_knock/image/black/off.png);\n"
"	border-style: inset;\n"
"}\n"
"QPushButton#btn_power:checked {\n"
"	image: url(:/power_knock/image/green/on.png);\n"
"	border-style: inset;\n"
"}"));
        btn_power->setCheckable(true);

        layout_power->addWidget(btn_power);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        layout_power->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(layout_power);

        layout_right_LOG = new QVBoxLayout();
        layout_right_LOG->setObjectName(QStringLiteral("layout_right_LOG"));

        horizontalLayout_2->addLayout(layout_right_LOG);


        verticalLayout_2->addLayout(horizontalLayout_2);


        horizontalLayout->addWidget(frame);


        retranslateUi(SerialBox);

        QMetaObject::connectSlotsByName(SerialBox);
    } // setupUi

    void retranslateUi(QFrame *SerialBox)
    {
        label_5->setText(QApplication::translate("SerialBox", "\320\243\320\277\321\200. \320\277\320\276\321\202\320\276\320\272\320\276\320\274:", Q_NULLPTR));
        label_4->setText(QApplication::translate("SerialBox", "\320\247\320\265\321\202\320\275\320\276\321\201\321\202\321\214:", Q_NULLPTR));
        label->setText(QApplication::translate("SerialBox", "\320\243\321\201\321\202\321\200\320\276\320\271\321\201\321\202\320\262\320\276:", Q_NULLPTR));
        label_6->setText(QApplication::translate("SerialBox", "\320\241\321\202\320\276\320\277\320\276\320\262\321\213\320\265 \320\261\320\270\321\202\321\213:", Q_NULLPTR));
        label_2->setText(QApplication::translate("SerialBox", "\320\241\320\272\320\276\321\200\320\276\321\201\321\202\321\214 \320\261\320\270\321\202/\321\201:", Q_NULLPTR));
        label_3->setText(QApplication::translate("SerialBox", "\320\221\320\270\321\202\321\213 \320\264\320\260\320\275\320\275\321\213\321\205:", Q_NULLPTR));
        btn_default->setText(QApplication::translate("SerialBox", "\320\237\320\276 \321\203\320\274\320\276\320\273\321\207\320\260\320\275\320\270\321\216 (9600 etc.)", Q_NULLPTR));
        Q_UNUSED(SerialBox);
    } // retranslateUi

};

namespace Ui {
    class SerialBox: public Ui_SerialBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SERIALBOX_H
