/********************************************************************************
** Form generated from reading UI file 'filterwindow.ui'
**
** Created: Sat Apr 26 18:53:52 2014
**      by: Qt User Interface Compiler version 4.8.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILTERWINDOW_H
#define UI_FILTERWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QSlider>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_FilterWindow
{
public:
    QWidget *centralWidget;
    QSlider *horizontalSlider;
    QVTKWidget *widget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FilterWindow)
    {
        if (FilterWindow->objectName().isEmpty())
            FilterWindow->setObjectName(QString::fromUtf8("FilterWindow"));
        FilterWindow->resize(698, 491);
        centralWidget = new QWidget(FilterWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        horizontalSlider = new QSlider(centralWidget);
        horizontalSlider->setObjectName(QString::fromUtf8("horizontalSlider"));
        horizontalSlider->setGeometry(QRect(30, 370, 631, 29));
        horizontalSlider->setOrientation(Qt::Horizontal);
        widget = new QVTKWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(20, 10, 641, 331));
        FilterWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FilterWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 698, 29));
        FilterWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FilterWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        FilterWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FilterWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FilterWindow->setStatusBar(statusBar);

        retranslateUi(FilterWindow);

        QMetaObject::connectSlotsByName(FilterWindow);
    } // setupUi

    void retranslateUi(QMainWindow *FilterWindow)
    {
        FilterWindow->setWindowTitle(QApplication::translate("FilterWindow", "FilterWindow", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FilterWindow: public Ui_FilterWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILTERWINDOW_H
