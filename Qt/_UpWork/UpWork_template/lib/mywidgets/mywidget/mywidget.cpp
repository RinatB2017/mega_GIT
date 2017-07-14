/*********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#include <QAbstractButton>
#include <QDoubleSpinBox>
#include <QMouseEvent>
#include <QMessageBox>
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
MyWidget::MyWidget(QWidget *parent) : QWidget(parent)
{
    connect_log();
#ifdef QT_DEBUG
    qDebug() << "MyWidget()";
    QTimer::singleShot(100, this, SLOT(debug()));
#endif
}
//--------------------------------------------------------------------------------
MyWidget::~MyWidget()
{
#ifdef QT_DEBUG
    qDebug() << "~MyWidget()";
#endif
}
//--------------------------------------------------------------------------------
// основная функция
void MyWidget::connect_log(void)
{
#ifdef NO_LOG
    connect(this, SIGNAL(info(QString)),    this, SLOT(log(QString)));
    connect(this, SIGNAL(debug(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(error(QString)),   this, SLOT(log(QString)));
    connect(this, SIGNAL(message(QString)), this, SLOT(log(QString)));
#else
    if(parentWidget())
    {
        connect(this, SIGNAL(info(QString)),    parentWidget(), SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parentWidget(), SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parentWidget(), SIGNAL(error(QString)));
        connect(this, SIGNAL(message(QString)), parentWidget(), SIGNAL(message(QString)));

        connect(this,               SIGNAL(syslog(int,QString,QString)),
                parentWidget(),     SIGNAL(syslog(int,QString,QString)));
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
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    if(mw)
    {
#ifndef NO_STATUSBAR
        mw->set_status2_text(data);
#endif
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
void MyWidget::block_this_button(bool state)
{
    if (QAbstractButton *pb=qobject_cast<QAbstractButton *>(sender()))
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
        if(obj->property("no_block").toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QComboBox *> all_cb = findChildren<QComboBox *>();
    foreach(QComboBox *obj, all_cb)
    {
        if(obj->property("no_block").toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QDoubleSpinBox *> all_dsb = findChildren<QDoubleSpinBox *>();
    foreach(QDoubleSpinBox *obj, all_dsb)
    {
        if(obj->property("no_block").toBool() == false)
        {
            obj->setDisabled(state);
        }
    }
    //---
    QList<QSpinBox *> all_sb = findChildren<QSpinBox *>();
    foreach(QSpinBox *obj, all_sb)
    {
        if(obj->property("no_block").toBool() == false)
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
            if(obj->property("no_block").toBool() == false)
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
void MyWidget::load_config(void)
{
    qDebug() << "load_config";
    load_widgets("MyWidget");
}
//--------------------------------------------------------------------------------
void MyWidget::save_config(void)
{
    qDebug() << "save_config";
    save_widgets("MyWidget");
}
//--------------------------------------------------------------------------------
bool MyWidget::set_param(QString group_name, QString name, QVariant value)
{
    if(name.isEmpty())  return false;

    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return false;
    }
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

    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return false;
    }
    settings->beginGroup(group_name);
    QVariant temp = settings->value(name, default_value);
    settings->endGroup();

    settings->deleteLater();
    *value = temp;
    return true;
}
//--------------------------------------------------------------------------------
QStringList MyWidget::get_all_param_name(void)
{
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return QStringList();
    }
    settings->beginGroup(PARAMS_GROUP_NAME);
    QStringList temp = settings->allKeys();
    settings->endGroup();

    settings->deleteLater();
    return temp;
}
//--------------------------------------------------------------------------------
void MyWidget::load_QCheckBox(QString group_name)
{
    QList<QCheckBox *> widgets = findChildren<QCheckBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QCheckBox *obj, widgets)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setChecked(settings->value("checked", false).toBool());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QCheckBox(QString group_name)
{
    QList<QCheckBox *> allobj = findChildren<QCheckBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

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
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QComboBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setCurrentIndex(settings->value("currentindex", 0).toInt());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QComboBox(QString group_name)
{
    QList<QComboBox *> allobj = findChildren<QComboBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

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
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QPushButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setText(settings->value("text", "").toString());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QPushButton(QString group_name)
{
    QList<QPushButton *> allobj = findChildren<QPushButton *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

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
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QToolButton *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setText(settings->value("text", "").toString());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QToolButton(QString group_name)
{
    QList<QToolButton *> allobj = findChildren<QToolButton *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

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
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QTextEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setText(settings->value("text", "").toString());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QTextEdit(QString group_name)
{
    QList<QTextEdit *> allobj = findChildren<QTextEdit *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

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
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QLineEdit *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setText(settings->value("text", "").toString());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QLineEdit(QString group_name)
{
    QList<QLineEdit *> allobj = findChildren<QLineEdit *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

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
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setValue(settings->value("value", 0).toInt());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSpinBox(QString group_name)
{
    QList<QSpinBox *> allobj = findChildren<QSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

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
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QDoubleSpinBox *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setValue(settings->value("value", 0).toFloat());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QDoubleSpinBox(QString group_name)
{
    QList<QDoubleSpinBox *> allobj = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

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
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QSlider *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->setSliderPosition(settings->value("position", 0).toDouble());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSlider(QString group_name)
{
    QList<QSlider *> allobj = findChildren<QSlider *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

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
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

    settings->beginGroup(group_name);
    foreach (QSplitter *obj, allobj)
    {
        if(!obj->objectName().isEmpty())
        {
            settings->beginGroup(obj->objectName());
            obj->restoreState(settings->value("state", 0).toByteArray());
            settings->endGroup();
        }
    }
    settings->endGroup();

    settings->deleteLater();
}
//--------------------------------------------------------------------------------
void MyWidget::save_QSplitter(QString group_name)
{
    QList<QSplitter *> allobj = findChildren<QSplitter *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);
    if(settings == nullptr)
    {
        return;
    }

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
    Q_UNUSED(group_name);
}
//--------------------------------------------------------------------------------
void MyWidget::save_widgets(QString group_name)
{
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
    Q_UNUSED(group_name);
}
//--------------------------------------------------------------------------------
void MyWidget::block_close(bool state)
{
    setProperty("flag_no_close", state);
}
//--------------------------------------------------------------------------------
void MyWidget::closeEvent(QCloseEvent *)
{
    qDebug() << "closeEvent";
}
//--------------------------------------------------------------------------------
void MyWidget::debug(void)
{
    show_objectname();
    block_wheel();
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
    msgBox.setDefaultButton(QMessageBox::Yes);
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
void MyWidget::load_checkBox(QList<check_box> data)
{
    QList<QCheckBox *> all_chb = findChildren<QCheckBox *>();

    foreach (check_box cb_obj, data) {
        foreach(QCheckBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj.name)
            {
                obj->setChecked(cb_obj.default_value);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::load_comboBox(QList<combo_box> data)
{
    QList<QComboBox *> all_chb = findChildren<QComboBox *>();

    foreach (combo_box cb_obj, data) {
        foreach(QComboBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj.name)
            {
                obj->setCurrentIndex(cb_obj.default_value);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::load_spinBox(QList<spin_box> data)
{
    QList<QSpinBox *> all_chb = findChildren<QSpinBox *>();

    foreach (spin_box cb_obj, data) {
        foreach(QSpinBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj.name)
            {
                obj->setValue(cb_obj.default_value);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::load_d_spinBox(QList<d_spin_box> data)
{
    QList<QDoubleSpinBox *> all_chb = findChildren<QDoubleSpinBox *>();

    foreach (d_spin_box cb_obj, data) {
        foreach(QDoubleSpinBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj.name)
            {
                obj->setValue(cb_obj.default_value);
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::save_checkBox(QString group_name, QList<QCheckBox *> data)
{
    QList<QCheckBox *> all_chb = findChildren<QCheckBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    settings->beginGroup(group_name);

    foreach (QCheckBox *cb_obj, data) {
        foreach(QCheckBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj->objectName())
            {
                settings->beginGroup(obj->objectName());
                settings->setValue("checked", QVariant(obj->isChecked()));
                settings->endGroup();
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::save_comboBox(QString group_name, QList<QComboBox *> data)
{
    QList<QComboBox *> all_chb = findChildren<QComboBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    settings->beginGroup(group_name);

    foreach (QComboBox *cb_obj, data) {
        foreach(QComboBox *obj, all_chb)
        {
            if(obj->objectName() == cb_obj->objectName())
            {
                settings->beginGroup(obj->objectName());
                settings->setValue("index", QVariant(obj->currentIndex()));
                settings->endGroup();
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::save_spinBox(QString group_name, QList<QSpinBox *> data)
{
    QList<QSpinBox *> all_chb = findChildren<QSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    settings->beginGroup(group_name);

    foreach (QSpinBox *sb_obj, data) {
        foreach(QSpinBox *obj, all_chb)
        {
            if(obj->objectName() == sb_obj->objectName())
            {
                settings->beginGroup(obj->objectName());
                settings->setValue("value", QVariant(obj->value()));
                settings->endGroup();
            }
        }
    }
}
//--------------------------------------------------------------------------------
void MyWidget::save_d_spinBox(QString group_name, QList<QDoubleSpinBox *> data)
{
    QList<QDoubleSpinBox *> all_chb = findChildren<QDoubleSpinBox *>();
    QSettings *settings = new QSettings(QString("%1%2").arg(APPNAME).arg(".ini"), QSettings::IniFormat);

    settings->beginGroup(group_name);

    foreach (QDoubleSpinBox *sb_obj, data) {
        foreach(QDoubleSpinBox *obj, all_chb)
        {
            if(obj->objectName() == sb_obj->objectName())
            {
                settings->beginGroup(obj->objectName());
                settings->setValue("value", QVariant(obj->value()));
                settings->endGroup();
            }
        }
    }
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
bool MyWidget::close(void)
{
    return is_close;
}
//--------------------------------------------------------------------------------
