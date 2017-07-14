/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QTextCodec>
#include <QTextEdit>
#include <QDialog>
#include <QWidget>
//--------------------------------------------------------------------------------
#include <QDebug>
//--------------------------------------------------------------------------------
#include "log_options.hpp"
#include "ui_log_options.h"
//--------------------------------------------------------------------------------
Log_options::Log_options(QWidget *parent):
    QDialog(parent),
    ui(new Ui::Log_options)
{
    ui->setupUi(this);

    setWindowTitle(tr("Options"));

    findCodecs();

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    ui->cb_CodecForCStrings->setCurrentIndex(ui->cb_CodecForCStrings->findText(QTextCodec::codecForCStrings()->name()));
#endif

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

    setFixedSize(sizeHint());
}
//--------------------------------------------------------------------------------
Log_options::~Log_options()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void Log_options::findCodecs(void)
{
    codecs.clear();
    foreach (int mib, QTextCodec::availableMibs())
    {
        QTextCodec *codec = QTextCodec::codecForMib(mib);
        codecs.append(codec->name());
    }
    qSort(codecs);

    ui->cb_CodecForCStrings->addItems(codecs);
}
//--------------------------------------------------------------------------------
bool Log_options::get_flag_ReadOnly(void)
{
    return ui->cb_readOnly->isChecked();
}
//--------------------------------------------------------------------------------
bool Log_options::get_flag_AcceptRichText(void)
{
    return ui->cb_acceptRichText->isChecked();
}
//--------------------------------------------------------------------------------
bool Log_options::get_flag_NoCRLF(void)
{
    return ui->cb_NoCRLF->isChecked();
}
//--------------------------------------------------------------------------------
bool Log_options::get_flag_AddDateTime(void)
{
    return ui->cb_add_DateTime->isChecked();
}
//--------------------------------------------------------------------------------
bool Log_options::get_flag_Color(void)
{
    return ui->cb_Color->isChecked();
}
//--------------------------------------------------------------------------------
bool Log_options::get_flag_AutoLoad(void)
{
    return ui->cb_Auto_Load->isChecked();
}
//--------------------------------------------------------------------------------
bool Log_options::get_flag_AutoSave(void)
{
    return ui->cb_Auto_Save->isChecked();
}
//--------------------------------------------------------------------------------
bool Log_options::get_flag_ErrorAsMessage(void)
{
    return ui->cb_ErrorAsMessage->isChecked();
}
//--------------------------------------------------------------------------------
bool Log_options::get_flag_TextIsWindows(void)
{
    return ui->cb_Text_Is_Windows->isChecked();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void Log_options::set_flag_ReadOnly(bool value)
{
    ui->cb_readOnly->setChecked(value);
}
//--------------------------------------------------------------------------------
void Log_options::set_flag_AcceptRichText(bool value)
{
    ui->cb_acceptRichText->setChecked(value);
}
//--------------------------------------------------------------------------------
void Log_options::set_flag_NoCRLF(bool value)
{
    ui->cb_NoCRLF->setChecked(value);
}
//--------------------------------------------------------------------------------
void Log_options::set_flag_AddDateTime(bool value)
{
    ui->cb_add_DateTime->setChecked(value);
}
//--------------------------------------------------------------------------------
void Log_options::set_flag_Color(bool value)
{
    ui->cb_Color->setChecked(value);
}
//--------------------------------------------------------------------------------
void Log_options::set_flag_AutoLoad(bool value)
{
    ui->cb_Auto_Load->setChecked(value);
}
//--------------------------------------------------------------------------------
void Log_options::set_flag_AutoSave(bool value)
{
    ui->cb_Auto_Save->setChecked(value);
}
//--------------------------------------------------------------------------------
void Log_options::set_flag_ErrorAsMessage(bool value)
{
    ui->cb_ErrorAsMessage->setChecked(value);
}
//--------------------------------------------------------------------------------
void Log_options::set_flag_TextIsWindows(bool value)
{
    ui->cb_Text_Is_Windows->setChecked(value);
}
//--------------------------------------------------------------------------------
