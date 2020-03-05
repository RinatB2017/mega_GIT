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
#include "ui_for_tests_mainbox.h"
//--------------------------------------------------------------------------------
#include "mysplashscreen.hpp"
#include "mymainwindow.hpp"
#include "for_tests_mainbox.hpp"
#include "defines.hpp"
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

    createTestBar();

    ui->horizontalSlider->setRange(0, 1000);

    connect(ui->sb_1,   SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->sb_2,   SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->sb_res, SIGNAL(valueChanged(int)),  this,   SLOT(check_in()));
    connect(ui->btn_ok, SIGNAL(clicked(bool)),      this,   SLOT(victory()));
    check_in();

    ui->toolButton->setIcon((QIcon(qApp->style()->standardIcon(QStyle::SP_TrashIcon))));

#if 1
    ui->btn_set_time->setIcon(qApp->style()->standardIcon(QStyle::SP_BrowserReload));
    ui->btn_run_timer->setIcon(qApp->style()->standardIcon(QStyle::SP_MediaPlay));

    connect(ui->btn_set_time,   &QToolButton::clicked,  this,   &MainBox::set_time);
    connect(ui->btn_run_timer,  &QToolButton::clicked,  this,   &MainBox::run_timer);
#endif

#if 1
    //setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
#else
    if(sizeHint().height() > 0)
    {
        setMinimumHeight(sizeHint().height());
    }
#endif
    load_widgets();

    //TODO xxx
    ui->btn_ok->setProperty("xxx", 1);
}
//--------------------------------------------------------------------------------
void MainBox::set_time(void)
{
    QTime nt;
    nt.setHMS(0, 0, 10);
    ui->timer_widget->set_timer(nt);
}
//--------------------------------------------------------------------------------
void MainBox::run_timer(void)
{
    ui->timer_widget->start();
}
//--------------------------------------------------------------------------------
void MainBox::check_in(void)
{
    int a = ui->sb_1->value();
    int b = ui->sb_2->value();
    int c = ui->sb_res->value();

    bool res = ((a + b) == c);
    ui->btn_ok->setEnabled(res);
}
//--------------------------------------------------------------------------------
void MainBox::victory(void)
{
    messagebox_info("Info", "Victory!");
}
//--------------------------------------------------------------------------------
void MainBox::createTestBar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);

    commands.clear();
    commands.append({ ID_TEST_0, "test 0", &MainBox::test_0 });
    commands.append({ ID_TEST_1, "test 1", &MainBox::test_1 });
    commands.append({ ID_TEST_2, "test 2", &MainBox::test_2 });
    commands.append({ ID_TEST_3, "test 3", &MainBox::test_3 });
    commands.append({ ID_TEST_4, "test 4", &MainBox::test_4 });
    commands.append({ ID_TEST_5, "test 5", &MainBox::test_5 });
    commands.append({ ID_TEST_6, "test 6", nullptr });

    QToolBar *testbar = new QToolBar("testbar");
    testbar->setObjectName("testbar");
    mw->addToolBar(Qt::TopToolBarArea, testbar);

    QCheckBox *cb_block = new QCheckBox("block");
    cb_block->setObjectName("cb_block");
    testbar->addWidget(cb_block);

    cb_test = new QComboBox(this);
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

    connect(cb_block, SIGNAL(clicked(bool)), cb_test,           SLOT(setDisabled(bool)));
    connect(cb_block, SIGNAL(clicked(bool)), btn_choice_test,   SLOT(setDisabled(bool)));

    mw->add_windowsmenu_action(testbar, testbar->toggleViewAction());
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;
    foreach (CMD command, commands)
    {
        if(command.cmd == cmd)
        {
            typedef bool (MainBox::*function)(void);
            function x;
            x = command.func;
            if(x)
            {
                (this->*x)();
            }
            else
            {
                emit error("no func");
            }

            return;
        }
    }
}
//--------------------------------------------------------------------------------
void MainBox::inFunc(QPushButton *btn, saveSlot slot)
{
    connect(btn,    &QPushButton::clicked,  this,   slot);
}
//--------------------------------------------------------------------------------
void MainBox::s_inFunc(void)
{
    emit trace(Q_FUNC_INFO);
    QMessageBox::information(nullptr, "", "info");
}
//--------------------------------------------------------------------------------
int MainBox::get_cnt(void)
{
    emit trace(Q_FUNC_INFO);
    return qrand() % 10;
}
//--------------------------------------------------------------------------------
void MainBox::test_validator(void)
{
    /* Create a string for a regular expression */
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    /*
     * Create a regular expression with a string
     * as a repeating element
     */
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    /*
     * Create a validation regular expression
     * using a regular expression
     */
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);

    /* Set Validator on QLineEdit */
    QLineEdit *lineEdit = new QLineEdit;
    lineEdit->setValidator(ipValidator);
    lineEdit->show();
}
//--------------------------------------------------------------------------------
bool MainBox::create_color_block(int width,
                                 int height,
                                 int w_border,
                                 QColor color_border,
                                 QColor color,
                                 QString path,
                                 QString filename)
{
    QPixmap *pixmap = new QPixmap(width, height);
    QPainter *painter = new QPainter(pixmap);
    painter->fillRect(0,
                      0,
                      pixmap->width(),
                      pixmap->height(),
                      QBrush(color_border));
    painter->fillRect(w_border,
                      w_border,
                      pixmap->width()-w_border*2,
                      pixmap->height()-w_border*2,
                      QBrush(color));

    bool ok = pixmap->save(path + filename);
    if(ok)
    {
        emit info(QString("File %1 successfully created").arg(path + filename));
    }
    else
    {
        emit error(QString("Error create %1").arg(path + filename));
    }
    return ok;
}
//--------------------------------------------------------------------------------
bool MainBox::test_assert(int value)
{
    Q_ASSERT(value > 5);    // если значение > 5, то всё нормально
    return value != 0;
}
//--------------------------------------------------------------------------------
void MainBox::print_mp(QWidget *widget)
{
    emit error(QString("objectName %1").arg(widget->objectName()));

    const QMetaObject* metaObject = widget->metaObject();
    emit info(QString("propertyCount  = %1").arg(metaObject->propertyCount()));
    emit info(QString("propertyOffset = %1").arg(metaObject->propertyOffset()));
    emit error("---");
    for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
    {
        emit error(QString("%1").arg(QString::fromLatin1(metaObject->property(i).name())));
    }
    emit error("---");
    emit info(QString("methodCount    = %1").arg(metaObject->methodCount()));
    emit info(QString("methodOffset   = %1").arg(metaObject->methodOffset()));
    emit error("---");
    for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
    {
        emit error(QString("%1").arg(QString::fromLatin1(metaObject->method(i).methodSignature())));
    }
    emit error("---");
}
//--------------------------------------------------------------------------------
bool MainBox::test_0(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_0()");

#if 1
    QWidgetList widgets = qApp->allWidgets();
    foreach (QWidget *widget, widgets)
    {
        QString oname = widget->objectName();
#if 1
        //if(oname == "rb_test")
        //if(oname == "cb_checkbox_test")
        //if(oname == "comboBox")
        if(oname == "btn_ok")
        {
            print_mp(widget);
            return true;
        }
#endif
        if(oname.left(1) != "q")
        {
            //emit info(oname);
            //widget->>met

#if 0
            const QMetaObject* metaObject = widget->metaObject();
            QStringList properties;
            for(int i = metaObject->propertyOffset(); i < metaObject->propertyCount(); ++i)
            {
                QString prop = QString::fromLatin1(metaObject->property(i).name());
                emit debug(prop);
                if(prop == "value")
                {
                    emit error(QString("value %1").arg(QString::fromLatin1(metaObject->property(i).name())));
                }
                if(prop == "checked")
                {
                    emit error(QString("checked %1").arg(QString::fromLatin1(metaObject->property(i).name())));
                }
            }
#endif

#if 0
            bool ok = false;
            int v = widget->property("value").toInt(&ok);
            if(ok)
            {
                emit error(QString("value = %1").arg(v));
            }
#endif

#if 0
            const QMetaObject* metaObject = widget->metaObject();
            QStringList methods;
            for(int i = metaObject->methodOffset(); i < metaObject->methodCount(); ++i)
            {
                methods << QString::fromLatin1(metaObject->method(i).methodSignature());
            }
            foreach (QString str, methods)
            {
                emit error(QString("   %1").arg(str));
            }
#endif
        }
    }
#endif

#if 0
    setProperty("is_exit", true);
#endif

#if 0
    emit info("Info");
    emit debug("Debug");
    emit error("Error");
    emit trace("Trace");
#endif

#if 0
    emit info(QDir::currentPath());

    //TODO сначала модель присваивается виджету для просмотра
    QFileSystemModel *model = new QFileSystemModel;
    QTreeView *tree = new QTreeView();
    tree->setModel(model);
    tree->setColumnHidden(1, true);
    tree->setColumnHidden(2, true);
    tree->setColumnHidden(3, true);

    //TODO и только потом переходим на нужный каталог
    QModelIndex idx = model->setRootPath(QDir::currentPath());
    tree->setRootIndex(idx);

    tree->show();
#endif

#if 0
    //ui->cb_checkbox_test->setDisabled(ui->cb_checkbox_test->isEnabled());
    ui->rb_test->setDisabled(ui->rb_test->isEnabled());
#endif

#if 0
    emit info(metaObject()->className());
#endif

#if 0
    const QString program = "kpat";
    const QStringList arguments = QStringList();
    QProcess process;
    process.setWorkingDirectory(".");
    process.start(program, arguments);
    bool res = process.waitForFinished(-1);
    emit info(QString("programm return %1").arg(res ? "true" : "false"));
#endif

#if 0
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_CHECK_PTR(mw);
    QList<QWidget *> l_widgets = mw->findChildren<QWidget *>();
    emit info(QString("find %1 widgets").arg(l_widgets.count()));

    foreach (QWidget *w, l_widgets)
    {
        if(w->objectName().left(3) != "qt_")
        {
            emit info(w->objectName());
        }
    }
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_1(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_1()");

#if 0
    setProperty("is_exit", false);
#endif


#if 0
    emit info("Текст <font style=\"color:red\">красный</font>");
#endif
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_2(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_2()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_3(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_3()");

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_4(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_4()");
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_5(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test_5()");
    return true;
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
    emit debug("load_setting !");
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
    emit debug("save_setting !");
}
//--------------------------------------------------------------------------------
