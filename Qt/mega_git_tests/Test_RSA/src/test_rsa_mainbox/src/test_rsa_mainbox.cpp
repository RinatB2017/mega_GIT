/*********************************************************************************
**                                                                              **
**     Copyright (C) 2015                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifdef HAVE_QT5
#   include <QtWidgets>
#else
#   include <QtGui>
#endif
//--------------------------------------------------------------------------------
#include "ui_test_rsa_mainbox.h"
//--------------------------------------------------------------------------------
#include "mywaitsplashscreen.hpp"
#include "mysplashscreen.hpp"
#include "mainwindow.hpp"
#include "test_rsa_mainbox.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MyWidget(parent),
    splash(splash),
    ui(new Ui::MainBox)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    save_widgets();
    delete ui;
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    ui->setupUi(this);

#ifdef QT_DEBUG
    createTestBar();
#endif

    load_widgets();

    setVisible(false);
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    commands.clear(); int id = 0;
    commands.append({ id++, "test 0", &MainBox::test_0 });
    commands.append({ id++, "test 1", &MainBox::test_1 });
    commands.append({ id++, "test 2", &MainBox::test_2 });
    commands.append({ id++, "test 3", &MainBox::test_3 });
    commands.append({ id++, "test 4", &MainBox::test_4 });
    commands.append({ id++, "test 5", &MainBox::test_5 });
    commands.append({ id++, "test 6", nullptr });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    cb_test = new QComboBox(this);
    cb_test->setObjectName("cb_test");
    foreach (CMD command, commands)
    {
        cb_test->addItem(command.cmd_text, QVariant(command.cmd));
    }

    testbar->addWidget(cb_test);
    QToolButton *btn_choice_test = add_button(testbar,
                                              new QToolButton(this),
                                              qApp->style()->standardIcon(QStyle::SP_MediaPlay),
                                              "choice_test",
                                              "choice_test");
    btn_choice_test->setObjectName("btn_choice_test");

    connect(btn_choice_test, SIGNAL(clicked()), this, SLOT(choice_test()));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok)
    {
        return;
    }
    auto cmd_it = std::find_if(
        commands.begin(),
        commands.end(),
        [cmd](CMD command){ return command.cmd == cmd; }
    );
    if (cmd_it != commands.end())
    {
        typedef void (MainBox::*function)(void);
        function x;
        x = cmd_it->func;
        if(x)
        {
            (this->*x)();
        }
        else
        {
            emit error("no func");
        }
    }
}
//--------------------------------------------------------------------------------
bool MainBox::generate_keys(void)
{
    QCA::SecureArray arg = "hello";

    if(!QCA::isSupported("pkey") || !QCA::PKey::supportedIOTypes().contains(QCA::PKey::RSA))
    {
        emit error("RSA not supported!");
        return false;
    }
    else
    {
        // When creating a public / private key pair, you make the
        // private key, and then extract the public key component from it
        // Using RSA is very common, however DSA can provide equivalent
        // signature/verification. This example applies to DSA to the
        // extent that the operations work on that key type.

        // QCA provides KeyGenerator as a convenient source of new keys,
        // however you could also import an existing key instead.
        seckey = QCA::KeyGenerator().createRSA(1024);
        if(seckey.isNull())
        {
            emit error("Failed to make private RSA key");
            return false;
        }

        pubkey = seckey.toPublicKey();

        // check if the key can encrypt
        if(!pubkey.canEncrypt())
        {
            emit error("Error: this kind of key cannot encrypt");
            return false;
        }
    }
    emit info("OK");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::load_private_file(QString filename, QCA::SecureArray passPhrase)
{
    QCA::ConvertResult conversionResult;
    seckey = QCA::PrivateKey::fromPEMFile(filename,
                                          passPhrase,
                                          &conversionResult);
    if (! (QCA::ConvertGood == conversionResult) )
    {
        emit error("Private key read failed");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::load_public_file(QString filename)
{
    pubkey = QCA::PublicKey::fromPEMFile(filename);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::save_private_file(QString filename, QCA::SecureArray passPhrase)
{
    seckey.toPEMFile(filename, passPhrase);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::save_public_file(QString filename)
{
    pubkey.toPEMFile(filename);
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::crypt(QCA::SecureArray data, QCA::SecureArray &result)
{
    result = pubkey.encrypt(data, QCA::EME_PKCS1_OAEP);
    if(data.isEmpty())
    {
        emit error("Error encrypting");
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::decrypt(QCA::SecureArray data, QByteArray *result)
{
    QCA::SecureArray decrypt;
    if(0 == seckey.decrypt(data, &decrypt, QCA::EME_PKCS1_OAEP))
    {
        emit error("Error decrypting.");
        return false;
    }
    (*result).clear();
    (*result).append(decrypt.data());
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::test_0(void)
{
    emit info("Test_0()");

    bool ok = false;

    QCA::Initializer init;
    generate_keys();
    save_public_file("keypublic.pem");
    save_private_file("keyprivate.pem", "pass phrase");

    QCA::SecureArray ba = "hello";
    QCA::SecureArray result;
    ok = crypt(ba, result);
    if(!ok)
    {
        emit error("Error crypt");
        return;
    }
    emit info(QString("%1").arg(result.toByteArray().toHex().data()));
}
//--------------------------------------------------------------------------------
void MainBox::test_1(void)
{
    emit info("Test_1()");

    bool ok = false;

    QCA::Initializer init;
    load_public_file("keypublic.pem");
    load_private_file("keyprivate.pem", "pass phrase");

    QCA::SecureArray ba = QByteArray::fromHex("356bd675e7cbc58f70dfcd9de1c06c7f957131700c1506b565094f4264f678234e15403e1b6f0a2a383a0105e0c5f41da596b5185c76be5d71e2051de9b66664b938061221f1a8c648fc12ea1fe14b44f82c795a1a4fa05ec227eecb79fc2c9c06665a057506292c25746a199af065d28ac2f37ef51aaa2a9e428f58087226c2");
    QByteArray result;
    ok = decrypt(ba, &result);
    if(!ok)
    {
        emit error("Error crypt");
        return;
    }
    emit info(QString("Result = [%1]").arg(result.data()));
}
//--------------------------------------------------------------------------------
void MainBox::test_2(void)
{
    emit info("Test_2()");

    QCA::Initializer init;
    load_public_file("keypublic.pem");
    load_private_file("keyprivate.pem", "pass phrase");

    QCA::SecureArray ba;
    ba.resize(100000);
    QCA::SecureArray result;

    QElapsedTimer timer;
    timer.start();
    for(int n=0; n<10000; n++)
    {
        bool ok = crypt(ba, result);
        if(!ok)
        {
            emit error("Error crypt");
            return;
        }
    }
    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_3(void)
{
    emit info("Test_3()");

    QCA::Initializer init;
    load_public_file("keypublic.pem");
    load_private_file("keyprivate.pem", "pass phrase");

    QCA::SecureArray ba = QByteArray::fromHex("356bd675e7cbc58f70dfcd9de1c06c7f957131700c1506b565094f4264f678234e15403e1b6f0a2a383a0105e0c5f41da596b5185c76be5d71e2051de9b66664b938061221f1a8c648fc12ea1fe14b44f82c795a1a4fa05ec227eecb79fc2c9c06665a057506292c25746a199af065d28ac2f37ef51aaa2a9e428f58087226c2");
    QByteArray result;

    QElapsedTimer timer;
    timer.start();
    for(int n=0; n<10000; n++)
    {
        bool ok = decrypt(ba, &result);
        if(!ok)
        {
            emit error("Error decrypt");
            return;
        }
    }
    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
    emit info("OK");
}
//--------------------------------------------------------------------------------
void MainBox::test_4(void)
{
    emit info("Test_4()");
}
//--------------------------------------------------------------------------------
void MainBox::test_5(void)
{
    emit info("Test_5()");
}
//--------------------------------------------------------------------------------
void MainBox::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{

}
//--------------------------------------------------------------------------------
