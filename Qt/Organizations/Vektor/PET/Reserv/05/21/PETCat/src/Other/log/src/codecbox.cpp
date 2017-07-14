//--------------------------------------------------------------------------------
#include <QTextCodec>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "codecbox.hpp"
#include "ui_codecbox.h"
//--------------------------------------------------------------------------------
CodecBox::CodecBox(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CodecBox)
{
    int n;

    ui->setupUi(this);
    findCodecs();

//    qDebug() << QTextCodec::codecForTr()->name();
//    qDebug() << QTextCodec::codecForCStrings()->name();
//    qDebug() << QTextCodec::codecForLocale()->name();

    n = ui->cbCodecForTr->findText(QTextCodec::codecForTr()->name());
    ui->cbCodecForTr->setCurrentIndex(n);

    n = ui->cbCodecForCStrings->findText(QTextCodec::codecForCStrings()->name());
    ui->cbCodecForCStrings->setCurrentIndex(n);

    n = ui->cbCodecForLocale->findText(QTextCodec::codecForLocale()->name());
    ui->cbCodecForLocale->setCurrentIndex(n);

    connect(ui->buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(ui->buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}
//--------------------------------------------------------------------------------
CodecBox::~CodecBox()
{
    delete ui;
}
//--------------------------------------------------------------------------------
void CodecBox::findCodecs(void)
{
    QTextCodec *codec;

    codecs.clear();
    foreach (int mib, QTextCodec::availableMibs())
    {
        codec = QTextCodec::codecForMib(mib);
        codecs.append(codec->name());
    }
    qSort(codecs);
    ui->cbCodecForTr->clear();
    ui->cbCodecForCStrings->clear();
    ui->cbCodecForLocale->clear();

    ui->cbCodecForTr->addItems(codecs);
    ui->cbCodecForCStrings->addItems(codecs);
    ui->cbCodecForLocale->addItems(codecs);
}
//--------------------------------------------------------------------------------
void CodecBox::accept(void)
{
    qDebug() << "accept()";

    QTextCodec::setCodecForTr(QTextCodec::codecForName(ui->cbCodecForTr->currentText().toAscii()));
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName(ui->cbCodecForCStrings->currentText().toAscii()));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName(ui->cbCodecForLocale->currentText().toAscii()));

    QDialog::accept();
}
//--------------------------------------------------------------------------------
