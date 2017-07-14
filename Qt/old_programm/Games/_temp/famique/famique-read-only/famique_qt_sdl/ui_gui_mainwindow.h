/********************************************************************************
** Form generated from reading UI file 'gui_mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUI_MAINWINDOW_H
#define UI_GUI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_GUI_MainWindow
{
public:
    QAction *action_Load_ROM;
    QAction *action_Exit;
    QAction *action_Start_CPU;
    QAction *action_Halt_CPU;
    QAction *action;
    QAction *actionView_PPU;
    QAction *actionAuto;
    QAction *actionCPU;
    QAction *actionPPU;
    QAction *actionReset_wrappers;
    QAction *actionGo_fullscreen;
    QAction *actionEnableSound;
    QAction *actionTurn_on;
    QAction *actionSoft_reset;
    QAction *actionHard_reset;
    QAction *actionConfigure;
    QAction *actionBlah;
    QAction *actionDump_stac;
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuEmulation;
    QMenu *menuReset_CPU;
    QMenu *menuDebug;
    QMenu *menuVideo;
    QMenu *menuFrame_limiter_frameskip;
    QMenu *menuSound;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *GUI_MainWindow)
    {
        if (GUI_MainWindow->objectName().isEmpty())
            GUI_MainWindow->setObjectName(QStringLiteral("GUI_MainWindow"));
        GUI_MainWindow->resize(256, 280);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(GUI_MainWindow->sizePolicy().hasHeightForWidth());
        GUI_MainWindow->setSizePolicy(sizePolicy);
        GUI_MainWindow->setMinimumSize(QSize(256, 280));
        GUI_MainWindow->setMaximumSize(QSize(256, 280));
        action_Load_ROM = new QAction(GUI_MainWindow);
        action_Load_ROM->setObjectName(QStringLiteral("action_Load_ROM"));
        action_Exit = new QAction(GUI_MainWindow);
        action_Exit->setObjectName(QStringLiteral("action_Exit"));
        action_Start_CPU = new QAction(GUI_MainWindow);
        action_Start_CPU->setObjectName(QStringLiteral("action_Start_CPU"));
        action_Halt_CPU = new QAction(GUI_MainWindow);
        action_Halt_CPU->setObjectName(QStringLiteral("action_Halt_CPU"));
        action = new QAction(GUI_MainWindow);
        action->setObjectName(QStringLiteral("action"));
        action->setCheckable(true);
        actionView_PPU = new QAction(GUI_MainWindow);
        actionView_PPU->setObjectName(QStringLiteral("actionView_PPU"));
        actionAuto = new QAction(GUI_MainWindow);
        actionAuto->setObjectName(QStringLiteral("actionAuto"));
        actionCPU = new QAction(GUI_MainWindow);
        actionCPU->setObjectName(QStringLiteral("actionCPU"));
        actionPPU = new QAction(GUI_MainWindow);
        actionPPU->setObjectName(QStringLiteral("actionPPU"));
        actionReset_wrappers = new QAction(GUI_MainWindow);
        actionReset_wrappers->setObjectName(QStringLiteral("actionReset_wrappers"));
        actionGo_fullscreen = new QAction(GUI_MainWindow);
        actionGo_fullscreen->setObjectName(QStringLiteral("actionGo_fullscreen"));
        actionEnableSound = new QAction(GUI_MainWindow);
        actionEnableSound->setObjectName(QStringLiteral("actionEnableSound"));
        actionEnableSound->setCheckable(true);
        actionEnableSound->setChecked(true);
        actionTurn_on = new QAction(GUI_MainWindow);
        actionTurn_on->setObjectName(QStringLiteral("actionTurn_on"));
        actionSoft_reset = new QAction(GUI_MainWindow);
        actionSoft_reset->setObjectName(QStringLiteral("actionSoft_reset"));
        actionHard_reset = new QAction(GUI_MainWindow);
        actionHard_reset->setObjectName(QStringLiteral("actionHard_reset"));
        actionConfigure = new QAction(GUI_MainWindow);
        actionConfigure->setObjectName(QStringLiteral("actionConfigure"));
        actionBlah = new QAction(GUI_MainWindow);
        actionBlah->setObjectName(QStringLiteral("actionBlah"));
        actionDump_stac = new QAction(GUI_MainWindow);
        actionDump_stac->setObjectName(QStringLiteral("actionDump_stac"));
        centralWidget = new QWidget(GUI_MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        GUI_MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(GUI_MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 256, 22));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menuEmulation = new QMenu(menuBar);
        menuEmulation->setObjectName(QStringLiteral("menuEmulation"));
        menuReset_CPU = new QMenu(menuEmulation);
        menuReset_CPU->setObjectName(QStringLiteral("menuReset_CPU"));
        menuDebug = new QMenu(menuBar);
        menuDebug->setObjectName(QStringLiteral("menuDebug"));
        menuVideo = new QMenu(menuBar);
        menuVideo->setObjectName(QStringLiteral("menuVideo"));
        menuFrame_limiter_frameskip = new QMenu(menuVideo);
        menuFrame_limiter_frameskip->setObjectName(QStringLiteral("menuFrame_limiter_frameskip"));
        menuSound = new QMenu(menuBar);
        menuSound->setObjectName(QStringLiteral("menuSound"));
        GUI_MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(GUI_MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setEnabled(true);
        sizePolicy.setHeightForWidth(statusBar->sizePolicy().hasHeightForWidth());
        statusBar->setSizePolicy(sizePolicy);
        statusBar->setMinimumSize(QSize(256, 20));
        statusBar->setMaximumSize(QSize(256, 20));
        statusBar->setBaseSize(QSize(256, 18));
        QFont font;
        font.setPointSize(10);
        font.setBold(true);
        font.setWeight(75);
        statusBar->setFont(font);
        statusBar->setSizeGripEnabled(false);
        GUI_MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menuEmulation->menuAction());
        menuBar->addAction(menuVideo->menuAction());
        menuBar->addAction(menuSound->menuAction());
        menuBar->addAction(menuDebug->menuAction());
        menu_File->addAction(action_Load_ROM);
        menu_File->addAction(action_Exit);
        menuEmulation->addAction(action_Start_CPU);
        menuEmulation->addAction(action_Halt_CPU);
        menuEmulation->addAction(actionConfigure);
        menuEmulation->addAction(menuReset_CPU->menuAction());
        menuReset_CPU->addAction(actionSoft_reset);
        menuReset_CPU->addAction(actionHard_reset);
        menuDebug->addAction(actionCPU);
        menuDebug->addAction(actionPPU);
        menuDebug->addSeparator();
        menuDebug->addAction(actionReset_wrappers);
        menuDebug->addAction(actionDump_stac);
        menuVideo->addAction(actionGo_fullscreen);
        menuVideo->addSeparator();
        menuVideo->addAction(menuFrame_limiter_frameskip->menuAction());
        menuFrame_limiter_frameskip->addAction(actionTurn_on);
        menuSound->addAction(actionEnableSound);

        retranslateUi(GUI_MainWindow);

        QMetaObject::connectSlotsByName(GUI_MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *GUI_MainWindow)
    {
        GUI_MainWindow->setWindowTitle(QApplication::translate("GUI_MainWindow", "Famique [Qt & SDL][NTSC]", 0));
        action_Load_ROM->setText(QApplication::translate("GUI_MainWindow", "&Load ROM", 0));
        action_Exit->setText(QApplication::translate("GUI_MainWindow", "&Exit", 0));
        action_Start_CPU->setText(QApplication::translate("GUI_MainWindow", "&Start Emulation", 0));
        action_Halt_CPU->setText(QApplication::translate("GUI_MainWindow", "&Halt Emulation", 0));
        action->setText(QApplication::translate("GUI_MainWindow", "--", 0));
        actionView_PPU->setText(QApplication::translate("GUI_MainWindow", "View PPU", 0));
        actionAuto->setText(QApplication::translate("GUI_MainWindow", "Auto", 0));
        actionCPU->setText(QApplication::translate("GUI_MainWindow", "CPU", 0));
        actionPPU->setText(QApplication::translate("GUI_MainWindow", "PPU", 0));
        actionReset_wrappers->setText(QApplication::translate("GUI_MainWindow", "Reset wrappers", 0));
        actionGo_fullscreen->setText(QApplication::translate("GUI_MainWindow", "Go fullscreen", 0));
        actionEnableSound->setText(QApplication::translate("GUI_MainWindow", "Enable", 0));
        actionTurn_on->setText(QApplication::translate("GUI_MainWindow", "Activate", 0));
        actionSoft_reset->setText(QApplication::translate("GUI_MainWindow", "Soft reset", 0));
        actionHard_reset->setText(QApplication::translate("GUI_MainWindow", "Hard reset", 0));
        actionConfigure->setText(QApplication::translate("GUI_MainWindow", "Configure", 0));
        actionBlah->setText(QApplication::translate("GUI_MainWindow", "blah", 0));
        actionDump_stac->setText(QApplication::translate("GUI_MainWindow", "Dump stack", 0));
        menu_File->setTitle(QApplication::translate("GUI_MainWindow", "&File", 0));
        menuEmulation->setTitle(QApplication::translate("GUI_MainWindow", "Emulation", 0));
        menuReset_CPU->setTitle(QApplication::translate("GUI_MainWindow", "Reset CPU", 0));
        menuDebug->setTitle(QApplication::translate("GUI_MainWindow", "Debug", 0));
        menuVideo->setTitle(QApplication::translate("GUI_MainWindow", "Video", 0));
        menuFrame_limiter_frameskip->setTitle(QApplication::translate("GUI_MainWindow", "Frame limiter/frameskip", 0));
        menuSound->setTitle(QApplication::translate("GUI_MainWindow", "Sound", 0));
    } // retranslateUi

};

namespace Ui {
    class GUI_MainWindow: public Ui_GUI_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUI_MAINWINDOW_H
