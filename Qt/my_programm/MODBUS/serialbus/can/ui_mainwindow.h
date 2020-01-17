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
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <sendframebox.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionConnect;
    QAction *actionDisconnect;
    QAction *actionQuit;
    QAction *actionAboutQt;
    QAction *actionClearLog;
    QAction *actionPluginDocumentation;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    SendFrameBox *sendFrameBox;
    QGroupBox *receivedMessagesBox;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_3;
    QTextEdit *receivedMessagesEdit;
    QMenuBar *menuBar;
    QMenu *menuCalls;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(551, 481);
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
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QString::fromUtf8("actionQuit"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/images/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuit->setIcon(icon2);
        actionAboutQt = new QAction(MainWindow);
        actionAboutQt->setObjectName(QString::fromUtf8("actionAboutQt"));
        actionClearLog = new QAction(MainWindow);
        actionClearLog->setObjectName(QString::fromUtf8("actionClearLog"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/images/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClearLog->setIcon(icon3);
        actionPluginDocumentation = new QAction(MainWindow);
        actionPluginDocumentation->setObjectName(QString::fromUtf8("actionPluginDocumentation"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        sendFrameBox = new SendFrameBox(centralWidget);
        sendFrameBox->setObjectName(QString::fromUtf8("sendFrameBox"));

        verticalLayout->addWidget(sendFrameBox);

        receivedMessagesBox = new QGroupBox(centralWidget);
        receivedMessagesBox->setObjectName(QString::fromUtf8("receivedMessagesBox"));
        verticalLayout_4 = new QVBoxLayout(receivedMessagesBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        label_3 = new QLabel(receivedMessagesBox);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        QFont font;
        font.setFamily(QString::fromUtf8("Courier"));
        font.setKerning(false);
        label_3->setFont(font);
        label_3->setFrameShape(QFrame::StyledPanel);

        verticalLayout_4->addWidget(label_3);

        receivedMessagesEdit = new QTextEdit(receivedMessagesBox);
        receivedMessagesEdit->setObjectName(QString::fromUtf8("receivedMessagesEdit"));
        receivedMessagesEdit->setFont(font);
        receivedMessagesEdit->setFrameShape(QFrame::StyledPanel);
        receivedMessagesEdit->setUndoRedoEnabled(false);
        receivedMessagesEdit->setLineWrapMode(QTextEdit::NoWrap);
        receivedMessagesEdit->setReadOnly(true);

        verticalLayout_4->addWidget(receivedMessagesEdit);


        verticalLayout->addWidget(receivedMessagesBox);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 551, 19));
        menuCalls = new QMenu(menuBar);
        menuCalls->setObjectName(QString::fromUtf8("menuCalls"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QString::fromUtf8("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuCalls->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menuCalls->addAction(actionConnect);
        menuCalls->addAction(actionDisconnect);
        menuCalls->addSeparator();
        menuCalls->addAction(actionClearLog);
        menuCalls->addSeparator();
        menuCalls->addAction(actionQuit);
        menuHelp->addAction(actionPluginDocumentation);
        menuHelp->addAction(actionAboutQt);
        mainToolBar->addAction(actionConnect);
        mainToolBar->addAction(actionDisconnect);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionClearLog);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "CAN Example", nullptr));
        actionConnect->setText(QCoreApplication::translate("MainWindow", "&Connect", nullptr));
        actionDisconnect->setText(QCoreApplication::translate("MainWindow", "&Disconnect", nullptr));
        actionQuit->setText(QCoreApplication::translate("MainWindow", "&Quit", nullptr));
        actionAboutQt->setText(QCoreApplication::translate("MainWindow", "&About Qt", nullptr));
        actionClearLog->setText(QCoreApplication::translate("MainWindow", "Clear &Log", nullptr));
        actionPluginDocumentation->setText(QCoreApplication::translate("MainWindow", "Plugin Documentation", nullptr));
#if QT_CONFIG(tooltip)
        actionPluginDocumentation->setToolTip(QCoreApplication::translate("MainWindow", "Open plugin documentation in Webbrowser", nullptr));
#endif // QT_CONFIG(tooltip)
        sendFrameBox->setTitle(QCoreApplication::translate("MainWindow", "Send CAN frame", nullptr));
        receivedMessagesBox->setTitle(QCoreApplication::translate("MainWindow", "Received CAN messages", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Timestamp        Flags  CAN-ID   DLC  Data", nullptr));
        menuCalls->setTitle(QCoreApplication::translate("MainWindow", "&Calls", nullptr));
        menuHelp->setTitle(QCoreApplication::translate("MainWindow", "&Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
