/*********************************************************************************
**                                                                              **
**     Copyright (C) 2020                                                       **
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
#include "test_json.hpp"
#include "defines.hpp"

#include "ui_test_json.h"
//--------------------------------------------------------------------------------
Test_JSON::Test_JSON(QWidget *parent) :
    MyWidget(parent),
    ui(new Ui::Test_JSON)
{
    ui->setupUi(this);

    init();
}
//--------------------------------------------------------------------------------
Test_JSON::~Test_JSON()
{
    QRect rect = geometry();

    int pos_x = rect.x();
    int pos_y = rect.y();
    int pos_w = rect.width();
    int pos_h = rect.height();

    save_int(TEST_JSON_X,   pos_x);
    save_int(TEST_JSON_Y,   pos_y);
    save_int(TEST_JSON_W,   pos_w);
    save_int(TEST_JSON_H,   pos_h);

    save_bytearray(TEST_JSON_SPLITTER,  ui->tj_splitter->saveState());

    delete ui;
}
//--------------------------------------------------------------------------------
void Test_JSON::init(void)
{
    connect(ui->btn_test,   &QPushButton::clicked,  this,   &Test_JSON::test);

    int pos_x = load_int(TEST_JSON_X);
    int pos_y = load_int(TEST_JSON_Y);
    int pos_w = load_int(TEST_JSON_W);
    int pos_h = load_int(TEST_JSON_H);

    QByteArray ba = load_bytearray(TEST_JSON_SPLITTER);

    if(pos_w <= 100) pos_w = 800;
    if(pos_h <= 100) pos_h = 600;

    setGeometry(pos_x, pos_y, pos_w, pos_h);
    ui->tj_splitter->restoreState(ba);
}
//--------------------------------------------------------------------------------
void Test_JSON::test(void)
{
    QString val = ui->te_result->toPlainText().remove("\r").remove("\n");
    if(val.isEmpty())
    {
        emit error("val is empty");
        return;
    }

    //---
    QByteArray ba;
    ba.append(val);
    ui->ShowJSON_widget->loadJson(ba);
    //---

    QJsonParseError jerror;
    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8(), &jerror);
    if(jerror.error != QJsonParseError::NoError)
    {
        switch(jerror.error)
        {
        case QJsonParseError::NoError:                  emit error("NoError");                  break;
        case QJsonParseError::UnterminatedObject:       emit error("UnterminatedObject");       break;
        case QJsonParseError::MissingNameSeparator:     emit error("MissingNameSeparator");     break;
        case QJsonParseError::UnterminatedArray:        emit error("UnterminatedArray");        break;
        case QJsonParseError::MissingValueSeparator:    emit error("MissingValueSeparator");    break;
        case QJsonParseError::IllegalValue:             emit error("IllegalValue");             break;
        case QJsonParseError::TerminationByNumber:      emit error("TerminationByNumber");      break;
        case QJsonParseError::IllegalNumber:            emit error("IllegalNumber");            break;
        case QJsonParseError::IllegalEscapeSequence:    emit error("IllegalEscapeSequence");    break;
        case QJsonParseError::IllegalUTF8String:        emit error("IllegalUTF8String");        break;
        case QJsonParseError::UnterminatedString:       emit error("UnterminatedString");       break;
        case QJsonParseError::MissingObject:            emit error("MissingObject");            break;
        case QJsonParseError::DeepNesting:              emit error("DeepNesting");              break;
        case QJsonParseError::DocumentTooLarge:         emit error("DocumentTooLarge");         break;
        case QJsonParseError::GarbageAtEnd:             emit error("GarbageAtEnd");             break;
        }

        emit error(jerror.errorString());
        return;
    }

    QJsonObject jsonObject = d.object();

    emit info(QString("jsonObject cnt %1").arg(jsonObject.count()));
    for(int n=0; n<jsonObject.count(); n++)
    {
        QString name = jsonObject.keys().at(n);
        emit info(QString("   %1").arg(name));

        emit info(QString("is array %1").arg(jsonObject[name].isArray()));

        int cnt = jsonObject[name].toArray().count();
        emit info(QString("     cnt %1").arg(cnt));
    }
}
//--------------------------------------------------------------------------------
void Test_JSON::updateText(void)
{
    ui->retranslateUi(this);
}
//--------------------------------------------------------------------------------
bool Test_JSON::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Test_JSON::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Test_JSON::save_setting(void)
{

}
//--------------------------------------------------------------------------------
