/*********************************************************************************
**                                                                              **
**     Copyright (C) 2016                                                       **
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
#ifndef MYWIDGET_HPP
#define MYWIDGET_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
#define TAG_SAVE    "w_save"
//--------------------------------------------------------------------------------
#define MESSAGEBOX_WIDTH 320
//--------------------------------------------------------------------------------
class MyWidget : public QWidget
{
    Q_OBJECT

public:
    enum StandardButton {
        // keep this in sync with QDialogButtonBox::StandardButton and QPlatformDialogHelper::StandardButton
        NoButton           = 0x00000000,
        Ok                 = 0x00000400,
        Save               = 0x00000800,
        SaveAll            = 0x00001000,
        Open               = 0x00002000,
        Yes                = 0x00004000,
        YesToAll           = 0x00008000,
        No                 = 0x00010000,
        NoToAll            = 0x00020000,
        Abort              = 0x00040000,
        Retry              = 0x00080000,
        Ignore             = 0x00100000,
        Close              = 0x00200000,
        Cancel             = 0x00400000,
        Discard            = 0x00800000,
        Help               = 0x01000000,
        Apply              = 0x02000000,
        Reset              = 0x04000000,
        RestoreDefaults    = 0x08000000,

        FirstButton        = Ok,                // internal
        LastButton         = RestoreDefaults,   // internal

        YesAll             = YesToAll,          // obsolete
        NoAll              = NoToAll,           // obsolete

        Default            = 0x00000100,        // obsolete
        Escape             = 0x00000200,        // obsolete
        FlagMask           = 0x00000300,        // obsolete
        ButtonMask         = ~FlagMask          // obsolete
    };

    explicit MyWidget(QWidget *parent = 0);
    ~MyWidget();

    void connect_log(QWidget *parent);

    void block_wheel(void);

    void block_this_button(bool state);
    void block_interface(bool state);
    void block_widget(const QString name, bool state);

    bool is_slot_exists(QWidget *obj, const char *slot_sign);

    //---
    static bool set_param(QString group_name,
                          QString name,
                          QVariant value);
    static bool get_param(QString group_name,
                          QString name,
                          QVariant default_value,
                          QVariant *value);
    //---

    static QStringList get_all_param_name(QString group_name);

    void block_close(bool state);

    void load_config(void);
    void save_config(void);

    void load_widgets(QString group_name);
    void save_widgets(QString group_name);

    //---
    void load_QCheckBox(QString group_name);
    void load_QComboBox(QString group_name);
    void load_QPushButton(QString group_name);
    void load_QToolButton(QString group_name);
    void load_QTextEdit(QString group_name);
    void load_QLineEdit(QString group_name);
    void load_QSpinBox(QString group_name);
    void load_QDoubleSpinBox(QString group_name);
    void load_QSlider(QString group_name);
    void load_QSplitter(QString group_name);
    //---
    void load_QCheckBox(QString filename, QString group_name);
    void load_QComboBox(QString filename, QString group_name);
    void load_QPushButton(QString filename, QString group_name);
    void load_QToolButton(QString filename, QString group_name);
    void load_QTextEdit(QString filename, QString group_name);
    void load_QLineEdit(QString filename, QString group_name);
    void load_QSpinBox(QString filename, QString group_name);
    void load_QDoubleSpinBox(QString filename, QString group_name);
    void load_QSlider(QString filename, QString group_name);
    void load_QSplitter(QString filename, QString group_name);
    //---
    void save_QCheckBox(QString group_name);
    void save_QComboBox(QString group_name);
    void save_QPushButton(QString group_name);
    void save_QToolButton(QString group_name);
    void save_QTextEdit(QString group_name);
    void save_QLineEdit(QString group_name);
    void save_QSpinBox(QString group_name);
    void save_QDoubleSpinBox(QString group_name);
    void save_QSlider(QString group_name);
    void save_QSplitter(QString group_name);
    //---
    void save_QCheckBox(QString filename, QString group_name);
    void save_QComboBox(QString filename, QString group_name);
    void save_QPushButton(QString filename, QString group_name);
    void save_QToolButton(QString filename, QString group_name);
    void save_QTextEdit(QString filename, QString group_name);
    void save_QLineEdit(QString filename, QString group_name);
    void save_QSpinBox(QString filename, QString group_name);
    void save_QDoubleSpinBox(QString filename, QString group_name);
    void save_QSlider(QString filename, QString group_name);
    void save_QSplitter(QString filename, QString group_name);
    //---

    //---
    void mark_spinbox(QString property_name,
                      QString prefix);
    void mark_doublespinbox(QString property_name,
                            QString prefix);
    void mark_combobox(QString property_name,
                       QString prefix);
    //---
    bool check_id_pet(QString filename,
                      uint32_t pid);
    //---
    void save_id_pet(QString filename,
                     uint32_t pid);
    //---

    //---
    static int messagebox_noicon(const QString title,
                                 const QString text,
                                 unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_info(const QString title,
                               const QString text,
                               unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_warning(const QString title,
                                  const QString text,
                                  unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_critical(const QString title,
                                   const QString text,
                                   unsigned int width = MESSAGEBOX_WIDTH);
    static int messagebox_question(const QString title,
                                   const QString text,
                                   unsigned int width = MESSAGEBOX_WIDTH);
    //---
    void print_event_flag_log(void);

    //TODO инкапсуляцию надо бы
    quint64 cnt_events = 0;

    quint64 cnt_events_energy_filtered = 0;

    quint64 cnt_events_address_filtered = 0;
    quint64 cnt_events_min_energy_filtered = 0;
    quint64 cnt_events_max_energy_filtered = 0;

    quint64 bad_flags = 0;
    quint64 all_filtered = 0;

    quint64 cnt_PET_EVENT_FLAG_FAULT_filtered = 0;
    quint64 cnt_PET_EVENT_FLAG_TS_INVALID_filtered = 0;
    quint64 cnt_PET_EVENT_FLAG_GATE_OVERLAY_filtered = 0;
    quint64 cnt_PET_EVENT_FLAG_QDC_OVERFLOW_filtered = 0;
    quint64 cnt_PET_EVENT_FLAG_QDC_UNDERFLOW_filtered = 0;
    quint64 cnt_PET_EVENT_FLAG_ADC_OVERFLOW_filtered = 0;
    quint64 cnt_PET_EVENT_FLAG_ADC_UNDERFLOW_filtered = 0;
    quint64 cnt_PET_EVENT_FLAG_SYNC_filtered = 0;
    //---

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void send_net_log(const QString &);
    void receive_net_log(const QString &);

private slots:
    void log(const QString data);

    void debug(void);
    void show_objectname(void);

protected:
    bool eventFilter(QObject*, QEvent* event);
    void closeEvent(QCloseEvent *);

};
//--------------------------------------------------------------------------------
#endif
