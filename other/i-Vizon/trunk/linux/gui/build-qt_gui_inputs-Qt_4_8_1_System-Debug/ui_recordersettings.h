/********************************************************************************
** Form generated from reading UI file 'recordersettings.ui'
**
** Created: Mon Apr 21 16:39:07 2014
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RECORDERSETTINGS_H
#define UI_RECORDERSETTINGS_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QComboBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QStatusBar>
#include <QtGui/QTabWidget>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_RecorderSettings
{
public:
    QWidget *centralWidget;
    QTabWidget *tabWidget;
    QWidget *tab;
    QComboBox *videoType1;
    QLabel *label_4;
    QComboBox *resTab1;
    QLabel *label_2;
    QLabel *label_6;
    QComboBox *overWriteTab1;
    QLabel *label_3;
    QComboBox *colorChannelsTab1;
    QPushButton *stopTab1;
    QLabel *label_5;
    QLabel *label_7;
    QPushButton *startTab1;
    QComboBox *camIdTab1;
    QLineEdit *lineEdit;
    QPushButton *setTab1;
    QWidget *tab_2;
    QLabel *label_8;
    QLabel *label_10;
    QComboBox *comboBox_7;
    QLabel *label_11;
    QComboBox *comboBox_9;
    QPushButton *pushButton_3;
    QComboBox *comboBox_10;
    QLabel *label_12;
    QPushButton *pushButton_4;
    QComboBox *comboBox_11;
    QLabel *label_14;
    QComboBox *comboBox_12;
    QLineEdit *lineEdit_2;
    QComboBox *comboBox_13;
    QComboBox *comboBox_14;
    QLabel *label_15;
    QComboBox *comboBox_15;
    QComboBox *comboBox_16;
    QComboBox *comboBox_8;
    QPushButton *pushButton_6;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *RecorderSettings)
    {
        if (RecorderSettings->objectName().isEmpty())
            RecorderSettings->setObjectName(QString::fromUtf8("RecorderSettings"));
        RecorderSettings->resize(649, 461);
        centralWidget = new QWidget(RecorderSettings);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        tabWidget->setGeometry(QRect(10, 0, 631, 381));
        tab = new QWidget();
        tab->setObjectName(QString::fromUtf8("tab"));
        videoType1 = new QComboBox(tab);
        videoType1->setObjectName(QString::fromUtf8("videoType1"));
        videoType1->setGeometry(QRect(440, 40, 66, 31));
        label_4 = new QLabel(tab);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(30, 163, 121, 21));
        resTab1 = new QComboBox(tab);
        resTab1->setObjectName(QString::fromUtf8("resTab1"));
        resTab1->setGeometry(QRect(160, 100, 108, 31));
        label_2 = new QLabel(tab);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(33, 100, 86, 21));
        label_6 = new QLabel(tab);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(306, 100, 84, 21));
        overWriteTab1 = new QComboBox(tab);
        overWriteTab1->setObjectName(QString::fromUtf8("overWriteTab1"));
        overWriteTab1->setGeometry(QRect(440, 100, 79, 31));
        label_3 = new QLabel(tab);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(33, 40, 83, 21));
        colorChannelsTab1 = new QComboBox(tab);
        colorChannelsTab1->setObjectName(QString::fromUtf8("colorChannelsTab1"));
        colorChannelsTab1->setGeometry(QRect(157, 163, 76, 31));
        stopTab1 = new QPushButton(tab);
        stopTab1->setObjectName(QString::fromUtf8("stopTab1"));
        stopTab1->setGeometry(QRect(450, 270, 85, 31));
        label_5 = new QLabel(tab);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(306, 40, 90, 21));
        label_7 = new QLabel(tab);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(110, 220, 37, 21));
        startTab1 = new QPushButton(tab);
        startTab1->setObjectName(QString::fromUtf8("startTab1"));
        startTab1->setGeometry(QRect(280, 270, 85, 31));
        camIdTab1 = new QComboBox(tab);
        camIdTab1->setObjectName(QString::fromUtf8("camIdTab1"));
        camIdTab1->setGeometry(QRect(160, 40, 41, 31));
        lineEdit = new QLineEdit(tab);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(180, 220, 341, 31));
        setTab1 = new QPushButton(tab);
        setTab1->setObjectName(QString::fromUtf8("setTab1"));
        setTab1->setGeometry(QRect(130, 270, 85, 31));
        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QString::fromUtf8("tab_2"));
        label_8 = new QLabel(tab_2);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(260, 20, 90, 21));
        label_10 = new QLabel(tab_2);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(370, 20, 84, 21));
        comboBox_7 = new QComboBox(tab_2);
        comboBox_7->setObjectName(QString::fromUtf8("comboBox_7"));
        comboBox_7->setGeometry(QRect(500, 50, 76, 31));
        label_11 = new QLabel(tab_2);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(160, 170, 37, 21));
        comboBox_9 = new QComboBox(tab_2);
        comboBox_9->setObjectName(QString::fromUtf8("comboBox_9"));
        comboBox_9->setGeometry(QRect(130, 50, 108, 31));
        pushButton_3 = new QPushButton(tab_2);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(260, 240, 85, 31));
        comboBox_10 = new QComboBox(tab_2);
        comboBox_10->setObjectName(QString::fromUtf8("comboBox_10"));
        comboBox_10->setGeometry(QRect(260, 50, 66, 31));
        label_12 = new QLabel(tab_2);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(480, 20, 121, 21));
        pushButton_4 = new QPushButton(tab_2);
        pushButton_4->setObjectName(QString::fromUtf8("pushButton_4"));
        pushButton_4->setGeometry(QRect(440, 240, 85, 31));
        comboBox_11 = new QComboBox(tab_2);
        comboBox_11->setObjectName(QString::fromUtf8("comboBox_11"));
        comboBox_11->setGeometry(QRect(370, 50, 79, 31));
        label_14 = new QLabel(tab_2);
        label_14->setObjectName(QString::fromUtf8("label_14"));
        label_14->setGeometry(QRect(20, 20, 83, 21));
        comboBox_12 = new QComboBox(tab_2);
        comboBox_12->setObjectName(QString::fromUtf8("comboBox_12"));
        comboBox_12->setGeometry(QRect(20, 50, 71, 31));
        lineEdit_2 = new QLineEdit(tab_2);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setGeometry(QRect(220, 160, 351, 31));
        comboBox_13 = new QComboBox(tab_2);
        comboBox_13->setObjectName(QString::fromUtf8("comboBox_13"));
        comboBox_13->setGeometry(QRect(20, 90, 71, 31));
        comboBox_14 = new QComboBox(tab_2);
        comboBox_14->setObjectName(QString::fromUtf8("comboBox_14"));
        comboBox_14->setGeometry(QRect(130, 90, 108, 31));
        label_15 = new QLabel(tab_2);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(140, 20, 91, 21));
        comboBox_15 = new QComboBox(tab_2);
        comboBox_15->setObjectName(QString::fromUtf8("comboBox_15"));
        comboBox_15->setGeometry(QRect(260, 90, 66, 31));
        comboBox_16 = new QComboBox(tab_2);
        comboBox_16->setObjectName(QString::fromUtf8("comboBox_16"));
        comboBox_16->setGeometry(QRect(370, 90, 79, 31));
        comboBox_8 = new QComboBox(tab_2);
        comboBox_8->setObjectName(QString::fromUtf8("comboBox_8"));
        comboBox_8->setGeometry(QRect(500, 90, 76, 31));
        pushButton_6 = new QPushButton(tab_2);
        pushButton_6->setObjectName(QString::fromUtf8("pushButton_6"));
        pushButton_6->setGeometry(QRect(80, 240, 85, 31));
        tabWidget->addTab(tab_2, QString());
        RecorderSettings->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(RecorderSettings);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 649, 29));
        RecorderSettings->setMenuBar(menuBar);
        mainToolBar = new QToolBar(RecorderSettings);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        RecorderSettings->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(RecorderSettings);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        RecorderSettings->setStatusBar(statusBar);

        retranslateUi(RecorderSettings);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(RecorderSettings);
    } // setupUi

    void retranslateUi(QMainWindow *RecorderSettings)
    {
        RecorderSettings->setWindowTitle(QApplication::translate("RecorderSettings", "RecorderSettings", 0, QApplication::UnicodeUTF8));
        videoType1->clear();
        videoType1->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "AVI", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "MP4", 0, QApplication::UnicodeUTF8)
        );
        label_4->setText(QApplication::translate("RecorderSettings", "Color Channels", 0, QApplication::UnicodeUTF8));
        resTab1->clear();
        resTab1->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "640x480", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "1024x768", 0, QApplication::UnicodeUTF8)
        );
        label_2->setText(QApplication::translate("RecorderSettings", "Resolution", 0, QApplication::UnicodeUTF8));
        label_6->setText(QApplication::translate("RecorderSettings", "OverWrite", 0, QApplication::UnicodeUTF8));
        overWriteTab1->clear();
        overWriteTab1->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "TRUE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "FALSE", 0, QApplication::UnicodeUTF8)
        );
        label_3->setText(QApplication::translate("RecorderSettings", "Camera ID", 0, QApplication::UnicodeUTF8));
        colorChannelsTab1->clear();
        colorChannelsTab1->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "GRAY", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "RGB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "BGR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "YUV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "RGBA", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "BGRA", 0, QApplication::UnicodeUTF8)
        );
        stopTab1->setText(QApplication::translate("RecorderSettings", "Stop", 0, QApplication::UnicodeUTF8));
        label_5->setText(QApplication::translate("RecorderSettings", "Video Type", 0, QApplication::UnicodeUTF8));
        label_7->setText(QApplication::translate("RecorderSettings", "Path", 0, QApplication::UnicodeUTF8));
        startTab1->setText(QApplication::translate("RecorderSettings", "Start", 0, QApplication::UnicodeUTF8));
        camIdTab1->clear();
        camIdTab1->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "3", 0, QApplication::UnicodeUTF8)
        );
        setTab1->setText(QApplication::translate("RecorderSettings", "Set", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("RecorderSettings", "Single Camera", 0, QApplication::UnicodeUTF8));
        label_8->setText(QApplication::translate("RecorderSettings", "Video Type", 0, QApplication::UnicodeUTF8));
        label_10->setText(QApplication::translate("RecorderSettings", "OverWrite", 0, QApplication::UnicodeUTF8));
        comboBox_7->clear();
        comboBox_7->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "GRAY", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "RGB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "BGR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "YUV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "RGBA", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "BGRA", 0, QApplication::UnicodeUTF8)
        );
        label_11->setText(QApplication::translate("RecorderSettings", "Path", 0, QApplication::UnicodeUTF8));
        comboBox_9->clear();
        comboBox_9->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "640x480", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "1024x768", 0, QApplication::UnicodeUTF8)
        );
        pushButton_3->setText(QApplication::translate("RecorderSettings", "Start", 0, QApplication::UnicodeUTF8));
        comboBox_10->clear();
        comboBox_10->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "AVI", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "MP4", 0, QApplication::UnicodeUTF8)
        );
        label_12->setText(QApplication::translate("RecorderSettings", "Color Channels", 0, QApplication::UnicodeUTF8));
        pushButton_4->setText(QApplication::translate("RecorderSettings", "Stop", 0, QApplication::UnicodeUTF8));
        comboBox_11->clear();
        comboBox_11->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "TRUE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "FALSE", 0, QApplication::UnicodeUTF8)
        );
        label_14->setText(QApplication::translate("RecorderSettings", "Camera ID", 0, QApplication::UnicodeUTF8));
        comboBox_12->clear();
        comboBox_12->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "NULL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "3", 0, QApplication::UnicodeUTF8)
        );
        comboBox_13->clear();
        comboBox_13->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "NULL", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "0", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "1", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "2", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "3", 0, QApplication::UnicodeUTF8)
        );
        comboBox_14->clear();
        comboBox_14->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "640x480", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "1024x768", 0, QApplication::UnicodeUTF8)
        );
        label_15->setText(QApplication::translate("RecorderSettings", "Resolution", 0, QApplication::UnicodeUTF8));
        comboBox_15->clear();
        comboBox_15->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "AVI", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "MP4", 0, QApplication::UnicodeUTF8)
        );
        comboBox_16->clear();
        comboBox_16->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "TRUE", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "FALSE", 0, QApplication::UnicodeUTF8)
        );
        comboBox_8->clear();
        comboBox_8->insertItems(0, QStringList()
         << QApplication::translate("RecorderSettings", "GRAY", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "RGB", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "BGR", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "YUV", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "RGBA", 0, QApplication::UnicodeUTF8)
         << QApplication::translate("RecorderSettings", "BGRA", 0, QApplication::UnicodeUTF8)
        );
        pushButton_6->setText(QApplication::translate("RecorderSettings", "Set", 0, QApplication::UnicodeUTF8));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("RecorderSettings", "Multi Camera", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RecorderSettings: public Ui_RecorderSettings {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RECORDERSETTINGS_H
