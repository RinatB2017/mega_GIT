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
#include <QAbstractButton>
#include <QDoubleSpinBox>
#include <QStyleFactory>
#include <QGridLayout>
#include <QSpacerItem>
#include <QMessageBox>
#include <QMouseEvent>
#include <QToolButton>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QCheckBox>
#include <QSettings>
#include <QComboBox>
#include <QSplitter>
#include <QSpinBox>
#include <QVariant>
#include <QTimer>
#include <QDebug>
//--------------------------------------------------------------------------------
#include "mainwindow.hpp"
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
#include "defines.hpp"
//--------------------------------------------------------------------------------
#define PARAMS_GROUP_NAME   "Params"
//--------------------------------------------------------------------------------
MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent)
{
    connect_log(parent);
    block_wheel();

    QTimer::singleShot(100, this, SLOT(debug()));
}
//--------------------------------------------------------------------------------
MyWidget::~MyWidget()
{

}
//--------------------------------------------------------------------------------
void MyWidget::connect_log(QWidget *parent)
{
#ifdef NO_LOG
    connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
    connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(message(QString)), this, SLOT(log(QString)));
#else
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(message(QString)), parent, SIGNAL(message(QString)));
    }
    else
    {
        connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
        connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
        connect(this, SIGNAL(message(QString)), this, SLOT(log(QString)));
    }
#endif
}
//--------------------------------------------------------------------------------
void MyWidget::log(const QString data)
{
#ifdef NO_LOG
    MainWindow *mw = dynamic_cast<MainWindow *>(parentWidget());

    if(mw)
    {
        mw->set_status2_text(data);
    }
    else
    {
        qDebug() << data;
    }
#else
    qDebug() << data;
#endif
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void MyWidget::mark_spinbox(QString property_name,
                            QString prefix)
{
    QList<QSpinBox *> all_sb = findChildren<QSpinBox *>();
    foreach(QSpinBox *obj, all_sb)
    {
        if(!obj->objectName().isEmpty())
        {
            QString name = QString("%1_%2")
                    .arg(prefix)
                    .arg(obj->objectName());
            obj->setProperty(property_name.toStdString().c_str(), name);
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::mark_doublespinbox(QString property_name,
                                  QString prefix)
{
    QList<QDoubleSpinBox *> all_sb = findChildren<QDoubleSpinBox *>();
    foreach(QDoubleSpinBox *obj, all_sb)
    {
        if(!obj->objectName().isEmpty())
        {
            QString name = QString("%1_%2")
                    .arg(prefix)
                    .arg(obj->objectName());
            obj->setProperty(property_name.toStdString().c_str(), name);
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::mark_combobox(QString property_name,
                             QString prefix)
{
    QList<QComboBox *> all_sb = findChildren<QComboBox *>();
    foreach(QComboBox *obj, all_sb)
    {
        if(!obj->objectName().isEmpty())
        {
            QString name = QString("%1_%2")
                    .arg(prefix)
                    .arg(obj->objectName());
            obj->setProperty(property_name.toStdString().c_str(), name);
        }
    }
}
//--------------------------------------------------------------------------------
bool MyWidget::check_id_pet(QString filename,
                            uint32_t pid)
{
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);
    Q_CHECK_PTR(settings);

    uint32_t t_pid = settings->value("PID", -666).toInt();

    settings->deleteLater();

    if(t_pid != pid) return false;
    return true;
}
//--------------------------------------------------------------------------------
void MyWidget::save_id_pet(QString filename,
                           uint32_t pid)
{
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);
    Q_CHECK_PTR(settings);

    settings->setValue("PID", pid);

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void MyWidget::block_this_button(bool state)
{
    QObject *btn = (QObject *)sender();
    if(!btn) return;

    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(btn))
    {
        pb->setDisabled(state);
    }
}
//--------------------------------------------------------------------------------
void MyWidget::block_interface(bool state)
{
    QList<QAbstractButton *> all_obj = findChildren<QAbstractButton *>();
    foreach(QAbstractButton *obj, all_obj)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QComboBox *> all_cb = findChildren<QComboBox *>();
    foreach(QComboBox *obj, all_cb)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QDoubleSpinBox *> all_dsb = findChildren<QDoubleSpinBox *>();
    foreach(QDoubleSpinBox *obj, all_dsb)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QSpinBox *> all_sb = findChildren<QSpinBox *>();
    foreach(QSpinBox *obj, all_sb)
    {
        if(obj->property(FLAG_NO_BLOCK).toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::block_widget(const QString name, bool state)
{
    if(name.isEmpty())
    {
        return;
    }
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->objectName() == name)
        {
            if(obj->property(FLAG_NO_BLOCK).toBool() == false)
            {
                obj->setDisabled(state);
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool MyWidget::is_slot_exists(QWidget *obj, const char *slot_sign)
{
    if(obj == NULL) return false;
    if(obj->metaObject()->indexOfSlot(QMetaObject::normalizedSignature(qPrintable(slot_sign))) == -1) return false;
    return true;
}
//--------------------------------------------------------------------------------
bool MyWidget::set_param(QString group_name,
                         QString name,
                         QVariant value)
{
    if(name.isEmpty())  return false;

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    settings->setValue(name, value);
    settings->endGroup();

    settings->deleteLater();
    return true;
}
//--------------------------------------------------------------------------------
bool MyWidget::get_param(QString group_name,
                         QString name,
                         QVariant default_value,
                         QVariant *value)
{
    if(name.isEmpty())  return false;

    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);
    Q_CHECK_PTR(settings);

    settings->beginGroup(group_name);
    QVariant temp = settings->value(name, default_value);
    settings->endGroup();

    settings->deleteLater();
    *value = temp;
    return true;
}
//--------------------------------------------------------------------------------
QStringList MyWidget::get_all_param_name(QString group_name)
{
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList temp = settings->allKeys();
    settings->endGroup();

    settings->deleteLater();
    return temp;
}
//--------------------------------------------------------------------------------
void MyWidget::load_QCheckBox(QString group_name)
{
    QList<QCheckBox *> widgets = findChildren<QCheckBox *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QCheckBox *obj, widgets)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setChecked(settings->value("checked", false).toBool());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QCheckBox(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QCheckBox *> widgets = findChildren<QCheckBox *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QCheckBox *obj, widgets)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setChecked(settings->value("checked", false).toBool());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QCheckBox(QString group_name)
{
    QList<QCheckBox *> allobj = findChildren<QCheckBox *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QCheckBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("checked", QVariant(obj->isChecked()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QCheckBox(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QCheckBox *> allobj = findChildren<QCheckBox *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QCheckBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("checked", QVariant(obj->isChecked()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QComboBox(QString group_name)
{
    QList<QComboBox *> allobj = findChildren<QComboBox *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QComboBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setCurrentIndex(settings->value("currentindex", 0).toInt());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QComboBox(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QComboBox *> allobj = findChildren<QComboBox *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QComboBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setCurrentIndex(settings->value("currentindex", 0).toInt());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QComboBox(QString group_name)
{
    QList<QComboBox *> allobj = findChildren<QComboBox *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QComboBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("currentindex", QVariant(obj->currentIndex()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QComboBox(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QComboBox *> allobj = findChildren<QComboBox *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QComboBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("currentindex", QVariant(obj->currentIndex()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QPushButton(QString group_name)
{
    QList<QPushButton *> allobj = findChildren<QPushButton *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QPushButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setText(settings->value("text", "").toString());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QPushButton(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QPushButton *> allobj = findChildren<QPushButton *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QPushButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setText(settings->value("text", "").toString());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QPushButton(QString group_name)
{
    QList<QPushButton *> allobj = findChildren<QPushButton *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QPushButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->text()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QPushButton(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QPushButton *> allobj = findChildren<QPushButton *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QPushButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->text()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QToolButton(QString group_name)
{
    QList<QToolButton *> allobj = findChildren<QToolButton *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QToolButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setText(settings->value("text", "").toString());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QToolButton(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QToolButton *> allobj = findChildren<QToolButton *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QToolButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setText(settings->value("text", "").toString());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QToolButton(QString group_name)
{
    QList<QToolButton *> allobj = findChildren<QToolButton *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QToolButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->text()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QToolButton(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QToolButton *> allobj = findChildren<QToolButton *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QToolButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->text()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QTextEdit(QString group_name)
{
    QList<QTextEdit *> allobj = findChildren<QTextEdit *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QTextEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setText(settings->value("text", "").toString());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QTextEdit(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QTextEdit *> allobj = findChildren<QTextEdit *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QTextEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setText(settings->value("text", "").toString());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QTextEdit(QString group_name)
{
    QList<QTextEdit *> allobj = findChildren<QTextEdit *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QTextEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->toPlainText()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QTextEdit(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QTextEdit *> allobj = findChildren<QTextEdit *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QTextEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->toPlainText()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QLineEdit(QString group_name)
{
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QLineEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setText(settings->value("text", "").toString());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QLineEdit(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QLineEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setText(settings->value("text", "").toString());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QLineEdit(QString group_name)
{
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QLineEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->text()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QLineEdit(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QLineEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("text", QVariant(obj->text()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSpinBox(QString group_name)
{
    QList<QSpinBox *> allobj = findChildren<QSpinBox *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setValue(settings->value("value", 0).toInt());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSpinBox(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QSpinBox *> allobj = findChildren<QSpinBox *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setValue(settings->value("value", 0).toInt());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSpinBox(QString group_name)
{
    QList<QSpinBox *> allobj = findChildren<QSpinBox *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("value", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSpinBox(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QSpinBox *> allobj = findChildren<QSpinBox *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("value", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QDoubleSpinBox(QString group_name)
{
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setValue(settings->value("value", 0).toFloat());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QDoubleSpinBox(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setValue(settings->value("value", 0).toFloat());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QDoubleSpinBox(QString group_name)
{
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("value", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QDoubleSpinBox(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("value", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSlider(QString group_name)
{
    QList<QSlider *> allobj = findChildren<QSlider *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QSlider *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setSliderPosition(settings->value("position", 0).toDouble());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSlider(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QSlider *> allobj = findChildren<QSlider *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QSlider *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->setSliderPosition(settings->value("position", 0).toDouble());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSlider(QString group_name)
{
    QList<QSlider *> allobj = findChildren<QSlider *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QSlider *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("position", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSlider(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QSlider *> allobj = findChildren<QSlider *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QSlider *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("position", QVariant(obj->value()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSplitter(QString group_name)
{
    QList<QSplitter *> allobj = findChildren<QSplitter *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QSplitter *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->restoreState(settings->value("state", 0).toByteArray());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_QSplitter(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QSplitter *> allobj = findChildren<QSplitter *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    QStringList sl = settings->childGroups();
    foreach (QSplitter *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            foreach (QString group, sl)
            {
                if(group == obj->objectName())
                {
                    settings->beginGroup(group);
                    obj->restoreState(settings->value("state", 0).toByteArray());
                    settings->endGroup();
                }
            }
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSplitter(QString group_name)
{
    QList<QSplitter *> allobj = findChildren<QSplitter *>();
    QSettings *settings = new QSettings(ININAME, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QSplitter *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("state", QVariant(obj->saveState()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSplitter(QString filename, QString group_name)
{
    if(filename.isEmpty())
    {
        return;
    }

    QList<QSplitter *> allobj = findChildren<QSplitter *>();
    QSettings *settings = new QSettings(filename, QSettings::IniFormat);

    settings->beginGroup(group_name);
    foreach(QSplitter *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            settings->setValue("state", QVariant(obj->saveState()));
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::load_widgets(QString group_name)
{
#ifdef QT_DEBUG
    qDebug() << "load_widgets " << group_name;
#endif
#ifdef SAVE_WIDGETS_CHECKBOX
    load_QCheckBox(group_name);
#endif
#ifdef SAVE_WIDGETS_COMBOBOX
    load_QComboBox(group_name);
#endif
#ifdef SAVE_WIDGETS_PUSHBUTTON
    load_QPushButton(group_name);
#endif
#ifdef SAVE_WIDGETS_TOOLBUTTON
    load_QToolButton(group_name);
#endif
#ifdef SAVE_WIDGETS_SPINBOX
    load_QSpinBox(group_name);
#endif
#ifdef SAVE_WIDGETS_DOUBLESPINBOX
    load_QDoubleSpinBox(group_name);
#endif
#ifdef SAVE_WIDGETS_SLIDER
    load_QSlider(group_name);
#endif
#ifdef SAVE_WIDGETS_SPLITTER
    load_QSplitter(group_name);
#endif
#ifdef SAVE_WIDGETS_TEXTEDIT
    load_QTextEdit(group_name);
#endif
#ifdef SAVE_WIDGETS_LINEEDIT
    load_QLineEdit(group_name);
#endif
    Q_UNUSED(group_name)
}
//--------------------------------------------------------------------------------
void MyWidget::save_widgets(QString group_name)
{
#ifdef QT_DEBUG
    qDebug() << "save_widgets " << group_name;
#endif
#ifdef SAVE_WIDGETS_CHECKBOX
    save_QCheckBox(group_name);
#endif
#ifdef SAVE_WIDGETS_COMBOBOX
    save_QComboBox(group_name);
#endif
#ifdef SAVE_WIDGETS_PUSHBUTTON
    save_QPushButton(group_name);
#endif
#ifdef SAVE_WIDGETS_TOOLBUTTON
    save_QToolButton(group_name);
#endif
#ifdef SAVE_WIDGETS_SPINBOX
    save_QSpinBox(group_name);
#endif
#ifdef SAVE_WIDGETS_DOUBLESPINBOX
    save_QDoubleSpinBox(group_name);
#endif
#ifdef SAVE_WIDGETS_SLIDER
    save_QSlider(group_name);
#endif
#ifdef SAVE_WIDGETS_SPLITTER
    save_QSplitter(group_name);
#endif
#ifdef SAVE_WIDGETS_TEXTEDIT
    save_QTextEdit(group_name);
#endif
#ifdef SAVE_WIDGETS_LINEEDIT
    save_QLineEdit(group_name);
#endif
    Q_UNUSED(group_name)
}
//--------------------------------------------------------------------------------
void MyWidget::block_close(bool state)
{
    setProperty("flag_no_close", state);
}
//--------------------------------------------------------------------------------
void MyWidget::closeEvent(QCloseEvent *)
{
#ifdef QT_DEBUG
    qDebug() << "closeEvent";
#endif
}
//--------------------------------------------------------------------------------
void MyWidget::debug(void)
{
#ifdef QT_DEBUG
    show_objectname();
#endif
}
//--------------------------------------------------------------------------------
void MyWidget::block_wheel(void)
{
    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(!obj->objectName().isEmpty())
        {
            obj->installEventFilter(this);
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::load_config(void)
{
#ifdef QT_DEBUG
    qDebug() << "===========================";
    qDebug() << "MyWidget::load_config";
#endif

    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->property(TAG_SAVE).toBool())
        {
#ifdef QT_DEBUG
            qDebug() << QString("%1 %2")
                        .arg(obj->metaObject()->className())
                        .arg(obj->objectName());
#endif
        }
    }

#ifdef QT_DEBUG
    qDebug() << "===========================";
#endif
}
//--------------------------------------------------------------------------------
void MyWidget::save_config(void)
{
#ifdef QT_DEBUG
    qDebug() << "===========================";
    qDebug() << "MyWidget::save_config";
#endif

    QList<QWidget *> all_obj = findChildren<QWidget *>();
    foreach(QWidget *obj, all_obj)
    {
        if(obj->property(TAG_SAVE).toBool())
        {
#ifdef QT_DEBUG
            qDebug() << QString("[%1] %2")
                        .arg(obj->metaObject()->className())
                        .arg(obj->objectName());
#endif
            if(QString(obj->metaObject()->className()) == "QComboBox")      set_param(PARAMS_GROUP_NAME, obj->objectName(), ((QComboBox *)obj)->currentText());
            if(QString(obj->metaObject()->className()) == "QCheckBox")      set_param(PARAMS_GROUP_NAME, obj->objectName(), ((QCheckBox *)obj)->isChecked());
            if(QString(obj->metaObject()->className()) == "QDoubleSpinBox") set_param(PARAMS_GROUP_NAME, obj->objectName(), ((QDoubleSpinBox *)obj)->value());
            if(QString(obj->metaObject()->className()) == "QSpinBox")       set_param(PARAMS_GROUP_NAME, obj->objectName(), ((QSpinBox *)obj)->value());
            if(QString(obj->metaObject()->className()) == "QLineEdit")      set_param(PARAMS_GROUP_NAME, obj->objectName(), ((QLineEdit *)obj)->text());
            if(QString(obj->metaObject()->className()) == "QTextEdit")      set_param(PARAMS_GROUP_NAME, obj->objectName(), ((QTextEdit *)obj)->toPlainText());
        }
    }

#ifdef QT_DEBUG
    qDebug() << "===========================";
#endif
}
//--------------------------------------------------------------------------------
void MyWidget::show_objectname(void)
{
    QList<QAbstractSpinBox *> all_sb = findChildren<QAbstractSpinBox *>();
    foreach(QAbstractSpinBox *obj, all_sb)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QTextEdit *> all_te = findChildren<QTextEdit *>();
    foreach(QTextEdit *obj, all_te)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QLineEdit *> all_le = findChildren<QLineEdit *>();
    foreach(QLineEdit *obj, all_le)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QComboBox *> all_cb = findChildren<QComboBox *>();
    foreach(QComboBox *obj, all_cb)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QCheckBox *> all_chb = findChildren<QCheckBox *>();
    foreach(QCheckBox *obj, all_chb)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QAbstractButton *> all_btn = findChildren<QAbstractButton *>();
    foreach(QAbstractButton *obj, all_btn)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
    QList<QAbstractSlider *> all_sl = findChildren<QAbstractSlider *>();
    foreach(QAbstractSlider *obj, all_sl)
    {
        if(obj->objectName().isEmpty() == false)
        {
            obj->setToolTip(obj->objectName());
        }
    }
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
//    QStyle *style = QStyleFactory::create("Fusion");
//    if(style)
//    {
//        msgBox.setStyle(style);
//    }
//--------------------------------------------------------------------------------
int MyWidget::messagebox_noicon(const QString title,
                                const QString text,
                                unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(ICON_PROGRAMM));
    msgBox.setIcon(QMessageBox::NoIcon);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyWidget::messagebox_info(const QString title,
                              const QString text,
                              unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(ICON_PROGRAMM));
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyWidget::messagebox_question(const QString title,
                                  const QString text,
                                  unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(ICON_PROGRAMM));
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyWidget::messagebox_critical(const QString title,
                                  const QString text,
                                  unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(ICON_PROGRAMM));
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
int MyWidget::messagebox_warning(const QString title,
                                 const QString text,
                                 unsigned int width)
{
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(ICON_PROGRAMM));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setWindowTitle(title);
    msgBox.setText(text);
    msgBox.setStandardButtons(QMessageBox::Ok);
    QSpacerItem* horizontalSpacer = new QSpacerItem(width, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)msgBox.layout();
    layout->addItem(horizontalSpacer, layout->rowCount(), 0, 1, layout->columnCount());
    return msgBox.exec();
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
void MyWidget::print_event_flag_log(void)
{
    bad_flags = 0;
    bad_flags += cnt_PET_EVENT_FLAG_FAULT_filtered;
    bad_flags += cnt_PET_EVENT_FLAG_TS_INVALID_filtered;
    bad_flags += cnt_PET_EVENT_FLAG_GATE_OVERLAY_filtered;
    bad_flags += cnt_PET_EVENT_FLAG_QDC_OVERFLOW_filtered;
    bad_flags += cnt_PET_EVENT_FLAG_QDC_UNDERFLOW_filtered;
    bad_flags += cnt_PET_EVENT_FLAG_ADC_OVERFLOW_filtered;
    bad_flags += cnt_PET_EVENT_FLAG_ADC_UNDERFLOW_filtered;
    bad_flags += cnt_PET_EVENT_FLAG_SYNC_filtered;

    all_filtered = 0;
    all_filtered += bad_flags;
    all_filtered += cnt_events_address_filtered;
    all_filtered += cnt_events_min_energy_filtered;
    all_filtered += cnt_events_max_energy_filtered;

    //---
    float cnt_PET_EVENT_FLAG_FAULT_filtered_perc            = (float)cnt_PET_EVENT_FLAG_FAULT_filtered / (float)bad_flags * 100.0f;
    float cnt_PET_EVENT_FLAG_TS_INVALID_filtered_perc       = (float)cnt_PET_EVENT_FLAG_TS_INVALID_filtered / (float)bad_flags * 100.0f;
    float cnt_PET_EVENT_FLAG_GATE_OVERLAY_filtered_perc     = (float)cnt_PET_EVENT_FLAG_GATE_OVERLAY_filtered / (float)bad_flags * 100.0f;
    float cnt_PET_EVENT_FLAG_QDC_OVERFLOW_filtered_perc     = (float)cnt_PET_EVENT_FLAG_QDC_OVERFLOW_filtered / (float)bad_flags * 100.0f;
    float cnt_PET_EVENT_FLAG_QDC_UNDERFLOW_filtered_perc    = (float)cnt_PET_EVENT_FLAG_QDC_UNDERFLOW_filtered / (float)bad_flags * 100.0f;
    float cnt_PET_EVENT_FLAG_ADC_OVERFLOW_filtered_perc     = (float)cnt_PET_EVENT_FLAG_ADC_OVERFLOW_filtered / (float)bad_flags * 100.0f;
    float cnt_PET_EVENT_FLAG_ADC_UNDERFLOW_filtered_perc    = (float)cnt_PET_EVENT_FLAG_ADC_UNDERFLOW_filtered / (float)bad_flags * 100.0f;
    float cnt_PET_EVENT_FLAG_SYNC_filtered_perc             = (float)cnt_PET_EVENT_FLAG_SYNC_filtered / (float)bad_flags * 100.0f;
    float all_filtered_perc =  (float)all_filtered / (float)cnt_events * 100.0f;
    //---

    emit info("Анализ событий (флаги):");
    emit info(QString("    Сбой работы устройства:             %1 (%2%)")
              .arg(cnt_PET_EVENT_FLAG_FAULT_filtered)
              .arg((cnt_PET_EVENT_FLAG_FAULT_filtered_perc > 0.01) ? QString("%1").arg(cnt_PET_EVENT_FLAG_FAULT_filtered_perc, 0, 'f', 2) : "< 0.01"));
    emit info(QString("    Некорректная метка времени:         %1 (%2%)")
              .arg(cnt_PET_EVENT_FLAG_TS_INVALID_filtered)
              .arg((cnt_PET_EVENT_FLAG_TS_INVALID_filtered_perc > 0.01) ? QString("%1").arg(cnt_PET_EVENT_FLAG_TS_INVALID_filtered_perc, 0, 'f', 2) : "< 0.01"));
    emit info(QString("    Наложение событий:                  %1 (%2%)")
              .arg(cnt_PET_EVENT_FLAG_GATE_OVERLAY_filtered)
              .arg((cnt_PET_EVENT_FLAG_GATE_OVERLAY_filtered_perc > 0.01) ? QString("%1").arg(cnt_PET_EVENT_FLAG_GATE_OVERLAY_filtered_perc, 0, 'f', 2) : "< 0.01"));
    emit info(QString("    Переполнение координатных сигналов: %1 (%2%)")
              .arg(cnt_PET_EVENT_FLAG_QDC_OVERFLOW_filtered)
              .arg((cnt_PET_EVENT_FLAG_QDC_OVERFLOW_filtered_perc > 0.01) ? QString("%1").arg(cnt_PET_EVENT_FLAG_QDC_OVERFLOW_filtered_perc, 0, 'f', 2) : "< 0.01"));
    emit info(QString("    Опустошения координатных сигналов:  %1 (%2%)")
              .arg(cnt_PET_EVENT_FLAG_QDC_UNDERFLOW_filtered)
              .arg((cnt_PET_EVENT_FLAG_QDC_UNDERFLOW_filtered_perc > 0.01) ? QString("%1").arg(cnt_PET_EVENT_FLAG_QDC_UNDERFLOW_filtered_perc, 0, 'f', 2) : "< 0.01"));
    emit info(QString("    Переполнение АЦП:                   %1 (%2%)")
              .arg(cnt_PET_EVENT_FLAG_ADC_OVERFLOW_filtered)
              .arg((cnt_PET_EVENT_FLAG_ADC_OVERFLOW_filtered_perc > 0.01) ? QString("%1").arg(cnt_PET_EVENT_FLAG_ADC_OVERFLOW_filtered_perc, 0, 'f', 2) : "< 0.01"));
    emit info(QString("    Опустошение АЦП:                    %1 (%2%)")
              .arg(cnt_PET_EVENT_FLAG_ADC_UNDERFLOW_filtered)
              .arg((cnt_PET_EVENT_FLAG_ADC_UNDERFLOW_filtered_perc > 0.01) ? QString("%1").arg(cnt_PET_EVENT_FLAG_ADC_UNDERFLOW_filtered_perc, 0, 'f', 2) : "< 0.01"));
    emit info(QString("    Признак кардиосинхронизации:        %1 (%2%)")
              .arg(cnt_PET_EVENT_FLAG_SYNC_filtered)
              .arg((cnt_PET_EVENT_FLAG_SYNC_filtered_perc > 0.01) ? QString("%1").arg(cnt_PET_EVENT_FLAG_SYNC_filtered_perc, 0, 'f', 2) : "< 0.01"));
    emit info(QString("Отклонено событий:                      %1 (%2%)")
              .arg(all_filtered)
              .arg((all_filtered_perc > 0.01) ? QString("%1").arg(all_filtered_perc, 0, 'f', 2) : "< 0.01"));
}
//--------------------------------------------------------------------------------
bool MyWidget::eventFilter(QObject*, QEvent* event)
{
    QMouseEvent *mouseEvent = (QMouseEvent *) event;
    //---
    if(mouseEvent->type() == QMouseEvent::Wheel)
    {
        return true;
    }
    return false;
}
//--------------------------------------------------------------------------------
