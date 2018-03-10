/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
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
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(551, 481);
        actionConnect = new QAction(MainWindow);
        actionConnect->setObjectName(QStringLiteral("actionConnect"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/connect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionConnect->setIcon(icon);
        actionDisconnect = new QAction(MainWindow);
        actionDisconnect->setObjectName(QStringLiteral("actionDisconnect"));
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/images/disconnect.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionDisconnect->setIcon(icon1);
        actionQuit = new QAction(MainWindow);
        actionQuit->setObjectName(QStringLiteral("actionQuit"));
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/images/application-exit.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionQuit->setIcon(icon2);
        actionAboutQt = new QAction(MainWindow);
        actionAboutQt->setObjectName(QStringLiteral("actionAboutQt"));
        actionClearLog = new QAction(MainWindow);
        actionClearLog->setObjectName(QStringLiteral("actionClearLog"));
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/images/clear.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionClearLog->setIcon(icon3);
        actionPluginDocumentation = new QAction(MainWindow);
        actionPluginDocumentation->setObjectName(QStringLiteral("actionPluginDocumentation"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        sendFrameBox = new SendFrameBox(centralWidget);
        sendFrameBox->setObjectName(QStringLiteral("sendFrameBox"));

        verticalLayout->addWidget(sendFrameBox);

        receivedMessagesBox = new QGroupBox(centralWidget);
        receivedMessagesBox->setObjectName(QStringLiteral("receivedMessagesBox"));
        verticalLayout_4 = new QVBoxLayout(receivedMessagesBox);
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setContentsMargins(11, 11, 11, 11);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_3 = new QLabel(receivedMessagesBox);
        label_3->setObjectName(QStringLiteral("label_3"));
        QFont font;
        font.setFamily(QStringLiteral("Courier"));
        font.setKerning(false);
        label_3->setFont(font);
        label_3->setFrameShape(QFrame::StyledPanel);

        verticalLayout_4->addWidget(label_3);

        receivedMessagesEdit = new QTextEdit(receivedMessagesBox);
        receivedMessagesEdit->setObjectName(QStringLiteral("receivedMessagesEdit"));
        receivedMessagesEdit->setFont(font);
        receivedMessagesEdit->setFrameShape(QFrame::StyledPanel);
        receivedMessagesEdit->setUndoRedoEnabled(false);
        receivedMessagesEdit->setLineWrapMode(QTextEdit::NoWrap);
        receivedMessagesEdit->setReadOnly(true);

        verticalLayout_4->addWidget(receivedMessagesEdit);


        verticalLayout->addWidget(receivedMessagesBox);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 551, 19));
        menuCalls = new QMenu(menuBar);
        menuCalls->setObjectName(QStringLiteral("menuCalls"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
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
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "CAN Example", nullptr));
        actionConnect->setText(QApplication::translate("MainWindow", "&Connect", nullptr));
        actionDisconnect->setText(QApplication::translate("MainWindow", "&Disconnect", nullptr));
        actionQuit->setText(QApplication::translate("MainWindow", "&Quit", nullptr));
        actionAboutQt->setText(QApplication::translate("MainWindow", "&About Qt", nullptr));
        actionClearLog->setText(QApplication::translate("MainWindow", "Clear &Log", nullptr));
        actionPluginDocumentation->setText(QApplication::translate("MainWindow", "Plugin Documentation", nullptr));
#ifndef QT_NO_TOOLTIP
        actionPluginDocumentation->setToolTip(QApplication::translate("MainWindow", "Open plugin documentation in Webbrowser", nullptr));
#endif // QT_NO_TOOLTIP
        sendFrameBox->setTitle(QApplication::translate("MainWindow", "Send CAN frame", nullptr));
        receivedMessagesBox->setTitle(QApplication::translate("MainWindow", "Received CAN messages", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "Timestamp        Flags  CAN-ID   DLC  Data", nullptr));
        menuCalls->setTitle(QApplication::translate("MainWindow", "&Calls", nullptr));
        menuHelp->setTitle(QApplication::translate("MainWindow", "&Help", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
