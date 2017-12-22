/********************************************************************************
** Form generated from reading UI file 'webcamera.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WEBCAMERA_H
#define UI_WEBCAMERA_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "cqtopencvviewergl.h"

QT_BEGIN_NAMESPACE

class Ui_WebCamera
{
public:
    QAction *actionStart;
    QAction *actionStop;
    QAction *actionTest;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QFrame *main_frame;
    QHBoxLayout *horizontalLayout_2;
    CQtOpenCVViewerGl *cameraWidget;
    QFrame *frame;
    QVBoxLayout *verticalLayout_4;
    QGroupBox *groupBox_3;
    QVBoxLayout *verticalLayout_5;
    QLineEdit *le_device;
    QGroupBox *groupBox;
    QVBoxLayout *verticalLayout;
    QCheckBox *checkBox_eyes;
    QCheckBox *checkBox_nose;
    QCheckBox *checkBox_mouth;
    QGroupBox *groupBox_2;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout;
    QLabel *lbl_saturation;
    QLabel *lbl_hue;
    QLabel *lbl_contrast;
    QLabel *label;
    QLabel *lbl_brightness;
    QLabel *label_4;
    QSlider *sl_contrast;
    QSlider *sl_brightness;
    QSlider *sl_saturation;
    QLabel *label_2;
    QLabel *label_3;
    QSlider *sl_hue;
    QHBoxLayout *horizontalLayout;
    QToolButton *btn_start;
    QToolButton *btn_stop;
    QSpacerItem *horizontalSpacer;
    QPushButton *btn_test;
    QSpacerItem *verticalSpacer;
    QLabel *lbl_result;

    void setupUi(QWidget *WebCamera)
    {
        if (WebCamera->objectName().isEmpty())
            WebCamera->setObjectName(QStringLiteral("WebCamera"));
        WebCamera->resize(315, 569);
        actionStart = new QAction(WebCamera);
        actionStart->setObjectName(QStringLiteral("actionStart"));
        actionStop = new QAction(WebCamera);
        actionStop->setObjectName(QStringLiteral("actionStop"));
        actionTest = new QAction(WebCamera);
        actionTest->setObjectName(QStringLiteral("actionTest"));
        verticalLayout_2 = new QVBoxLayout(WebCamera);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        main_frame = new QFrame(WebCamera);
        main_frame->setObjectName(QStringLiteral("main_frame"));
        main_frame->setFrameShape(QFrame::StyledPanel);
        main_frame->setFrameShadow(QFrame::Raised);
        horizontalLayout_2 = new QHBoxLayout(main_frame);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        cameraWidget = new CQtOpenCVViewerGl(main_frame);
        cameraWidget->setObjectName(QStringLiteral("cameraWidget"));

        horizontalLayout_2->addWidget(cameraWidget);

        frame = new QFrame(main_frame);
        frame->setObjectName(QStringLiteral("frame"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(frame->sizePolicy().hasHeightForWidth());
        frame->setSizePolicy(sizePolicy);
        frame->setFrameShape(QFrame::StyledPanel);
        frame->setFrameShadow(QFrame::Raised);
        verticalLayout_4 = new QVBoxLayout(frame);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        groupBox_3 = new QGroupBox(frame);
        groupBox_3->setObjectName(QStringLiteral("groupBox_3"));
        verticalLayout_5 = new QVBoxLayout(groupBox_3);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        le_device = new QLineEdit(groupBox_3);
        le_device->setObjectName(QStringLiteral("le_device"));

        verticalLayout_5->addWidget(le_device);


        verticalLayout_4->addWidget(groupBox_3);

        groupBox = new QGroupBox(frame);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        verticalLayout = new QVBoxLayout(groupBox);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        checkBox_eyes = new QCheckBox(groupBox);
        checkBox_eyes->setObjectName(QStringLiteral("checkBox_eyes"));

        verticalLayout->addWidget(checkBox_eyes);

        checkBox_nose = new QCheckBox(groupBox);
        checkBox_nose->setObjectName(QStringLiteral("checkBox_nose"));

        verticalLayout->addWidget(checkBox_nose);

        checkBox_mouth = new QCheckBox(groupBox);
        checkBox_mouth->setObjectName(QStringLiteral("checkBox_mouth"));

        verticalLayout->addWidget(checkBox_mouth);


        verticalLayout_4->addWidget(groupBox);

        groupBox_2 = new QGroupBox(frame);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        verticalLayout_3 = new QVBoxLayout(groupBox_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        lbl_saturation = new QLabel(groupBox_2);
        lbl_saturation->setObjectName(QStringLiteral("lbl_saturation"));

        gridLayout->addWidget(lbl_saturation, 2, 2, 1, 1);

        lbl_hue = new QLabel(groupBox_2);
        lbl_hue->setObjectName(QStringLiteral("lbl_hue"));

        gridLayout->addWidget(lbl_hue, 3, 2, 1, 1);

        lbl_contrast = new QLabel(groupBox_2);
        lbl_contrast->setObjectName(QStringLiteral("lbl_contrast"));

        gridLayout->addWidget(lbl_contrast, 1, 2, 1, 1);

        label = new QLabel(groupBox_2);
        label->setObjectName(QStringLiteral("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        lbl_brightness = new QLabel(groupBox_2);
        lbl_brightness->setObjectName(QStringLiteral("lbl_brightness"));
        lbl_brightness->setMinimumSize(QSize(50, 0));

        gridLayout->addWidget(lbl_brightness, 0, 2, 1, 1);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QStringLiteral("label_4"));

        gridLayout->addWidget(label_4, 3, 0, 1, 1);

        sl_contrast = new QSlider(groupBox_2);
        sl_contrast->setObjectName(QStringLiteral("sl_contrast"));
        sl_contrast->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sl_contrast, 1, 1, 1, 1);

        sl_brightness = new QSlider(groupBox_2);
        sl_brightness->setObjectName(QStringLiteral("sl_brightness"));
        sl_brightness->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sl_brightness, 0, 1, 1, 1);

        sl_saturation = new QSlider(groupBox_2);
        sl_saturation->setObjectName(QStringLiteral("sl_saturation"));
        sl_saturation->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sl_saturation, 2, 1, 1, 1);

        label_2 = new QLabel(groupBox_2);
        label_2->setObjectName(QStringLiteral("label_2"));

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QStringLiteral("label_3"));

        gridLayout->addWidget(label_3, 2, 0, 1, 1);

        sl_hue = new QSlider(groupBox_2);
        sl_hue->setObjectName(QStringLiteral("sl_hue"));
        sl_hue->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(sl_hue, 3, 1, 1, 1);


        verticalLayout_3->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        btn_start = new QToolButton(groupBox_2);
        btn_start->setObjectName(QStringLiteral("btn_start"));

        horizontalLayout->addWidget(btn_start);

        btn_stop = new QToolButton(groupBox_2);
        btn_stop->setObjectName(QStringLiteral("btn_stop"));

        horizontalLayout->addWidget(btn_stop);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout);


        verticalLayout_4->addWidget(groupBox_2);

        btn_test = new QPushButton(frame);
        btn_test->setObjectName(QStringLiteral("btn_test"));

        verticalLayout_4->addWidget(btn_test);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_4->addItem(verticalSpacer);

        lbl_result = new QLabel(frame);
        lbl_result->setObjectName(QStringLiteral("lbl_result"));

        verticalLayout_4->addWidget(lbl_result);


        horizontalLayout_2->addWidget(frame);


        horizontalLayout_3->addWidget(main_frame);


        verticalLayout_2->addLayout(horizontalLayout_3);


        retranslateUi(WebCamera);

        QMetaObject::connectSlotsByName(WebCamera);
    } // setupUi

    void retranslateUi(QWidget *WebCamera)
    {
        WebCamera->setWindowTitle(QApplication::translate("WebCamera", "Camera", nullptr));
        actionStart->setText(QApplication::translate("WebCamera", "&Start", nullptr));
        actionStop->setText(QApplication::translate("WebCamera", "S&top", nullptr));
        actionTest->setText(QApplication::translate("WebCamera", "&Test", nullptr));
        groupBox_3->setTitle(QApplication::translate("WebCamera", "Device", nullptr));
        groupBox->setTitle(QApplication::translate("WebCamera", "Options", nullptr));
        checkBox_eyes->setText(QApplication::translate("WebCamera", "Detect Eyes", nullptr));
        checkBox_nose->setText(QApplication::translate("WebCamera", "Detect Nose", nullptr));
        checkBox_mouth->setText(QApplication::translate("WebCamera", "Detect Mouth", nullptr));
        groupBox_2->setTitle(QApplication::translate("WebCamera", "Camera", nullptr));
        lbl_saturation->setText(QApplication::translate("WebCamera", "0", nullptr));
        lbl_hue->setText(QApplication::translate("WebCamera", "0", nullptr));
        lbl_contrast->setText(QApplication::translate("WebCamera", "0", nullptr));
        label->setText(QApplication::translate("WebCamera", "Brightness", nullptr));
        lbl_brightness->setText(QApplication::translate("WebCamera", "0", nullptr));
        label_4->setText(QApplication::translate("WebCamera", "Hue", nullptr));
        label_2->setText(QApplication::translate("WebCamera", "Contrast", nullptr));
        label_3->setText(QApplication::translate("WebCamera", "Saturation", nullptr));
        btn_start->setText(QApplication::translate("WebCamera", "...", nullptr));
        btn_stop->setText(QApplication::translate("WebCamera", "...", nullptr));
        btn_test->setText(QApplication::translate("WebCamera", "test", nullptr));
    } // retranslateUi

};

namespace Ui {
    class WebCamera: public Ui_WebCamera {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WEBCAMERA_H
