/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTreeView>
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
    QSpacerItem *horizontalSpacer;
    QSpinBox *serverEdit;
    QComboBox *connectType;
    QLabel *label_2;
    QLabel *label;
    QLineEdit *portEdit;
    QHBoxLayout *horizontalLayout_2;
    QGroupBox *groupBox_2;
    QGridLayout *gridLayout_3;
    QLabel *label_4;
    QSpinBox *readAddress;
    QLabel *label_5;
    QComboBox *readSize;
    QLabel *label_9;
    QListWidget *readValue;
    QGroupBox *writeGroupBox;
    QGridLayout *gridLayout_2;
    QLabel *label_7;
    QTreeView *writeValueTable;
    QSpinBox *writeAddress;
    QLabel *label_8;
    QComboBox *writeSize;
    QLabel *label_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label_6;
    QComboBox *writeTable;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *readButton;
    QPushButton *writeButton;
    QPushButton *readWriteButton;
    QStatusBar *statusBar;
    QMenuBar *menuBar;
    QMenu *menuDevice;
    QMenu *menuToo_ls;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(574, 630);
        MainWindow->setMaximumSize(QSize(16777215, 1000));
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

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        gridLayout->addItem(horizontalSpacer, 0, 4, 1, 1);

        serverEdit = new QSpinBox(centralWidget);
        serverEdit->setObjectName(QString::fromUtf8("serverEdit"));
        sizePolicy1.setHeightForWidth(serverEdit->sizePolicy().hasHeightForWidth());
        serverEdit->setSizePolicy(sizePolicy1);
        serverEdit->setMinimum(1);
        serverEdit->setMaximum(247);

        gridLayout->addWidget(serverEdit, 0, 6, 1, 1);

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

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        groupBox_2 = new QGroupBox(centralWidget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setMinimumSize(QSize(250, 0));
        gridLayout_3 = new QGridLayout(groupBox_2);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QString::fromUtf8("gridLayout_3"));
        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        gridLayout_3->addWidget(label_4, 0, 0, 1, 1);

        readAddress = new QSpinBox(groupBox_2);
        readAddress->setObjectName(QString::fromUtf8("readAddress"));
        readAddress->setMaximum(9);

        gridLayout_3->addWidget(readAddress, 0, 1, 1, 1);

        label_5 = new QLabel(groupBox_2);
        label_5->setObjectName(QString::fromUtf8("label_5"));

        gridLayout_3->addWidget(label_5, 1, 0, 1, 1);

        readSize = new QComboBox(groupBox_2);
        readSize->addItem(QString());
        readSize->addItem(QString());
        readSize->addItem(QString());
        readSize->addItem(QString());
        readSize->addItem(QString());
        readSize->addItem(QString());
        readSize->addItem(QString());
        readSize->addItem(QString());
        readSize->addItem(QString());
        readSize->addItem(QString());
        readSize->setObjectName(QString::fromUtf8("readSize"));

        gridLayout_3->addWidget(readSize, 1, 1, 1, 1);

        label_9 = new QLabel(groupBox_2);
        label_9->setObjectName(QString::fromUtf8("label_9"));

        gridLayout_3->addWidget(label_9, 2, 0, 1, 1);

        readValue = new QListWidget(groupBox_2);
        readValue->setObjectName(QString::fromUtf8("readValue"));
        readValue->setMinimumSize(QSize(0, 0));

        gridLayout_3->addWidget(readValue, 3, 0, 1, 2);


        horizontalLayout_2->addWidget(groupBox_2);

        writeGroupBox = new QGroupBox(centralWidget);
        writeGroupBox->setObjectName(QString::fromUtf8("writeGroupBox"));
        writeGroupBox->setMinimumSize(QSize(225, 0));
        gridLayout_2 = new QGridLayout(writeGroupBox);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        label_7 = new QLabel(writeGroupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));

        gridLayout_2->addWidget(label_7, 0, 0, 1, 1);

        writeValueTable = new QTreeView(writeGroupBox);
        writeValueTable->setObjectName(QString::fromUtf8("writeValueTable"));
        writeValueTable->setProperty("showDropIndicator", QVariant(true));
        writeValueTable->setAlternatingRowColors(true);
        writeValueTable->setRootIsDecorated(false);
        writeValueTable->setUniformRowHeights(true);
        writeValueTable->setItemsExpandable(false);
        writeValueTable->setExpandsOnDoubleClick(false);
        writeValueTable->header()->setVisible(true);

        gridLayout_2->addWidget(writeValueTable, 3, 0, 1, 2);

        writeAddress = new QSpinBox(writeGroupBox);
        writeAddress->setObjectName(QString::fromUtf8("writeAddress"));
        writeAddress->setMaximum(9);

        gridLayout_2->addWidget(writeAddress, 0, 1, 1, 1);

        label_8 = new QLabel(writeGroupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));

        gridLayout_2->addWidget(label_8, 1, 0, 1, 1);

        writeSize = new QComboBox(writeGroupBox);
        writeSize->addItem(QString());
        writeSize->addItem(QString());
        writeSize->addItem(QString());
        writeSize->addItem(QString());
        writeSize->addItem(QString());
        writeSize->addItem(QString());
        writeSize->addItem(QString());
        writeSize->addItem(QString());
        writeSize->addItem(QString());
        writeSize->addItem(QString());
        writeSize->setObjectName(QString::fromUtf8("writeSize"));

        gridLayout_2->addWidget(writeSize, 1, 1, 1, 1);

        label_3 = new QLabel(writeGroupBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        gridLayout_2->addWidget(label_3, 2, 0, 1, 1);


        horizontalLayout_2->addWidget(writeGroupBox);


        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label_6 = new QLabel(centralWidget);
        label_6->setObjectName(QString::fromUtf8("label_6"));

        horizontalLayout->addWidget(label_6);

        writeTable = new QComboBox(centralWidget);
        writeTable->setObjectName(QString::fromUtf8("writeTable"));

        horizontalLayout->addWidget(writeTable);

        horizontalSpacer_2 = new QSpacerItem(13, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        readButton = new QPushButton(centralWidget);
        readButton->setObjectName(QString::fromUtf8("readButton"));
        sizePolicy1.setHeightForWidth(readButton->sizePolicy().hasHeightForWidth());
        readButton->setSizePolicy(sizePolicy1);

        horizontalLayout->addWidget(readButton);

        writeButton = new QPushButton(centralWidget);
        writeButton->setObjectName(QString::fromUtf8("writeButton"));

        horizontalLayout->addWidget(writeButton);

        readWriteButton = new QPushButton(centralWidget);
        readWriteButton->setObjectName(QString::fromUtf8("readWriteButton"));
        readWriteButton->setEnabled(false);

        horizontalLayout->addWidget(readWriteButton);


        verticalLayout->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 574, 24));
        menuDevice = new QMenu(menuBar);
        menuDevice->setObjectName(QString::fromUtf8("menuDevice"));
        menuToo_ls = new QMenu(menuBar);
        menuToo_ls->setObjectName(QString::fromUtf8("menuToo_ls"));
        MainWindow->setMenuBar(menuBar);
        QWidget::setTabOrder(connectType, portEdit);
        QWidget::setTabOrder(portEdit, serverEdit);
        QWidget::setTabOrder(serverEdit, connectButton);
        QWidget::setTabOrder(connectButton, readAddress);
        QWidget::setTabOrder(readAddress, readSize);
        QWidget::setTabOrder(readSize, readValue);
        QWidget::setTabOrder(readValue, writeAddress);
        QWidget::setTabOrder(writeAddress, writeSize);
        QWidget::setTabOrder(writeSize, writeValueTable);
        QWidget::setTabOrder(writeValueTable, writeTable);
        QWidget::setTabOrder(writeTable, readButton);
        QWidget::setTabOrder(readButton, writeButton);
        QWidget::setTabOrder(writeButton, readWriteButton);

        menuBar->addAction(menuDevice->menuAction());
        menuBar->addAction(menuToo_ls->menuAction());
        menuDevice->addAction(actionConnect);
        menuDevice->addAction(actionDisconnect);
        menuDevice->addSeparator();
        menuDevice->addAction(actionExit);
        menuToo_ls->addAction(actionOptions);

        retranslateUi(MainWindow);

        connectButton->setDefault(true);
        readSize->setCurrentIndex(9);
        writeSize->setCurrentIndex(9);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "Modbus Master Example", nullptr));
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
        groupBox_2->setTitle(QCoreApplication::translate("MainWindow", "Read", nullptr));
        label_4->setText(QCoreApplication::translate("MainWindow", "Start address:", nullptr));
        label_5->setText(QCoreApplication::translate("MainWindow", "Number of values:", nullptr));
        readSize->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        readSize->setItemText(1, QCoreApplication::translate("MainWindow", "2", nullptr));
        readSize->setItemText(2, QCoreApplication::translate("MainWindow", "3", nullptr));
        readSize->setItemText(3, QCoreApplication::translate("MainWindow", "4", nullptr));
        readSize->setItemText(4, QCoreApplication::translate("MainWindow", "5", nullptr));
        readSize->setItemText(5, QCoreApplication::translate("MainWindow", "6", nullptr));
        readSize->setItemText(6, QCoreApplication::translate("MainWindow", "7", nullptr));
        readSize->setItemText(7, QCoreApplication::translate("MainWindow", "8", nullptr));
        readSize->setItemText(8, QCoreApplication::translate("MainWindow", "9", nullptr));
        readSize->setItemText(9, QCoreApplication::translate("MainWindow", "10", nullptr));

        label_9->setText(QCoreApplication::translate("MainWindow", "Result:", nullptr));
        writeGroupBox->setTitle(QCoreApplication::translate("MainWindow", "Write", nullptr));
        label_7->setText(QCoreApplication::translate("MainWindow", "Start address:", nullptr));
        label_8->setText(QCoreApplication::translate("MainWindow", "Number of values:", nullptr));
        writeSize->setItemText(0, QCoreApplication::translate("MainWindow", "1", nullptr));
        writeSize->setItemText(1, QCoreApplication::translate("MainWindow", "2", nullptr));
        writeSize->setItemText(2, QCoreApplication::translate("MainWindow", "3", nullptr));
        writeSize->setItemText(3, QCoreApplication::translate("MainWindow", "4", nullptr));
        writeSize->setItemText(4, QCoreApplication::translate("MainWindow", "5", nullptr));
        writeSize->setItemText(5, QCoreApplication::translate("MainWindow", "6", nullptr));
        writeSize->setItemText(6, QCoreApplication::translate("MainWindow", "7", nullptr));
        writeSize->setItemText(7, QCoreApplication::translate("MainWindow", "8", nullptr));
        writeSize->setItemText(8, QCoreApplication::translate("MainWindow", "9", nullptr));
        writeSize->setItemText(9, QCoreApplication::translate("MainWindow", "10", nullptr));

        label_3->setText(QString());
        label_6->setText(QCoreApplication::translate("MainWindow", "Table:", nullptr));
        readButton->setText(QCoreApplication::translate("MainWindow", "Read", nullptr));
        writeButton->setText(QCoreApplication::translate("MainWindow", "Write", nullptr));
        readWriteButton->setText(QCoreApplication::translate("MainWindow", "Read-Write", nullptr));
        menuDevice->setTitle(QCoreApplication::translate("MainWindow", "&Device", nullptr));
        menuToo_ls->setTitle(QCoreApplication::translate("MainWindow", "Too&ls", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
