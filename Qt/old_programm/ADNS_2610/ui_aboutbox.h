/********************************************************************************
** Form generated from reading UI file 'aboutbox.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ABOUTBOX_H
#define UI_ABOUTBOX_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_AboutBox
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *labelLogo;
    QTextEdit *teAbout;
    QHBoxLayout *horizontalLayout_2;
    QToolButton *btn_about_qt;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *AboutBox)
    {
        if (AboutBox->objectName().isEmpty())
            AboutBox->setObjectName(QStringLiteral("AboutBox"));
        AboutBox->resize(523, 307);
        QIcon icon;
        icon.addFile(QStringLiteral(":/images/computer.png"), QSize(), QIcon::Normal, QIcon::Off);
        AboutBox->setWindowIcon(icon);
        verticalLayout = new QVBoxLayout(AboutBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        labelLogo = new QLabel(AboutBox);
        labelLogo->setObjectName(QStringLiteral("labelLogo"));
        labelLogo->setMinimumSize(QSize(247, 250));
        labelLogo->setPixmap(QPixmap(QString::fromUtf8(":/logo/pinguin.png")));
        labelLogo->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(labelLogo);

        teAbout = new QTextEdit(AboutBox);
        teAbout->setObjectName(QStringLiteral("teAbout"));
        teAbout->setMinimumSize(QSize(247, 0));
        teAbout->setReadOnly(true);

        horizontalLayout->addWidget(teAbout);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        btn_about_qt = new QToolButton(AboutBox);
        btn_about_qt->setObjectName(QStringLiteral("btn_about_qt"));

        horizontalLayout_2->addWidget(btn_about_qt);

        buttonBox = new QDialogButtonBox(AboutBox);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Close);
        buttonBox->setCenterButtons(true);

        horizontalLayout_2->addWidget(buttonBox);


        verticalLayout->addLayout(horizontalLayout_2);


        retranslateUi(AboutBox);

        QMetaObject::connectSlotsByName(AboutBox);
    } // setupUi

    void retranslateUi(QDialog *AboutBox)
    {
        AboutBox->setWindowTitle(QApplication::translate("AboutBox", "About", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class AboutBox: public Ui_AboutBox {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ABOUTBOX_H
