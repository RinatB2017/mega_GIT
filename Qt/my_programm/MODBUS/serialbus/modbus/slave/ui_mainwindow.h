/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.14.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionExit;
    QAction *actionOptions;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QGridLayout *gridLayout;
    QLabel *label_27;
    QPushButton *connectButton;
    QSpinBox *serverEdit;
    QSpacerItem *horizontalSpacer;
    QComboBox *connectType;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *portEdit;
    QHBoxLayout *horizontalLayout;
    QCheckBox *setBusyBox;
    QCheckBox *listenOnlyBox;
    QSpacerItem *horizontalSpacer_2;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout_2;
    QCheckBox *disc_2;
    QLabel *label_23;
    QLineEdit *holdReg_2;
    QCheckBox *coils_3;
    QLineEdit *inReg_2;
    QCheckBox *disc_3;
    QLabel *label_11;
    QLineEdit *inReg_3;
    QCheckBox *coils_4;
    QLineEdit *holdReg_3;
    QCheckBox *disc_4;
    QLabel *label_8;
    QLineEdit *inReg_4;
    QLineEdit *holdReg_4;
    QCheckBox *coils_5;
    QLineEdit *inReg_5;
    QCheckBox *disc_5;
    QLabel *label_12;
    QLineEdit *holdReg_5;
    QCheckBox *coils_6;
    QLineEdit *holdReg_0;
    QCheckBox *coils_1;
    QCheckBox *disc_1;
    QLabel *label_22;
    QLineEdit *inReg_1;
    QLineEdit *holdReg_1;
    QCheckBox *coils_2;
    QLineEdit *inReg_9;
    QLabel *label_16;
    QCheckBox *disc_9;
    QLineEdit *holdReg_9;
    QLineEdit *inReg_6;
    QCheckBox *disc_6;
    QLineEdit *holdReg_6;
    QCheckBox *coils_7;
    QLabel *label_13;
    QCheckBox *disc_7;
    QLineEdit *inReg_7;
    QLabel *label_14;
    QLineEdit *holdReg_7;
    QCheckBox *coils_8;
    QCheckBox *disc_8;
    QLabel *label_15;
    QLineEdit *inReg_8;
    QCheckBox *coils_9;
    QLineEdit *holdReg_8;
    QLabel *discreteInputs;
    QLabel *inputRegisters;
    QLabel *coils;
    QLabel *address;
    QLabel *holdingRegisters;
    QCheckBox *coils_0;
    QCheckBox *disc_0;
    QLineEdit *inReg_0;
    QLabel *label_21;
    QSpacerItem *verticalSpacer_2;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menu_Device;
    QMenu *menuToo_ls;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(780, 503);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QString::fromUtf8("actionConnect"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/images/connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon);
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QString::fromUtf8("actionDisconnect"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/images/disconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon1);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionExit->setIcon(icon2);
        actionOptions = new QAction(MainWindow);
        actionOptions->setObjectName(QString::fromUtf8("actionOptions"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/settings.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOptions->setIcon(icon3);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(-1, 9, -1, 0);
        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        label_27 = new QLabel(centralWidget);
        label_27->setObjectName(QString::fromUtf8("label_27"));
        QSizePolicy sizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_27->sizePolicy().hasHeightForWidth());
        label_27->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_27, 0, 5, 1, 1);

        connectButton = new QPushButton(centralWidget);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));
        QSizePolicy sizePolicy1(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(connectButton->sizePolicy().hasHeightForWidth());
        connectButton->setSizePolicy(sizePolicy1);
        connectButton->setCheckable(false);
        connectButton->setAutoDefault(false);

        gridLayout->addWidget(connectButton, 0, 7, 1, 1);

        serverEdit = new QSpinBox(centralWidget);
        serverEdit->setObjectName(QString::fromUtf8("serverEdit"));
        sizePolicy1.setHeightForWidth(serverEdit->sizePolicy().hasHeightForWidth());
        serverEdit->setSizePolicy(sizePolicy1);
        serverEdit->setMinimum(1);
        serverEdit->setMaximum(247);

        gridLayout->addWidget(serverEdit, 0, 6, 1, 1);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 4, 1, 1);

        connectType = new QComboBox(centralWidget);
        connectType->addItem(QString());
        connectType->addItem(QString());
        connectType->setObjectName(QString::fromUtf8("connectType"));

        gridLayout->addWidget(connectType, 0, 1, 1, 1);

        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 0, 2, 1, 1);

        label = new QLabel(centralWidget);
        label->setObjectName(QString::fromUtf8("label"));

        gridLayout->addWidget(label, 0, 0, 1, 1);

        portEdit = new QLineEdit(centralWidget);
        portEdit->setObjectName(QString::fromUtf8("portEdit"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(portEdit->sizePolicy().hasHeightForWidth());
        portEdit->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(portEdit, 0, 3, 1, 1);


        verticalLayout->addLayout(gridLayout);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        setBusyBox = new QCheckBox(centralWidget);
        setBusyBox->setObjectName(QString::fromUtf8("setBusyBox"));

        horizontalLayout->addWidget(setBusyBox);

        listenOnlyBox = new QCheckBox(centralWidget);
        listenOnlyBox->setObjectName(QString::fromUtf8("listenOnlyBox"));

        horizontalLayout->addWidget(listenOnlyBox);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout->addLayout(horizontalLayout);

        verticalSpacer = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Maximum);

        verticalLayout->addItem(verticalSpacer);

        gridLayout_2 = new QGridLayout();
        gridLayout_2->setSpacing(6);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        disc_2 = new QCheckBox(centralWidget);
        disc_2->setObjectName(QString::fromUtf8("disc_2"));

        gridLayout_2->addWidget(disc_2, 3, 1, 1, 1);

        label_23 = new QLabel(centralWidget);
        label_23->setObjectName(QString::fromUtf8("label_23"));
        label_23->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_23, 3, 2, 1, 1);

        holdReg_2 = new QLineEdit(centralWidget);
        holdReg_2->setObjectName(QString::fromUtf8("holdReg_2"));
        holdReg_2->setMaxLength(32767);

        gridLayout_2->addWidget(holdReg_2, 3, 4, 1, 1);

        coils_3 = new QCheckBox(centralWidget);
        coils_3->setObjectName(QString::fromUtf8("coils_3"));
        sizePolicy1.setHeightForWidth(coils_3->sizePolicy().hasHeightForWidth());
        coils_3->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(coils_3, 4, 0, 1, 1);

        inReg_2 = new QLineEdit(centralWidget);
        inReg_2->setObjectName(QString::fromUtf8("inReg_2"));
        inReg_2->setMaxLength(32767);

        gridLayout_2->addWidget(inReg_2, 3, 3, 1, 1);

        disc_3 = new QCheckBox(centralWidget);
        disc_3->setObjectName(QString::fromUtf8("disc_3"));

        gridLayout_2->addWidget(disc_3, 4, 1, 1, 1);

        label_11 = new QLabel(centralWidget);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_11, 4, 2, 1, 1);

        inReg_3 = new QLineEdit(centralWidget);
        inReg_3->setObjectName(QString::fromUtf8("inReg_3"));
        inReg_3->setMaxLength(32767);

        gridLayout_2->addWidget(inReg_3, 4, 3, 1, 1);

        coils_4 = new QCheckBox(centralWidget);
        coils_4->setObjectName(QString::fromUtf8("coils_4"));
        sizePolicy1.setHeightForWidth(coils_4->sizePolicy().hasHeightForWidth());
        coils_4->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(coils_4, 5, 0, 1, 1);

        holdReg_3 = new QLineEdit(centralWidget);
        holdReg_3->setObjectName(QString::fromUtf8("holdReg_3"));
        holdReg_3->setMaxLength(32767);

        gridLayout_2->addWidget(holdReg_3, 4, 4, 1, 1);

        disc_4 = new QCheckBox(centralWidget);
        disc_4->setObjectName(QString::fromUtf8("disc_4"));

        gridLayout_2->addWidget(disc_4, 5, 1, 1, 1);

        label_8 = new QLabel(centralWidget);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_8, 5, 2, 1, 1);

        inReg_4 = new QLineEdit(centralWidget);
        inReg_4->setObjectName(QString::fromUtf8("inReg_4"));
        inReg_4->setMaxLength(32767);

        gridLayout_2->addWidget(inReg_4, 5, 3, 1, 1);

        holdReg_4 = new QLineEdit(centralWidget);
        holdReg_4->setObjectName(QString::fromUtf8("holdReg_4"));
        holdReg_4->setMaxLength(32767);

        gridLayout_2->addWidget(holdReg_4, 5, 4, 1, 1);

        coils_5 = new QCheckBox(centralWidget);
        coils_5->setObjectName(QString::fromUtf8("coils_5"));
        sizePolicy1.setHeightForWidth(coils_5->sizePolicy().hasHeightForWidth());
        coils_5->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(coils_5, 6, 0, 1, 1);

        inReg_5 = new QLineEdit(centralWidget);
        inReg_5->setObjectName(QString::fromUtf8("inReg_5"));
        inReg_5->setMaxLength(32767);

        gridLayout_2->addWidget(inReg_5, 6, 3, 1, 1);

        disc_5 = new QCheckBox(centralWidget);
        disc_5->setObjectName(QString::fromUtf8("disc_5"));

        gridLayout_2->addWidget(disc_5, 6, 1, 1, 1);

        label_12 = new QLabel(centralWidget);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_12, 6, 2, 1, 1);

        holdReg_5 = new QLineEdit(centralWidget);
        holdReg_5->setObjectName(QString::fromUtf8("holdReg_5"));
        holdReg_5->setMaxLength(32767);

        gridLayout_2->addWidget(holdReg_5, 6, 4, 1, 1);

        coils_6 = new QCheckBox(centralWidget);
        coils_6->setObjectName(QString::fromUtf8("coils_6"));
        sizePolicy1.setHeightForWidth(coils_6->sizePolicy().hasHeightForWidth());
        coils_6->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(coils_6, 7, 0, 1, 1);

        holdReg_0 = new QLineEdit(centralWidget);
        holdReg_0->setObjectName(QString::fromUtf8("holdReg_0"));
        holdReg_0->setMaxLength(32767);

        gridLayout_2->addWidget(holdReg_0, 1, 4, 1, 1);

        coils_1 = new QCheckBox(centralWidget);
        coils_1->setObjectName(QString::fromUtf8("coils_1"));
        sizePolicy1.setHeightForWidth(coils_1->sizePolicy().hasHeightForWidth());
        coils_1->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(coils_1, 2, 0, 1, 1);

        disc_1 = new QCheckBox(centralWidget);
        disc_1->setObjectName(QString::fromUtf8("disc_1"));

        gridLayout_2->addWidget(disc_1, 2, 1, 1, 1);

        label_22 = new QLabel(centralWidget);
        label_22->setObjectName(QString::fromUtf8("label_22"));
        label_22->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_22, 2, 2, 1, 1);

        inReg_1 = new QLineEdit(centralWidget);
        inReg_1->setObjectName(QString::fromUtf8("inReg_1"));
        inReg_1->setMaxLength(32767);

        gridLayout_2->addWidget(inReg_1, 2, 3, 1, 1);

        holdReg_1 = new QLineEdit(centralWidget);
        holdReg_1->setObjectName(QString::fromUtf8("holdReg_1"));
        holdReg_1->setMaxLength(32767);

        gridLayout_2->addWidget(holdReg_1, 2, 4, 1, 1);

        coils_2 = new QCheckBox(centralWidget);
        coils_2->setObjectName(QString::fromUtf8("coils_2"));
        sizePolicy1.setHeightForWidth(coils_2->sizePolicy().hasHeightForWidth());
        coils_2->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(coils_2, 3, 0, 1, 1);

        inReg_9 = new QLineEdit(centralWidget);
        inReg_9->setObjectName(QString::fromUtf8("inReg_9"));
        inReg_9->setMaxLength(32767);

        gridLayout_2->addWidget(inReg_9, 10, 3, 1, 1);

        label_16 = new QLabel(centralWidget);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_16, 10, 2, 1, 1);

        disc_9 = new QCheckBox(centralWidget);
        disc_9->setObjectName(QString::fromUtf8("disc_9"));

        gridLayout_2->addWidget(disc_9, 10, 1, 1, 1);

        holdReg_9 = new QLineEdit(centralWidget);
        holdReg_9->setObjectName(QString::fromUtf8("holdReg_9"));
        holdReg_9->setMaxLength(32767);

        gridLayout_2->addWidget(holdReg_9, 10, 4, 1, 1);

        inReg_6 = new QLineEdit(centralWidget);
        inReg_6->setObjectName(QString::fromUtf8("inReg_6"));
        inReg_6->setMaxLength(32767);

        gridLayout_2->addWidget(inReg_6, 7, 3, 1, 1);

        disc_6 = new QCheckBox(centralWidget);
        disc_6->setObjectName(QString::fromUtf8("disc_6"));

        gridLayout_2->addWidget(disc_6, 7, 1, 1, 1);

        holdReg_6 = new QLineEdit(centralWidget);
        holdReg_6->setObjectName(QString::fromUtf8("holdReg_6"));
        holdReg_6->setMaxLength(32767);

        gridLayout_2->addWidget(holdReg_6, 7, 4, 1, 1);

        coils_7 = new QCheckBox(centralWidget);
        coils_7->setObjectName(QString::fromUtf8("coils_7"));
        sizePolicy1.setHeightForWidth(coils_7->sizePolicy().hasHeightForWidth());
        coils_7->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(coils_7, 8, 0, 1, 1);

        label_13 = new QLabel(centralWidget);
        label_13->setObjectName(QString::fromUtf8("label_13"));
        label_13->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_13, 7, 2, 1, 1);

        disc_7 = new QCheckBox(centralWidget);
        disc_7->setObjectName(QString::fromUtf8("disc_7"));

        gridLayout_2->addWidget(disc_7, 8, 1, 1, 1);

        inReg_7 = new QLineEdit(centralWidget);
        inReg_7->setObjectName(QString::fromUtf8("inReg_7"));
        inReg_7->setMaxLength(32767);

        gridLayout_2->addWidget(inReg_7, 8, 3, 1, 1);

        label_14 = new QLabel(centralWidget);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_14, 8, 2, 1, 1);

        holdReg_7 = new QLineEdit(centralWidget);
        holdReg_7->setObjectName(QString::fromUtf8("holdReg_7"));
        holdReg_7->setMaxLength(32767);

        gridLayout_2->addWidget(holdReg_7, 8, 4, 1, 1);

        coils_8 = new QCheckBox(centralWidget);
        coils_8->setObjectName(QString::fromUtf8("coils_8"));
        sizePolicy1.setHeightForWidth(coils_8->sizePolicy().hasHeightForWidth());
        coils_8->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(coils_8, 9, 0, 1, 1);

        disc_8 = new QCheckBox(centralWidget);
        disc_8->setObjectName(QString::fromUtf8("disc_8"));

        gridLayout_2->addWidget(disc_8, 9, 1, 1, 1);

        label_15 = new QLabel(centralWidget);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_15, 9, 2, 1, 1);

        inReg_8 = new QLineEdit(centralWidget);
        inReg_8->setObjectName(QString::fromUtf8("inReg_8"));
        inReg_8->setMaxLength(32767);

        gridLayout_2->addWidget(inReg_8, 9, 3, 1, 1);

        coils_9 = new QCheckBox(centralWidget);
        coils_9->setObjectName(QString::fromUtf8("coils_9"));
        sizePolicy1.setHeightForWidth(coils_9->sizePolicy().hasHeightForWidth());
        coils_9->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(coils_9, 10, 0, 1, 1);

        holdReg_8 = new QLineEdit(centralWidget);
        holdReg_8->setObjectName(QString::fromUtf8("holdReg_8"));
        holdReg_8->setMaxLength(32767);

        gridLayout_2->addWidget(holdReg_8, 9, 4, 1, 1);

        discreteInputs = new QLabel(centralWidget);
        discreteInputs->setObjectName(QString::fromUtf8("discreteInputs"));

        gridLayout_2->addWidget(discreteInputs, 0, 1, 1, 1);

        inputRegisters = new QLabel(centralWidget);
        inputRegisters->setObjectName(QString::fromUtf8("inputRegisters"));

        gridLayout_2->addWidget(inputRegisters, 0, 3, 1, 1);

        coils = new QLabel(centralWidget);
        coils->setObjectName(QString::fromUtf8("coils"));
        sizePolicy.setHeightForWidth(coils->sizePolicy().hasHeightForWidth());
        coils->setSizePolicy(sizePolicy);

        gridLayout_2->addWidget(coils, 0, 0, 1, 1);

        address = new QLabel(centralWidget);
        address->setObjectName(QString::fromUtf8("address"));
        address->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(address, 0, 2, 1, 1);

        holdingRegisters = new QLabel(centralWidget);
        holdingRegisters->setObjectName(QString::fromUtf8("holdingRegisters"));

        gridLayout_2->addWidget(holdingRegisters, 0, 4, 1, 1);

        coils_0 = new QCheckBox(centralWidget);
        coils_0->setObjectName(QString::fromUtf8("coils_0"));
        sizePolicy1.setHeightForWidth(coils_0->sizePolicy().hasHeightForWidth());
        coils_0->setSizePolicy(sizePolicy1);

        gridLayout_2->addWidget(coils_0, 1, 0, 1, 1);

        disc_0 = new QCheckBox(centralWidget);
        disc_0->setObjectName(QString::fromUtf8("disc_0"));

        gridLayout_2->addWidget(disc_0, 1, 1, 1, 1);

        inReg_0 = new QLineEdit(centralWidget);
        inReg_0->setObjectName(QString::fromUtf8("inReg_0"));
        inReg_0->setMaxLength(32767);

        gridLayout_2->addWidget(inReg_0, 1, 3, 1, 1);

        label_21 = new QLabel(centralWidget);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_21, 1, 2, 1, 1);


        verticalLayout->addLayout(gridLayout_2);

        verticalSpacer_2 = new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        verticalLayout->addItem(verticalSpacer_2);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 780, 24));
        menu_Device = new QMenu(menuBar);
        menu_Device->setObjectName(QString::fromUtf8("menu_Device"));
        menuToo_ls = new QMenu(menuBar);
        menuToo_ls->setObjectName(QString::fromUtf8("menuToo_ls"));
        MainWindow->setMenuBar(menuBar);
        QWidget::setTabOrder(connectType, portEdit);
        QWidget::setTabOrder(portEdit, serverEdit);
        QWidget::setTabOrder(serverEdit, connectButton);
        QWidget::setTabOrder(connectButton, coils_0);
        QWidget::setTabOrder(coils_0, disc_0);
        QWidget::setTabOrder(disc_0, inReg_0);
        QWidget::setTabOrder(inReg_0, holdReg_0);
        QWidget::setTabOrder(holdReg_0, coils_1);
        QWidget::setTabOrder(coils_1, disc_1);
        QWidget::setTabOrder(disc_1, inReg_1);
        QWidget::setTabOrder(inReg_1, holdReg_1);
        QWidget::setTabOrder(holdReg_1, coils_2);
        QWidget::setTabOrder(coils_2, disc_2);
        QWidget::setTabOrder(disc_2, inReg_2);
        QWidget::setTabOrder(inReg_2, holdReg_2);
        QWidget::setTabOrder(holdReg_2, coils_3);
        QWidget::setTabOrder(coils_3, disc_3);
        QWidget::setTabOrder(disc_3, inReg_3);
        QWidget::setTabOrder(inReg_3, holdReg_3);
        QWidget::setTabOrder(holdReg_3, coils_4);
        QWidget::setTabOrder(coils_4, disc_4);
        QWidget::setTabOrder(disc_4, inReg_4);
        QWidget::setTabOrder(inReg_4, holdReg_4);
        QWidget::setTabOrder(holdReg_4, coils_5);
        QWidget::setTabOrder(coils_5, disc_5);
        QWidget::setTabOrder(disc_5, inReg_5);
        QWidget::setTabOrder(inReg_5, holdReg_5);
        QWidget::setTabOrder(holdReg_5, coils_6);
        QWidget::setTabOrder(coils_6, disc_6);
        QWidget::setTabOrder(disc_6, inReg_6);
        QWidget::setTabOrder(inReg_6, holdReg_6);
        QWidget::setTabOrder(holdReg_6, coils_7);
        QWidget::setTabOrder(coils_7, disc_7);
        QWidget::setTabOrder(disc_7, inReg_7);
        QWidget::setTabOrder(inReg_7, holdReg_7);
        QWidget::setTabOrder(holdReg_7, coils_8);
        QWidget::setTabOrder(coils_8, disc_8);
        QWidget::setTabOrder(disc_8, inReg_8);
        QWidget::setTabOrder(inReg_8, holdReg_8);
        QWidget::setTabOrder(holdReg_8, coils_9);
        QWidget::setTabOrder(coils_9, disc_9);
        QWidget::setTabOrder(disc_9, inReg_9);
        QWidget::setTabOrder(inReg_9, holdReg_9);

        menuBar->addAction(menu_Device->menuAction());
        menuBar->addAction(menuToo_ls->menuAction());
        menu_Device->addAction(actionConnect);
        menu_Device->addAction(actionDisconnect);
        menu_Device->addSeparator();
        menu_Device->addAction(actionExit);
        menuToo_ls->addAction(actionOptions);

        retranslateUi(MainWindow);

        connectButton->setDefault(true);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Modbus Server Example", nullptr));
        actionConnect->setText(QCoreApplication::translate("MainWindow", "&Connect", nullptr));
        actionDisconnect->setText(QCoreApplication::translate("MainWindow", "&Disconnect", nullptr));
        actionExit->setText(QCoreApplication::translate("MainWindow", "&Quit", nullptr));
        actionOptions->setText(QCoreApplication::translate("MainWindow", "&Options", nullptr));
        label_27->setText(QCoreApplication::translate("MainWindow", "Server Address:", nullptr));
        connectButton->setText(QCoreApplication::translate("MainWindow", "Connect", nullptr));
        connectType->setItemText(0, QCoreApplication::translate("MainWindow", "Serial", nullptr));
        connectType->setItemText(1, QCoreApplication::translate("MainWindow", "TCP", nullptr));

        label_2->setText(QCoreApplication::translate("MainWindow", "Port:", nullptr));
        label->setText(QCoreApplication::translate("MainWindow", "Connection type:", nullptr));
        setBusyBox->setText(QCoreApplication::translate("MainWindow", "Set server busy", nullptr));
        listenOnlyBox->setText(QCoreApplication::translate("MainWindow", "Set server listen only", nullptr));
        disc_2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        label_23->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        holdReg_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        coils_3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        inReg_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        disc_3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        label_11->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        inReg_3->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        coils_4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        holdReg_3->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        disc_4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        inReg_4->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        holdReg_4->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        coils_5->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        inReg_5->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        disc_5->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        label_12->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        holdReg_5->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        coils_6->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        holdReg_0->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        coils_1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        disc_1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        label_22->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        inReg_1->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        holdReg_1->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        coils_2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        inReg_9->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        label_16->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        disc_9->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        holdReg_9->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        inReg_6->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        disc_6->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        holdReg_6->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        coils_7->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        label_13->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        disc_7->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        inReg_7->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        label_14->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        holdReg_7->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        coils_8->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        disc_8->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        label_15->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        inReg_8->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        coils_9->setText(QCoreApplication::translate("MainWindow", "9", nullptr));
        holdReg_8->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        discreteInputs->setText(QCoreApplication::translate("MainWindow", "Discrete Inputs", nullptr));
        inputRegisters->setText(QCoreApplication::translate("MainWindow", "Input Registers", nullptr));
        coils->setText(QCoreApplication::translate("MainWindow", "Coils", nullptr));
        address->setText(QCoreApplication::translate("MainWindow", "Address", nullptr));
        holdingRegisters->setText(QCoreApplication::translate("MainWindow", "Holding Registers", nullptr));
        coils_0->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        disc_0->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        inReg_0->setPlaceholderText(QCoreApplication::translate("MainWindow", "Hexadecimal A-F, a-f, 0-9.", nullptr));
        label_21->setText(QCoreApplication::translate("MainWindow", "0", nullptr));
        menu_Device->setTitle(QCoreApplication::translate("MainWindow", "&Device", nullptr));
        menuToo_ls->setTitle(QCoreApplication::translate("MainWindow", "Too&ls", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
