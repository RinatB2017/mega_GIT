/********************************************************************************
** Form generated from reading UI file 'chat.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHAT_H
#define UI_CHAT_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_Chat
{
public:
    QVBoxLayout *verticalLayout;
    QGroupBox *localAdapterBox;
    QHBoxLayout *horizontalLayout_3;
    QRadioButton *firstAdapter;
    QRadioButton *secondAdapter;
    QTextEdit *chat;
    QHBoxLayout *horizontalLayout;
    QLineEdit *sendText;
    QPushButton *sendButton;
    QHBoxLayout *horizontalLayout_2;
    QSpacerItem *horizontalSpacer;
    QPushButton *connectButton;
    QPushButton *quitButton;

    void setupUi(QDialog *Chat)
    {
        if (Chat->objectName().isEmpty())
            Chat->setObjectName(QString::fromUtf8("Chat"));
        Chat->resize(400, 300);
        verticalLayout = new QVBoxLayout(Chat);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        localAdapterBox = new QGroupBox(Chat);
        localAdapterBox->setObjectName(QString::fromUtf8("localAdapterBox"));
        localAdapterBox->setCheckable(false);
        horizontalLayout_3 = new QHBoxLayout(localAdapterBox);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        firstAdapter = new QRadioButton(localAdapterBox);
        firstAdapter->setObjectName(QString::fromUtf8("firstAdapter"));

        horizontalLayout_3->addWidget(firstAdapter);

        secondAdapter = new QRadioButton(localAdapterBox);
        secondAdapter->setObjectName(QString::fromUtf8("secondAdapter"));

        horizontalLayout_3->addWidget(secondAdapter);


        verticalLayout->addWidget(localAdapterBox);

        chat = new QTextEdit(Chat);
        chat->setObjectName(QString::fromUtf8("chat"));
        chat->setFocusPolicy(Qt::NoFocus);

        verticalLayout->addWidget(chat);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        sendText = new QLineEdit(Chat);
        sendText->setObjectName(QString::fromUtf8("sendText"));

        horizontalLayout->addWidget(sendText);

        sendButton = new QPushButton(Chat);
        sendButton->setObjectName(QString::fromUtf8("sendButton"));
        sendButton->setDefault(true);

        horizontalLayout->addWidget(sendButton);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer);

        connectButton = new QPushButton(Chat);
        connectButton->setObjectName(QString::fromUtf8("connectButton"));

        horizontalLayout_2->addWidget(connectButton);


        verticalLayout->addLayout(horizontalLayout_2);

        quitButton = new QPushButton(Chat);
        quitButton->setObjectName(QString::fromUtf8("quitButton"));

        verticalLayout->addWidget(quitButton);


        retranslateUi(Chat);

        QMetaObject::connectSlotsByName(Chat);
    } // setupUi

    void retranslateUi(QDialog *Chat)
    {
        Chat->setWindowTitle(QApplication::translate("Chat", "Bluetooth Chat", 0, QApplication::UnicodeUTF8));
        localAdapterBox->setTitle(QApplication::translate("Chat", "Local Bluetooth Adapter", 0, QApplication::UnicodeUTF8));
        firstAdapter->setText(QApplication::translate("Chat", "Default", 0, QApplication::UnicodeUTF8));
        secondAdapter->setText(QString());
        sendButton->setText(QApplication::translate("Chat", "Send", 0, QApplication::UnicodeUTF8));
        connectButton->setText(QApplication::translate("Chat", "Connect", 0, QApplication::UnicodeUTF8));
        quitButton->setText(QApplication::translate("Chat", "Quit", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class Chat: public Ui_Chat {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHAT_H
