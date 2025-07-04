/*********************************************************************************
**                                                                              **
**     Copyright (C) 2025                                                       **
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
#ifndef LOG_OPTIONS_HPP
#define LOG_OPTIONS_HPP
//--------------------------------------------------------------------------------
#include <QTextCodec>
#include <QTextEdit>
#include <QDialog>
//--------------------------------------------------------------------------------
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#   include <QTextCodec>
#endif
//--------------------------------------------------------------------------------
namespace Ui {
    class Log_options;
}
//--------------------------------------------------------------------------------
class Log_options : public QDialog
{
    Q_OBJECT

    //TODO не забыть, что название property должны совпадать с первым полем Q_PROPERTY
    Q_PROPERTY(bool flag_ReadOnly           READ get_flag_ReadOnly          WRITE set_flag_ReadOnly)
    Q_PROPERTY(bool flag_AcceptRichText     READ get_flag_AcceptRichText    WRITE set_flag_AcceptRichText)

    Q_PROPERTY(bool flag_NoCRLF             READ get_flag_NoCRLF            WRITE set_flag_NoCRLF)
    Q_PROPERTY(bool flag_AddDateTime        READ get_flag_AddDateTime       WRITE set_flag_AddDateTime)
    Q_PROPERTY(bool flag_Color              READ get_flag_Color             WRITE set_flag_Color)
    Q_PROPERTY(bool flag_ErrorAsMessage     READ get_flag_ErrorAsMessage    WRITE set_flag_ErrorAsMessage)
    Q_PROPERTY(bool flag_TextIsWindows      READ get_flag_TextIsWindows     WRITE set_flag_TextIsWindows)

public:
    explicit Log_options(QWidget *parent = nullptr);
    virtual ~Log_options();

    QTextCodec *get_text_codec(void);

private:
    Ui::Log_options *ui;
    QStringList codecs;
    QTextEdit *logEdit = nullptr;

    QString autosave_filename = "noname.log";

    void init(void);

    void findCodecs(void);

    bool get_flag_ReadOnly(void);
    bool get_flag_AcceptRichText(void);

    bool get_flag_NoCRLF(void);
    bool get_flag_AddDateTime(void);
    bool get_flag_Color(void);
    bool get_flag_ErrorAsMessage(void);
    bool get_flag_TextIsWindows(void);
    QString get_file_AutoSave(void);

    void set_flag_ReadOnly(bool value);
    void set_flag_AcceptRichText(bool value);

    void set_flag_NoCRLF(bool value);
    void set_flag_AddDateTime(bool value);
    void set_flag_Color(bool value);
    void set_flag_ErrorAsMessage(bool value);
    void set_flag_TextIsWindows(bool value);
};
//--------------------------------------------------------------------------------
#endif // OPTIONS_BOX_HPP
