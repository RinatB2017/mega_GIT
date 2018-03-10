/********************************************************************************
** Form generated from reading UI file 'interface.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTERFACE_H
#define UI_INTERFACE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "plaintextedit.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionExit;
    QAction *actionClear;
    QAction *actionCopy;
    QAction *actionSelect_All;
    QWidget *centralwidget;
    QGridLayout *gridLayout_4;
    QGroupBox *settingsBox;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer;
    QSpacerItem *horizontalSpacer_4;
    QPushButton *connectButton;
    QWidget *settingsWidget;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout;
    QSpacerItem *horizontalSpacer;
    QLabel *timeoutLabel;
    QLabel *tcpPortLabel;
    QSpacerItem *horizontalSpacer_3;
    QLineEdit *tcpAddressEdit;
    QFrame *line;
    QLabel *tcpAddressLabel;
    QComboBox *serialPortCombo;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *tcpPortEdit;
    QLabel *serialPortLabel;
    QSpinBox *timeoutSpin;
    QRadioButton *serialRadio;
    QLabel *retriesLabel;
    QRadioButton *tcpRadio;
    QSpinBox *retriesSpin;
    QLabel *parityLabel;
    QLabel *dataBitsLabel;
    QLabel *stopBitsLabel;
    QLabel *baudRateLabel;
    QComboBox *parityCombo;
    QComboBox *dataBitsCombo;
    QComboBox *stopBitsCombo;
    QComboBox *baudRateCombo;
    QPushButton *disconnectButton;
    QStackedWidget *stackedWidget;
    QWidget *page;
    QGridLayout *gridLayout_8;
    QGroupBox *groupBox;
    QGridLayout *gridLayout_7;
    QLabel *addressLabel;
    QLabel *fcSerialLabel;
    QComboBox *fcSerialDrop;
    QLabel *pduSerialLabel;
    QLineEdit *pduSerialLine;
    QSpacerItem *verticalSpacer_7;
    QSpinBox *addressSpin;
    QWidget *page_2;
    QGridLayout *gridLayout_9;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_6;
    QGridLayout *gridLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_7;
    QLabel *tiLabel;
    QFrame *tiLine;
    QHBoxLayout *horizontalLayout_4;
    QLabel *ti1Label;
    QSpinBox *ti1Spin;
    QLabel *ti2Label;
    QSpinBox *ti2Spin;
    QSpacerItem *horizontalSpacer_6;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_6;
    QLabel *piLabel;
    QFrame *piLine;
    QHBoxLayout *horizontalLayout_5;
    QLabel *pi1Label;
    QSpinBox *pi1Spin;
    QLabel *pi2Label;
    QSpinBox *pi2Spin;
    QSpacerItem *verticalSpacer_3;
    QSpacerItem *verticalSpacer_4;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout_8;
    QLabel *lLabel;
    QFrame *lLine;
    QHBoxLayout *horizontalLayout_3;
    QLabel *l1Label;
    QSpinBox *l1Spin;
    QLabel *l2Label;
    QSpinBox *l2Spin;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout_9;
    QLabel *uiLabel;
    QFrame *line_3;
    QHBoxLayout *horizontalLayout_10;
    QLabel *ui1Label;
    QSpinBox *ui1Spin;
    QSpacerItem *verticalSpacer_5;
    QFrame *line_4;
    QSpacerItem *verticalSpacer_6;
    QGridLayout *gridLayout_5;
    QLabel *fcTcpLabel;
    QComboBox *fcTcpDrop;
    QLabel *pduTcpLabel;
    QLineEdit *pduTcpLine;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer_5;
    QPushButton *sendButton;
    QFrame *line_2;
    PlainTextEdit *logTextEdit;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuEdit;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(667, 465);
        MainWindow->setUnifiedTitleAndToolBarOnMac(false);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionClear = new QAction(MainWindow);
        actionClear->setObjectName(QStringLiteral("actionClear"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QStringLiteral("actionCopy"));
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QStringLiteral("actionSelect_All"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(centralwidget->sizePolicy().hasHeightForWidth());
        centralwidget->setSizePolicy(sizePolicy);
        gridLayout_4 = new QGridLayout(centralwidget);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        settingsBox = new QGroupBox(centralwidget);
        settingsBox->setObjectName(QStringLiteral("settingsBox"));
        gridLayout_2 = new QGridLayout(settingsBox);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 1, 0, 1, 1);

        horizontalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout_2->addItem(horizontalSpacer_4, 2, 0, 1, 1);

        connectButton = new QPushButton(settingsBox);
        connectButton->setObjectName(QStringLiteral("connectButton"));
        connectButton->setCheckable(false);
        connectButton->setChecked(false);

        gridLayout_2->addWidget(connectButton, 2, 1, 1, 1);

        settingsWidget = new QWidget(settingsBox);
        settingsWidget->setObjectName(QStringLiteral("settingsWidget"));
        horizontalLayout = new QHBoxLayout(settingsWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        gridLayout = new QGridLayout();
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        horizontalSpacer = new QSpacerItem(13, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 7, 0, 1, 1);

        timeoutLabel = new QLabel(settingsWidget);
        timeoutLabel->setObjectName(QStringLiteral("timeoutLabel"));
        sizePolicy.setHeightForWidth(timeoutLabel->sizePolicy().hasHeightForWidth());
        timeoutLabel->setSizePolicy(sizePolicy);
        timeoutLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(timeoutLabel, 11, 0, 1, 2);

        tcpPortLabel = new QLabel(settingsWidget);
        tcpPortLabel->setObjectName(QStringLiteral("tcpPortLabel"));
        tcpPortLabel->setEnabled(false);
        sizePolicy.setHeightForWidth(tcpPortLabel->sizePolicy().hasHeightForWidth());
        tcpPortLabel->setSizePolicy(sizePolicy);
        tcpPortLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(tcpPortLabel, 7, 1, 1, 1);

        horizontalSpacer_3 = new QSpacerItem(13, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_3, 1, 0, 1, 1);

        tcpAddressEdit = new QLineEdit(settingsWidget);
        tcpAddressEdit->setObjectName(QStringLiteral("tcpAddressEdit"));
        tcpAddressEdit->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Fixed);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tcpAddressEdit->sizePolicy().hasHeightForWidth());
        tcpAddressEdit->setSizePolicy(sizePolicy1);
        tcpAddressEdit->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(tcpAddressEdit, 8, 2, 1, 1);

        line = new QFrame(settingsWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setMinimumSize(QSize(0, 0));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 9, 0, 1, 3);

        tcpAddressLabel = new QLabel(settingsWidget);
        tcpAddressLabel->setObjectName(QStringLiteral("tcpAddressLabel"));
        tcpAddressLabel->setEnabled(false);
        sizePolicy.setHeightForWidth(tcpAddressLabel->sizePolicy().hasHeightForWidth());
        tcpAddressLabel->setSizePolicy(sizePolicy);
        tcpAddressLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(tcpAddressLabel, 8, 1, 1, 1);

        serialPortCombo = new QComboBox(settingsWidget);
        serialPortCombo->setObjectName(QStringLiteral("serialPortCombo"));

        gridLayout->addWidget(serialPortCombo, 1, 2, 1, 1);

        horizontalSpacer_2 = new QSpacerItem(13, 20, QSizePolicy::Maximum, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer_2, 8, 0, 1, 1);

        tcpPortEdit = new QLineEdit(settingsWidget);
        tcpPortEdit->setObjectName(QStringLiteral("tcpPortEdit"));
        tcpPortEdit->setEnabled(false);
        sizePolicy1.setHeightForWidth(tcpPortEdit->sizePolicy().hasHeightForWidth());
        tcpPortEdit->setSizePolicy(sizePolicy1);
        tcpPortEdit->setMinimumSize(QSize(0, 0));

        gridLayout->addWidget(tcpPortEdit, 7, 2, 1, 1);

        serialPortLabel = new QLabel(settingsWidget);
        serialPortLabel->setObjectName(QStringLiteral("serialPortLabel"));
        sizePolicy.setHeightForWidth(serialPortLabel->sizePolicy().hasHeightForWidth());
        serialPortLabel->setSizePolicy(sizePolicy);
        serialPortLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(serialPortLabel, 1, 1, 1, 1);

        timeoutSpin = new QSpinBox(settingsWidget);
        timeoutSpin->setObjectName(QStringLiteral("timeoutSpin"));
        sizePolicy1.setHeightForWidth(timeoutSpin->sizePolicy().hasHeightForWidth());
        timeoutSpin->setSizePolicy(sizePolicy1);
        timeoutSpin->setMinimumSize(QSize(0, 0));
        timeoutSpin->setMinimum(50);
        timeoutSpin->setMaximum(100000);
        timeoutSpin->setValue(1000);

        gridLayout->addWidget(timeoutSpin, 11, 2, 1, 1);

        serialRadio = new QRadioButton(settingsWidget);
        serialRadio->setObjectName(QStringLiteral("serialRadio"));
        QSizePolicy sizePolicy2(QSizePolicy::Maximum, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(serialRadio->sizePolicy().hasHeightForWidth());
        serialRadio->setSizePolicy(sizePolicy2);
        serialRadio->setChecked(true);

        gridLayout->addWidget(serialRadio, 0, 0, 1, 2);

        retriesLabel = new QLabel(settingsWidget);
        retriesLabel->setObjectName(QStringLiteral("retriesLabel"));
        retriesLabel->setEnabled(true);
        sizePolicy.setHeightForWidth(retriesLabel->sizePolicy().hasHeightForWidth());
        retriesLabel->setSizePolicy(sizePolicy);
        retriesLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(retriesLabel, 10, 0, 1, 2);

        tcpRadio = new QRadioButton(settingsWidget);
        tcpRadio->setObjectName(QStringLiteral("tcpRadio"));
        sizePolicy2.setHeightForWidth(tcpRadio->sizePolicy().hasHeightForWidth());
        tcpRadio->setSizePolicy(sizePolicy2);

        gridLayout->addWidget(tcpRadio, 6, 0, 1, 2);

        retriesSpin = new QSpinBox(settingsWidget);
        retriesSpin->setObjectName(QStringLiteral("retriesSpin"));
        retriesSpin->setEnabled(true);
        sizePolicy1.setHeightForWidth(retriesSpin->sizePolicy().hasHeightForWidth());
        retriesSpin->setSizePolicy(sizePolicy1);
        retriesSpin->setMinimumSize(QSize(0, 0));
        retriesSpin->setValue(3);

        gridLayout->addWidget(retriesSpin, 10, 2, 1, 1);

        parityLabel = new QLabel(settingsWidget);
        parityLabel->setObjectName(QStringLiteral("parityLabel"));

        gridLayout->addWidget(parityLabel, 2, 1, 1, 1);

        dataBitsLabel = new QLabel(settingsWidget);
        dataBitsLabel->setObjectName(QStringLiteral("dataBitsLabel"));

        gridLayout->addWidget(dataBitsLabel, 3, 1, 1, 1);

        stopBitsLabel = new QLabel(settingsWidget);
        stopBitsLabel->setObjectName(QStringLiteral("stopBitsLabel"));

        gridLayout->addWidget(stopBitsLabel, 4, 1, 1, 1);

        baudRateLabel = new QLabel(settingsWidget);
        baudRateLabel->setObjectName(QStringLiteral("baudRateLabel"));

        gridLayout->addWidget(baudRateLabel, 5, 1, 1, 1);

        parityCombo = new QComboBox(settingsWidget);
        parityCombo->addItem(QString());
        parityCombo->addItem(QString());
        parityCombo->addItem(QString());
        parityCombo->addItem(QString());
        parityCombo->addItem(QString());
        parityCombo->setObjectName(QStringLiteral("parityCombo"));

        gridLayout->addWidget(parityCombo, 2, 2, 1, 1);

        dataBitsCombo = new QComboBox(settingsWidget);
        dataBitsCombo->addItem(QString());
        dataBitsCombo->addItem(QString());
        dataBitsCombo->addItem(QString());
        dataBitsCombo->addItem(QString());
        dataBitsCombo->setObjectName(QStringLiteral("dataBitsCombo"));

        gridLayout->addWidget(dataBitsCombo, 3, 2, 1, 1);

        stopBitsCombo = new QComboBox(settingsWidget);
        stopBitsCombo->addItem(QString());
        stopBitsCombo->addItem(QString());
        stopBitsCombo->addItem(QString());
        stopBitsCombo->setObjectName(QStringLiteral("stopBitsCombo"));

        gridLayout->addWidget(stopBitsCombo, 4, 2, 1, 1);

        baudRateCombo = new QComboBox(settingsWidget);
        baudRateCombo->addItem(QString());
        baudRateCombo->addItem(QString());
        baudRateCombo->addItem(QString());
        baudRateCombo->addItem(QString());
        baudRateCombo->addItem(QString());
        baudRateCombo->addItem(QString());
        baudRateCombo->addItem(QString());
        baudRateCombo->addItem(QString());
        baudRateCombo->setObjectName(QStringLiteral("baudRateCombo"));

        gridLayout->addWidget(baudRateCombo, 5, 2, 1, 1);


        horizontalLayout->addLayout(gridLayout);


        gridLayout_2->addWidget(settingsWidget, 0, 0, 1, 3);

        disconnectButton = new QPushButton(settingsBox);
        disconnectButton->setObjectName(QStringLiteral("disconnectButton"));
        disconnectButton->setEnabled(false);

        gridLayout_2->addWidget(disconnectButton, 2, 2, 1, 1);


        gridLayout_4->addWidget(settingsBox, 0, 0, 2, 1);

        stackedWidget = new QStackedWidget(centralwidget);
        stackedWidget->setObjectName(QStringLiteral("stackedWidget"));
        page = new QWidget();
        page->setObjectName(QStringLiteral("page"));
        gridLayout_8 = new QGridLayout(page);
        gridLayout_8->setObjectName(QStringLiteral("gridLayout_8"));
        gridLayout_8->setContentsMargins(0, 0, 0, 0);
        groupBox = new QGroupBox(page);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        gridLayout_7 = new QGridLayout(groupBox);
        gridLayout_7->setObjectName(QStringLiteral("gridLayout_7"));
        addressLabel = new QLabel(groupBox);
        addressLabel->setObjectName(QStringLiteral("addressLabel"));
        sizePolicy.setHeightForWidth(addressLabel->sizePolicy().hasHeightForWidth());
        addressLabel->setSizePolicy(sizePolicy);
        addressLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_7->addWidget(addressLabel, 0, 0, 1, 1);

        fcSerialLabel = new QLabel(groupBox);
        fcSerialLabel->setObjectName(QStringLiteral("fcSerialLabel"));
        QSizePolicy sizePolicy3(QSizePolicy::Maximum, QSizePolicy::Preferred);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(fcSerialLabel->sizePolicy().hasHeightForWidth());
        fcSerialLabel->setSizePolicy(sizePolicy3);

        gridLayout_7->addWidget(fcSerialLabel, 1, 0, 1, 1);

        fcSerialDrop = new QComboBox(groupBox);
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->addItem(QString());
        fcSerialDrop->setObjectName(QStringLiteral("fcSerialDrop"));
        sizePolicy1.setHeightForWidth(fcSerialDrop->sizePolicy().hasHeightForWidth());
        fcSerialDrop->setSizePolicy(sizePolicy1);

        gridLayout_7->addWidget(fcSerialDrop, 1, 1, 1, 2);

        pduSerialLabel = new QLabel(groupBox);
        pduSerialLabel->setObjectName(QStringLiteral("pduSerialLabel"));
        sizePolicy3.setHeightForWidth(pduSerialLabel->sizePolicy().hasHeightForWidth());
        pduSerialLabel->setSizePolicy(sizePolicy3);

        gridLayout_7->addWidget(pduSerialLabel, 2, 0, 1, 1);

        pduSerialLine = new QLineEdit(groupBox);
        pduSerialLine->setObjectName(QStringLiteral("pduSerialLine"));

        gridLayout_7->addWidget(pduSerialLine, 2, 1, 1, 2);

        verticalSpacer_7 = new QSpacerItem(20, 163, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_7->addItem(verticalSpacer_7, 3, 1, 1, 1);

        addressSpin = new QSpinBox(groupBox);
        addressSpin->setObjectName(QStringLiteral("addressSpin"));
        sizePolicy1.setHeightForWidth(addressSpin->sizePolicy().hasHeightForWidth());
        addressSpin->setSizePolicy(sizePolicy1);
        addressSpin->setMinimumSize(QSize(0, 0));
        addressSpin->setMaximum(255);
        addressSpin->setValue(1);

        gridLayout_7->addWidget(addressSpin, 0, 1, 1, 2);


        gridLayout_8->addWidget(groupBox, 0, 0, 1, 1);

        stackedWidget->addWidget(page);
        page_2 = new QWidget();
        page_2->setObjectName(QStringLiteral("page_2"));
        gridLayout_9 = new QGridLayout(page_2);
        gridLayout_9->setObjectName(QStringLiteral("gridLayout_9"));
        gridLayout_9->setContentsMargins(0, 0, 0, 0);
        groupBox_2 = new QGroupBox(page_2);
        groupBox_2->setObjectName(QStringLiteral("groupBox_2"));
        gridLayout_6 = new QGridLayout(groupBox_2);
        gridLayout_6->setObjectName(QStringLiteral("gridLayout_6"));
        gridLayout_3 = new QGridLayout();
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        tiLabel = new QLabel(groupBox_2);
        tiLabel->setObjectName(QStringLiteral("tiLabel"));
        sizePolicy3.setHeightForWidth(tiLabel->sizePolicy().hasHeightForWidth());
        tiLabel->setSizePolicy(sizePolicy3);

        horizontalLayout_7->addWidget(tiLabel);

        tiLine = new QFrame(groupBox_2);
        tiLine->setObjectName(QStringLiteral("tiLine"));
        tiLine->setFrameShape(QFrame::HLine);
        tiLine->setFrameShadow(QFrame::Sunken);

        horizontalLayout_7->addWidget(tiLine);


        verticalLayout_2->addLayout(horizontalLayout_7);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        ti1Label = new QLabel(groupBox_2);
        ti1Label->setObjectName(QStringLiteral("ti1Label"));
        sizePolicy3.setHeightForWidth(ti1Label->sizePolicy().hasHeightForWidth());
        ti1Label->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(ti1Label);

        ti1Spin = new QSpinBox(groupBox_2);
        ti1Spin->setObjectName(QStringLiteral("ti1Spin"));
        ti1Spin->setMaximum(255);

        horizontalLayout_4->addWidget(ti1Spin);

        ti2Label = new QLabel(groupBox_2);
        ti2Label->setObjectName(QStringLiteral("ti2Label"));
        sizePolicy3.setHeightForWidth(ti2Label->sizePolicy().hasHeightForWidth());
        ti2Label->setSizePolicy(sizePolicy3);

        horizontalLayout_4->addWidget(ti2Label);

        ti2Spin = new QSpinBox(groupBox_2);
        ti2Spin->setObjectName(QStringLiteral("ti2Spin"));
        ti2Spin->setMaximum(255);

        horizontalLayout_4->addWidget(ti2Spin);


        verticalLayout_2->addLayout(horizontalLayout_4);


        gridLayout_3->addLayout(verticalLayout_2, 0, 0, 1, 1);

        horizontalSpacer_6 = new QSpacerItem(35, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        gridLayout_3->addItem(horizontalSpacer_6, 0, 1, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        piLabel = new QLabel(groupBox_2);
        piLabel->setObjectName(QStringLiteral("piLabel"));
        sizePolicy3.setHeightForWidth(piLabel->sizePolicy().hasHeightForWidth());
        piLabel->setSizePolicy(sizePolicy3);

        horizontalLayout_6->addWidget(piLabel);

        piLine = new QFrame(groupBox_2);
        piLine->setObjectName(QStringLiteral("piLine"));
        piLine->setFrameShape(QFrame::HLine);
        piLine->setFrameShadow(QFrame::Sunken);

        horizontalLayout_6->addWidget(piLine);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        pi1Label = new QLabel(groupBox_2);
        pi1Label->setObjectName(QStringLiteral("pi1Label"));
        sizePolicy3.setHeightForWidth(pi1Label->sizePolicy().hasHeightForWidth());
        pi1Label->setSizePolicy(sizePolicy3);

        horizontalLayout_5->addWidget(pi1Label);

        pi1Spin = new QSpinBox(groupBox_2);
        pi1Spin->setObjectName(QStringLiteral("pi1Spin"));
        pi1Spin->setMaximum(255);

        horizontalLayout_5->addWidget(pi1Spin);

        pi2Label = new QLabel(groupBox_2);
        pi2Label->setObjectName(QStringLiteral("pi2Label"));
        sizePolicy3.setHeightForWidth(pi2Label->sizePolicy().hasHeightForWidth());
        pi2Label->setSizePolicy(sizePolicy3);

        horizontalLayout_5->addWidget(pi2Label);

        pi2Spin = new QSpinBox(groupBox_2);
        pi2Spin->setObjectName(QStringLiteral("pi2Spin"));
        pi2Spin->setMaximum(255);

        horizontalLayout_5->addWidget(pi2Spin);


        verticalLayout->addLayout(horizontalLayout_5);


        gridLayout_3->addLayout(verticalLayout, 0, 2, 1, 1);

        verticalSpacer_3 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer_3, 1, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_3->addItem(verticalSpacer_4, 1, 2, 1, 1);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        lLabel = new QLabel(groupBox_2);
        lLabel->setObjectName(QStringLiteral("lLabel"));
        sizePolicy3.setHeightForWidth(lLabel->sizePolicy().hasHeightForWidth());
        lLabel->setSizePolicy(sizePolicy3);

        horizontalLayout_8->addWidget(lLabel);

        lLine = new QFrame(groupBox_2);
        lLine->setObjectName(QStringLiteral("lLine"));
        lLine->setFrameShape(QFrame::HLine);
        lLine->setFrameShadow(QFrame::Sunken);

        horizontalLayout_8->addWidget(lLine);


        verticalLayout_3->addLayout(horizontalLayout_8);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        l1Label = new QLabel(groupBox_2);
        l1Label->setObjectName(QStringLiteral("l1Label"));
        sizePolicy3.setHeightForWidth(l1Label->sizePolicy().hasHeightForWidth());
        l1Label->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(l1Label);

        l1Spin = new QSpinBox(groupBox_2);
        l1Spin->setObjectName(QStringLiteral("l1Spin"));
        l1Spin->setMaximum(255);

        horizontalLayout_3->addWidget(l1Spin);

        l2Label = new QLabel(groupBox_2);
        l2Label->setObjectName(QStringLiteral("l2Label"));
        sizePolicy3.setHeightForWidth(l2Label->sizePolicy().hasHeightForWidth());
        l2Label->setSizePolicy(sizePolicy3);

        horizontalLayout_3->addWidget(l2Label);

        l2Spin = new QSpinBox(groupBox_2);
        l2Spin->setObjectName(QStringLiteral("l2Spin"));
        l2Spin->setMaximum(255);

        horizontalLayout_3->addWidget(l2Spin);


        verticalLayout_3->addLayout(horizontalLayout_3);


        gridLayout_3->addLayout(verticalLayout_3, 2, 0, 1, 1);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        uiLabel = new QLabel(groupBox_2);
        uiLabel->setObjectName(QStringLiteral("uiLabel"));
        sizePolicy3.setHeightForWidth(uiLabel->sizePolicy().hasHeightForWidth());
        uiLabel->setSizePolicy(sizePolicy3);

        horizontalLayout_9->addWidget(uiLabel);

        line_3 = new QFrame(groupBox_2);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        horizontalLayout_9->addWidget(line_3);


        verticalLayout_4->addLayout(horizontalLayout_9);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        ui1Label = new QLabel(groupBox_2);
        ui1Label->setObjectName(QStringLiteral("ui1Label"));
        sizePolicy3.setHeightForWidth(ui1Label->sizePolicy().hasHeightForWidth());
        ui1Label->setSizePolicy(sizePolicy3);

        horizontalLayout_10->addWidget(ui1Label);

        ui1Spin = new QSpinBox(groupBox_2);
        ui1Spin->setObjectName(QStringLiteral("ui1Spin"));
        ui1Spin->setMaximum(255);
        ui1Spin->setValue(1);

        horizontalLayout_10->addWidget(ui1Spin);


        verticalLayout_4->addLayout(horizontalLayout_10);


        gridLayout_3->addLayout(verticalLayout_4, 2, 2, 1, 1);


        gridLayout_6->addLayout(gridLayout_3, 0, 0, 1, 3);

        verticalSpacer_5 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_6->addItem(verticalSpacer_5, 1, 0, 1, 1);

        line_4 = new QFrame(groupBox_2);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout_6->addWidget(line_4, 2, 0, 2, 3);

        verticalSpacer_6 = new QSpacerItem(20, 20, QSizePolicy::Minimum, QSizePolicy::Fixed);

        gridLayout_6->addItem(verticalSpacer_6, 3, 1, 1, 1);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));
        fcTcpLabel = new QLabel(groupBox_2);
        fcTcpLabel->setObjectName(QStringLiteral("fcTcpLabel"));
        sizePolicy3.setHeightForWidth(fcTcpLabel->sizePolicy().hasHeightForWidth());
        fcTcpLabel->setSizePolicy(sizePolicy3);

        gridLayout_5->addWidget(fcTcpLabel, 0, 0, 1, 1);

        fcTcpDrop = new QComboBox(groupBox_2);
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->addItem(QString());
        fcTcpDrop->setObjectName(QStringLiteral("fcTcpDrop"));
        sizePolicy1.setHeightForWidth(fcTcpDrop->sizePolicy().hasHeightForWidth());
        fcTcpDrop->setSizePolicy(sizePolicy1);

        gridLayout_5->addWidget(fcTcpDrop, 0, 1, 1, 1);

        pduTcpLabel = new QLabel(groupBox_2);
        pduTcpLabel->setObjectName(QStringLiteral("pduTcpLabel"));
        sizePolicy3.setHeightForWidth(pduTcpLabel->sizePolicy().hasHeightForWidth());
        pduTcpLabel->setSizePolicy(sizePolicy3);

        gridLayout_5->addWidget(pduTcpLabel, 1, 0, 1, 1);

        pduTcpLine = new QLineEdit(groupBox_2);
        pduTcpLine->setObjectName(QStringLiteral("pduTcpLine"));

        gridLayout_5->addWidget(pduTcpLine, 1, 1, 1, 1);


        gridLayout_6->addLayout(gridLayout_5, 4, 0, 1, 3);

        verticalSpacer_2 = new QSpacerItem(20, 3, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_6->addItem(verticalSpacer_2, 5, 2, 1, 1);


        gridLayout_9->addWidget(groupBox_2, 0, 0, 1, 1);

        stackedWidget->addWidget(page_2);

        gridLayout_4->addWidget(stackedWidget, 0, 1, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_5);

        sendButton = new QPushButton(centralwidget);
        sendButton->setObjectName(QStringLiteral("sendButton"));
        sendButton->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(sendButton->sizePolicy().hasHeightForWidth());
        sendButton->setSizePolicy(sizePolicy4);

        horizontalLayout_2->addWidget(sendButton);


        gridLayout_4->addLayout(horizontalLayout_2, 1, 1, 1, 1);

        line_2 = new QFrame(centralwidget);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_4->addWidget(line_2, 2, 0, 1, 2);

        logTextEdit = new PlainTextEdit(centralwidget);
        logTextEdit->setObjectName(QStringLiteral("logTextEdit"));
        logTextEdit->setUndoRedoEnabled(false);
        logTextEdit->setReadOnly(true);
        logTextEdit->setTextInteractionFlags(Qt::TextSelectableByKeyboard|Qt::TextSelectableByMouse);

        gridLayout_4->addWidget(logTextEdit, 3, 0, 1, 2);

        gridLayout_4->setRowStretch(3, 1);
        gridLayout_4->setColumnStretch(1, 1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 667, 21));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuEdit = new QMenu(menubar);
        menuEdit->setObjectName(QStringLiteral("menuEdit"));
        MainWindow->setMenuBar(menubar);
#ifndef QT_NO_SHORTCUT
        timeoutLabel->setBuddy(timeoutSpin);
        tcpPortLabel->setBuddy(tcpPortEdit);
        tcpAddressLabel->setBuddy(tcpAddressEdit);
        serialPortLabel->setBuddy(serialPortCombo);
        retriesLabel->setBuddy(retriesSpin);
        parityLabel->setBuddy(parityCombo);
        dataBitsLabel->setBuddy(dataBitsCombo);
        stopBitsLabel->setBuddy(stopBitsCombo);
        baudRateLabel->setBuddy(baudRateCombo);
        addressLabel->setBuddy(addressSpin);
        fcSerialLabel->setBuddy(fcSerialDrop);
        pduSerialLabel->setBuddy(pduSerialLine);
        ti1Label->setBuddy(ti1Spin);
        ti2Label->setBuddy(ti2Spin);
        pi1Label->setBuddy(pi1Spin);
        pi2Label->setBuddy(pi2Spin);
        l1Label->setBuddy(l1Spin);
        l2Label->setBuddy(l2Spin);
        ui1Label->setBuddy(ui1Spin);
        fcTcpLabel->setBuddy(fcTcpDrop);
        pduTcpLabel->setBuddy(pduTcpLine);
#endif // QT_NO_SHORTCUT
        QWidget::setTabOrder(serialRadio, serialPortCombo);
        QWidget::setTabOrder(serialPortCombo, parityCombo);
        QWidget::setTabOrder(parityCombo, dataBitsCombo);
        QWidget::setTabOrder(dataBitsCombo, stopBitsCombo);
        QWidget::setTabOrder(stopBitsCombo, baudRateCombo);
        QWidget::setTabOrder(baudRateCombo, tcpRadio);
        QWidget::setTabOrder(tcpRadio, tcpPortEdit);
        QWidget::setTabOrder(tcpPortEdit, tcpAddressEdit);
        QWidget::setTabOrder(tcpAddressEdit, retriesSpin);
        QWidget::setTabOrder(retriesSpin, timeoutSpin);
        QWidget::setTabOrder(timeoutSpin, connectButton);
        QWidget::setTabOrder(connectButton, disconnectButton);
        QWidget::setTabOrder(disconnectButton, addressSpin);
        QWidget::setTabOrder(addressSpin, fcSerialDrop);
        QWidget::setTabOrder(fcSerialDrop, pduSerialLine);
        QWidget::setTabOrder(pduSerialLine, ti1Spin);
        QWidget::setTabOrder(ti1Spin, ti2Spin);
        QWidget::setTabOrder(ti2Spin, pi1Spin);
        QWidget::setTabOrder(pi1Spin, pi2Spin);
        QWidget::setTabOrder(pi2Spin, l1Spin);
        QWidget::setTabOrder(l1Spin, l2Spin);
        QWidget::setTabOrder(l2Spin, ui1Spin);
        QWidget::setTabOrder(ui1Spin, fcTcpDrop);
        QWidget::setTabOrder(fcTcpDrop, pduTcpLine);
        QWidget::setTabOrder(pduTcpLine, sendButton);
        QWidget::setTabOrder(sendButton, logTextEdit);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuEdit->menuAction());
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionClear);
        menuEdit->addAction(actionCopy);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);

        retranslateUi(MainWindow);
        QObject::connect(tcpRadio, SIGNAL(toggled(bool)), tcpPortEdit, SLOT(setEnabled(bool)));
        QObject::connect(tcpRadio, SIGNAL(toggled(bool)), tcpAddressEdit, SLOT(setEnabled(bool)));
        QObject::connect(serialRadio, SIGNAL(toggled(bool)), serialPortLabel, SLOT(setEnabled(bool)));
        QObject::connect(tcpRadio, SIGNAL(toggled(bool)), tcpPortLabel, SLOT(setEnabled(bool)));
        QObject::connect(tcpRadio, SIGNAL(toggled(bool)), tcpAddressLabel, SLOT(setEnabled(bool)));
        QObject::connect(connectButton, SIGNAL(clicked(bool)), connectButton, SLOT(setEnabled(bool)));
        QObject::connect(connectButton, SIGNAL(clicked(bool)), disconnectButton, SLOT(setDisabled(bool)));
        QObject::connect(disconnectButton, SIGNAL(clicked(bool)), connectButton, SLOT(setDisabled(bool)));
        QObject::connect(disconnectButton, SIGNAL(clicked(bool)), disconnectButton, SLOT(setEnabled(bool)));
        QObject::connect(connectButton, SIGNAL(clicked(bool)), settingsWidget, SLOT(setEnabled(bool)));
        QObject::connect(disconnectButton, SIGNAL(clicked(bool)), settingsWidget, SLOT(setDisabled(bool)));
        QObject::connect(disconnectButton, SIGNAL(clicked(bool)), sendButton, SLOT(setEnabled(bool)));
        QObject::connect(connectButton, SIGNAL(clicked(bool)), sendButton, SLOT(setDisabled(bool)));
        QObject::connect(serialRadio, SIGNAL(toggled(bool)), serialPortCombo, SLOT(setEnabled(bool)));
        QObject::connect(serialRadio, SIGNAL(toggled(bool)), parityLabel, SLOT(setEnabled(bool)));
        QObject::connect(serialRadio, SIGNAL(toggled(bool)), dataBitsLabel, SLOT(setEnabled(bool)));
        QObject::connect(serialRadio, SIGNAL(toggled(bool)), stopBitsLabel, SLOT(setEnabled(bool)));
        QObject::connect(serialRadio, SIGNAL(toggled(bool)), baudRateLabel, SLOT(setEnabled(bool)));
        QObject::connect(serialRadio, SIGNAL(toggled(bool)), parityCombo, SLOT(setEnabled(bool)));
        QObject::connect(serialRadio, SIGNAL(toggled(bool)), dataBitsCombo, SLOT(setEnabled(bool)));
        QObject::connect(serialRadio, SIGNAL(toggled(bool)), stopBitsCombo, SLOT(setEnabled(bool)));
        QObject::connect(serialRadio, SIGNAL(toggled(bool)), baudRateCombo, SLOT(setEnabled(bool)));
        QObject::connect(actionClear, SIGNAL(triggered()), logTextEdit, SLOT(clear()));
        QObject::connect(actionSelect_All, SIGNAL(triggered()), logTextEdit, SLOT(selectAll()));
        QObject::connect(actionCopy, SIGNAL(triggered()), logTextEdit, SLOT(copy()));
        QObject::connect(actionSelect_All, SIGNAL(triggered()), logTextEdit, SLOT(setFocus()));

        parityCombo->setCurrentIndex(1);
        dataBitsCombo->setCurrentIndex(3);
        baudRateCombo->setCurrentIndex(4);
        stackedWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Modbus ADU Editor", nullptr));
        actionExit->setText(QApplication::translate("MainWindow", "Exit", nullptr));
#ifndef QT_NO_SHORTCUT
        actionExit->setShortcut(QApplication::translate("MainWindow", "Alt+F4", nullptr));
#endif // QT_NO_SHORTCUT
        actionClear->setText(QApplication::translate("MainWindow", "Clear", nullptr));
        actionCopy->setText(QApplication::translate("MainWindow", "Copy", nullptr));
#ifndef QT_NO_SHORTCUT
        actionCopy->setShortcut(QApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_NO_SHORTCUT
        actionSelect_All->setText(QApplication::translate("MainWindow", "Select All", nullptr));
#ifndef QT_NO_SHORTCUT
        actionSelect_All->setShortcut(QApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_NO_SHORTCUT
        settingsBox->setTitle(QString());
        connectButton->setText(QApplication::translate("MainWindow", "Connect", nullptr));
        timeoutLabel->setText(QApplication::translate("MainWindow", "Timeout (msec):", nullptr));
        tcpPortLabel->setText(QApplication::translate("MainWindow", "Port:", nullptr));
        tcpAddressEdit->setText(QApplication::translate("MainWindow", "127.0.0.1", nullptr));
        tcpAddressLabel->setText(QApplication::translate("MainWindow", "Address:", nullptr));
        tcpPortEdit->setText(QApplication::translate("MainWindow", "502", nullptr));
        serialPortLabel->setText(QApplication::translate("MainWindow", "Port:", nullptr));
        serialRadio->setText(QApplication::translate("MainWindow", "Serial", nullptr));
        retriesLabel->setText(QApplication::translate("MainWindow", "Number of retries:", nullptr));
        tcpRadio->setText(QApplication::translate("MainWindow", "TCP", nullptr));
        parityLabel->setText(QApplication::translate("MainWindow", "Parity:", nullptr));
        dataBitsLabel->setText(QApplication::translate("MainWindow", "Data Bits:", nullptr));
        stopBitsLabel->setText(QApplication::translate("MainWindow", "Stop Bits:", nullptr));
        baudRateLabel->setText(QApplication::translate("MainWindow", "BaudRate:", nullptr));
        parityCombo->setItemText(0, QApplication::translate("MainWindow", "No", nullptr));
        parityCombo->setItemText(1, QApplication::translate("MainWindow", "Even", nullptr));
        parityCombo->setItemText(2, QApplication::translate("MainWindow", "Odd", nullptr));
        parityCombo->setItemText(3, QApplication::translate("MainWindow", "Space", nullptr));
        parityCombo->setItemText(4, QApplication::translate("MainWindow", "Mark", nullptr));

        dataBitsCombo->setItemText(0, QApplication::translate("MainWindow", "5", nullptr));
        dataBitsCombo->setItemText(1, QApplication::translate("MainWindow", "6", nullptr));
        dataBitsCombo->setItemText(2, QApplication::translate("MainWindow", "7", nullptr));
        dataBitsCombo->setItemText(3, QApplication::translate("MainWindow", "8", nullptr));

        stopBitsCombo->setItemText(0, QApplication::translate("MainWindow", "1", nullptr));
        stopBitsCombo->setItemText(1, QApplication::translate("MainWindow", "3", nullptr));
        stopBitsCombo->setItemText(2, QApplication::translate("MainWindow", "2", nullptr));

        baudRateCombo->setItemText(0, QApplication::translate("MainWindow", "1200", nullptr));
        baudRateCombo->setItemText(1, QApplication::translate("MainWindow", "2400", nullptr));
        baudRateCombo->setItemText(2, QApplication::translate("MainWindow", "4800", nullptr));
        baudRateCombo->setItemText(3, QApplication::translate("MainWindow", "9600", nullptr));
        baudRateCombo->setItemText(4, QApplication::translate("MainWindow", "19200", nullptr));
        baudRateCombo->setItemText(5, QApplication::translate("MainWindow", "38400", nullptr));
        baudRateCombo->setItemText(6, QApplication::translate("MainWindow", "57600", nullptr));
        baudRateCombo->setItemText(7, QApplication::translate("MainWindow", "115200", nullptr));

        disconnectButton->setText(QApplication::translate("MainWindow", "Disconnect", nullptr));
        groupBox->setTitle(QString());
        addressLabel->setText(QApplication::translate("MainWindow", "Server address:", nullptr));
        fcSerialLabel->setText(QApplication::translate("MainWindow", "Function code:", nullptr));
        fcSerialDrop->setItemText(0, QApplication::translate("MainWindow", "Type your custom function code and data below", nullptr));
        fcSerialDrop->setItemText(1, QApplication::translate("MainWindow", "0x01 - ReadCoils", nullptr));
        fcSerialDrop->setItemText(2, QApplication::translate("MainWindow", "0x02 - Read Discrete Inputs", nullptr));
        fcSerialDrop->setItemText(3, QApplication::translate("MainWindow", "0x03 - Read Holding Registers", nullptr));
        fcSerialDrop->setItemText(4, QApplication::translate("MainWindow", "0x04 - Read Input Registers", nullptr));
        fcSerialDrop->setItemText(5, QApplication::translate("MainWindow", "0x05 - Write Single Coil", nullptr));
        fcSerialDrop->setItemText(6, QApplication::translate("MainWindow", "0x06 - Write Single Register", nullptr));
        fcSerialDrop->setItemText(7, QApplication::translate("MainWindow", "0x07 - Read Exception Status", nullptr));
        fcSerialDrop->setItemText(8, QApplication::translate("MainWindow", "0x08 - Diagnostics", nullptr));
        fcSerialDrop->setItemText(9, QApplication::translate("MainWindow", "0x0B - Get Comm Event Counter", nullptr));
        fcSerialDrop->setItemText(10, QApplication::translate("MainWindow", "0x0C - Get Comm Event Log", nullptr));
        fcSerialDrop->setItemText(11, QApplication::translate("MainWindow", "0x0F - Write Multiple Coils", nullptr));
        fcSerialDrop->setItemText(12, QApplication::translate("MainWindow", "0x10 - Write Multiple Registers", nullptr));
        fcSerialDrop->setItemText(13, QApplication::translate("MainWindow", "0x11 - Report Server Id", nullptr));
        fcSerialDrop->setItemText(14, QApplication::translate("MainWindow", "0x14 - Read File Record", nullptr));
        fcSerialDrop->setItemText(15, QApplication::translate("MainWindow", "0x15 - Write File Record", nullptr));
        fcSerialDrop->setItemText(16, QApplication::translate("MainWindow", "0x16 - Mask Write Register", nullptr));
        fcSerialDrop->setItemText(17, QApplication::translate("MainWindow", "0x17 - Read/Write Multiple Registers", nullptr));
        fcSerialDrop->setItemText(18, QApplication::translate("MainWindow", "0x18 - Read Fifo Queue", nullptr));
        fcSerialDrop->setItemText(19, QApplication::translate("MainWindow", "0x2B - Encapsulated Interface Transport", nullptr));

        pduSerialLabel->setText(QApplication::translate("MainWindow", "Data:", nullptr));
        pduSerialLine->setText(QString());
        pduSerialLine->setPlaceholderText(QApplication::translate("MainWindow", "Hexadecimal representation of the data in big-endian byte order.", nullptr));
        addressSpin->setSpecialValueText(QApplication::translate("MainWindow", "broadcast", nullptr));
        groupBox_2->setTitle(QString());
        tiLabel->setText(QApplication::translate("MainWindow", "Transaction identifier", nullptr));
        ti1Label->setText(QApplication::translate("MainWindow", "1. Byte:", nullptr));
        ti2Label->setText(QApplication::translate("MainWindow", "2. Byte:", nullptr));
        piLabel->setText(QApplication::translate("MainWindow", "Protocol identifier", nullptr));
        pi1Label->setText(QApplication::translate("MainWindow", "1. Byte:", nullptr));
        pi2Label->setText(QApplication::translate("MainWindow", "2. Byte:", nullptr));
        lLabel->setText(QApplication::translate("MainWindow", "Length", nullptr));
        l1Label->setText(QApplication::translate("MainWindow", "1. Byte:", nullptr));
        l2Label->setText(QApplication::translate("MainWindow", "2. Byte:", nullptr));
        uiLabel->setText(QApplication::translate("MainWindow", "Unit identifier", nullptr));
        ui1Label->setText(QApplication::translate("MainWindow", "1. Byte:", nullptr));
        ui1Spin->setSpecialValueText(QApplication::translate("MainWindow", "broadcast", nullptr));
        fcTcpLabel->setText(QApplication::translate("MainWindow", "Function code:", nullptr));
        fcTcpDrop->setItemText(0, QApplication::translate("MainWindow", "Type your custom function code and data below", nullptr));
        fcTcpDrop->setItemText(1, QApplication::translate("MainWindow", "0x01 - ReadCoils", nullptr));
        fcTcpDrop->setItemText(2, QApplication::translate("MainWindow", "0x02 - Read Discrete Inputs", nullptr));
        fcTcpDrop->setItemText(3, QApplication::translate("MainWindow", "0x03 - Read Holding Registers", nullptr));
        fcTcpDrop->setItemText(4, QApplication::translate("MainWindow", "0x04 - Read Input Registers", nullptr));
        fcTcpDrop->setItemText(5, QApplication::translate("MainWindow", "0x05 - Write Single Coil", nullptr));
        fcTcpDrop->setItemText(6, QApplication::translate("MainWindow", "0x06 - Write Single Register", nullptr));
        fcTcpDrop->setItemText(7, QApplication::translate("MainWindow", "0x07 - Read Exception Status", nullptr));
        fcTcpDrop->setItemText(8, QApplication::translate("MainWindow", "0x08 - Diagnostics", nullptr));
        fcTcpDrop->setItemText(9, QApplication::translate("MainWindow", "0x0B - Get Comm Event Counter", nullptr));
        fcTcpDrop->setItemText(10, QApplication::translate("MainWindow", "0x0C - Get Comm Event Log", nullptr));
        fcTcpDrop->setItemText(11, QApplication::translate("MainWindow", "0x0F - Write Multiple Coils", nullptr));
        fcTcpDrop->setItemText(12, QApplication::translate("MainWindow", "0x10 - Write Multiple Registers", nullptr));
        fcTcpDrop->setItemText(13, QApplication::translate("MainWindow", "0x11 - Report Server Id", nullptr));
        fcTcpDrop->setItemText(14, QApplication::translate("MainWindow", "0x14 - Read File Record", nullptr));
        fcTcpDrop->setItemText(15, QApplication::translate("MainWindow", "0x15 - Write File Record", nullptr));
        fcTcpDrop->setItemText(16, QApplication::translate("MainWindow", "0x16 - Mask Write Register", nullptr));
        fcTcpDrop->setItemText(17, QApplication::translate("MainWindow", "0x17 - Read/Write Multiple Registers", nullptr));
        fcTcpDrop->setItemText(18, QApplication::translate("MainWindow", "0x18 - Read Fifo Queue", nullptr));
        fcTcpDrop->setItemText(19, QApplication::translate("MainWindow", "0x2B - Encapsulated Interface Transport", nullptr));

        pduTcpLabel->setText(QApplication::translate("MainWindow", "Data:", nullptr));
        pduTcpLine->setText(QString());
        pduTcpLine->setPlaceholderText(QApplication::translate("MainWindow", "Hexadecimal representation of the data in big-endian byte order.", nullptr));
        sendButton->setText(QApplication::translate("MainWindow", "Send", nullptr));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QApplication::translate("MainWindow", "Edit", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTERFACE_H
