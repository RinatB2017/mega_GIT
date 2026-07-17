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
#include "for_tests_mainbox.hpp"
//--------------------------------------------------------------------------------
MainBox::MainBox(QWidget *parent,
                 MySplashScreen *splash) :
    MainBox_GUI(parent),
    splash(splash),
    m_thread(nullptr)
{
    init();
}
//--------------------------------------------------------------------------------
MainBox::~MainBox()
{
    stopThread();

    // m_sshServer->stopServer();
    // delete m_sshServer;

    m_remoteConsole->stopServer();

    save_widgets();
}
//--------------------------------------------------------------------------------
void MainBox::init(void)
{
    create_test_bar();

    // add_lcd_clock();
    // add_digital_clock(false);
    add_digital_clock();

    // Создаем поток один раз на всё время жизни окна
    m_thread = new QThread(this);
    m_worker = nullptr;

    // m_sshServer = new SimpleSshServer(2222, this);

    m_remoteConsole = new SimpleRemoteConsole(2222, this);

    load_widgets();
}
//--------------------------------------------------------------------------------
bool MainBox::set_theme_windows(void)
{
    QString filename = ":/themes_css/Theme (Windows).css";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error(QString("file [%1] not open").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::set_norton_commander(void)
{
    QString filename = ":/themes_qss/Norton Commander.qss";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error(QString("file [%1] not open").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::set_styles(void)
{
    QString filename = ":/themes_qss/styles.qss";
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly))
    {
        QByteArray ba = file.readAll();
        qApp->setStyleSheet(ba.data());
    }
    else
    {
        emit error(QString("file [%1] not open").arg(filename));
        return false;
    }
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::create_test_bar(void)
{
    MainWindow *mw = dynamic_cast<MainWindow *>(topLevelWidget());
    Q_ASSERT(mw);

    if(mw)
    {
        commands.clear(); int id = 0;
        commands.append({ id++, "run_cube_widget",      &MainBox::run_cube_widget });
        commands.append({ id++, "run_ogl_widget",       &MainBox::run_ogl_widget });
        commands.append({ id++, "test",                 &MainBox::test });
        commands.append({ id++, "test2",                &MainBox::test2 });
        commands.append({ id++, "test_style",           &MainBox::test_style });
        commands.append({ id++, "Theme (Windows).css",  &MainBox::set_theme_windows });
        commands.append({ id++, "Norton Commander.qss", &MainBox::set_norton_commander });
        commands.append({ id++, "styles.qss",           &MainBox::set_styles });

        QToolBar *testbar = new QToolBar("testbar");
        testbar->setObjectName("testbar");
        mw->addToolBar(Qt::TopToolBarArea, testbar);

        cb_block = new QCheckBox("block");
        cb_block->setObjectName("cb_block");
        testbar->addWidget(cb_block);

        sb_test = new QSpinBox(this);
        sb_test->setObjectName("sb_test");
        testbar->addWidget(sb_test);

        cb_test = new QComboBox(this);
        cb_test->setObjectName("cb_test");
        cb_test->setProperty(NO_SAVE, true);
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

        connect(btn_choice_test,    &QToolButton::clicked,  this,   &MainBox::choice_test);

        connect(cb_block,   &QCheckBox::clicked,    cb_test,            &QComboBox::setDisabled);
        connect(cb_block,   &QCheckBox::clicked,    btn_choice_test,    &QComboBox::setDisabled);
        connect(cb_block,   &QCheckBox::clicked,    sb_test,            &QComboBox::setDisabled);
    }
    else
    {
        emit error("mw not found!");
    }
}
//--------------------------------------------------------------------------------
void MainBox::choice_test(void)
{
    bool ok = false;
    int cmd = cb_test->itemData(cb_test->currentIndex(), Qt::UserRole).toInt(&ok);
    if(!ok) return;

    auto cmd_it = std::find_if(
                commands.begin(),
                commands.end(),
                [cmd](CMD command){ return command.cmd == cmd; }
            );
    if (cmd_it != commands.end())
    {
        typedef bool (MainBox::*function)(void);
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
    else
    {
        emit error("not found");
    }
}
//--------------------------------------------------------------------------------
template<typename T1, typename T2>
void MainBox::test_template(void)
{
    QList<T1 *> allle = findChildren<T1 *>();
    foreach (T1 *obj, allle)
    {
        obj->setDisabled(true);
    }
    QList<T2 *> allle2 = findChildren<T2 *>();
    foreach (T2 *obj, allle2)
    {
        obj->setDisabled(true);
    }
}
//--------------------------------------------------------------------------------
template<class T>
void MainBox::test_function(int delay)
{
    QTimer::singleShot(delay, [this]{
        T::test2();
    });
}
//--------------------------------------------------------------------------------
void MainBox::test_function2(bool (MainBox::*func)(void))
{
    typedef bool (MainBox::*function)(void);
    function f;
    f = func;

    block_interface(true);
    (this->*f)();
    block_interface(false);
}
//--------------------------------------------------------------------------------
void MainBox::heavy_function(bool *result)
{
    QElapsedTimer timer;
    double res = 0;

    emit info("heavy_function started");
    timer.start();
    for(int y=0; y<100000; y++)
    {
        for(int x=0; x<10000; x++)
        {
            res += (qSin(x) * qCos(y));
        }
    }
    emit info(QString("res: %1").arg(res));
    emit info(QString("Elapsed %1 msec").arg(timer.elapsed()));
    emit info("heavy_function function");

    *result = true;
}
//--------------------------------------------------------------------------------
void MainBox::calc_line(qreal center_x,
                        qreal center_y,
                        qreal angle,
                        qreal radius,
                        qreal *end_x,
                        qreal *end_y)
{
    //qDebug() << "calc_line" << center_x << center_y << angle << radius;

    qreal A = radius;
    qreal B = qCos(qDegreesToRadians(angle)) * A;
    qreal C = qSin(qDegreesToRadians(angle)) * A;

    //qDebug() << center_x + B << center_y + C;

    *end_x = center_x + B;
    *end_y = center_y + C;
}
//--------------------------------------------------------------------------------
bool MainBox::calc_norm(qreal x,
                        qreal y,
                        qreal w,
                        qreal h,
                        qreal *norm_x,
                        qreal *norm_y)
{
    if(x<0 || y<0) return false;
    if(w<0 || h<0) return false;
    if(x>w || y>h) return false;

    *norm_x = (x * 2.0) / w - 1.0;
    *norm_y = (y * 2.0) / h - 1.0;
    return true;
}
//--------------------------------------------------------------------------------
QImage MainBox::create_bone(int num)
{
    QImage image = QImage(512, 512, QImage::Format_ARGB32);
    image.fill(QColor(Qt::white));

    QPen pen;
    pen.setColor(QColor(Qt::black));
    pen.setWidth(5);

    QPainter painter;
    painter.begin(&image);
    painter.setPen(pen);
    painter.drawRect(0, 0, 511, 511);
    painter.setFont(QFont("Liberation Mono", 70));
    painter.drawText(250, 280, QString::number(num));
    painter.end();

    return image;
}
//--------------------------------------------------------------------------------
void MainBox::createWidgetByName(void)
{
    // 1. У нас есть только строка с именем класса
    QString className = "CyberWidget";

    // 2. Ищем мета-тип по его строковому имени
    QMetaType type = QMetaType::fromName(className.toUtf8().constData());

    if (type.isValid()) {
        // 3. Запрашиваем "паспорт" класса (QMetaObject) для этого типа
        const QMetaObject *metaObject = type.metaObject();

        if (metaObject) {
            // 4. МЫ СТРОИМ ОБЪЕКТ ПО СТРОКЕ!
            // Метод newInstance() вызывает конструктор, помеченный как Q_INVOKABLE
            QObject *obj = metaObject->newInstance();

            if (obj) {
                // Объект успешно создан. Приводим его к базовому типу QWidget
                QWidget *widget = qobject_cast<QWidget*>(obj);

                // Теперь с ним можно работать как с обычной формой:
                widget->setWindowTitle("Я создан по строке!");
                widget->resize(300, 200);
                widget->show();
            } else {
                emit error("Не удалось вызвать конструктор. Проверьте Q_INVOKABLE!");
            }
        }
    } else {
        emit error(QString("Класс с именем %1 не зарегистрирован в системе.")
                       .arg(className));
    }
}
//--------------------------------------------------------------------------------
void MainBox::startThread(void)
{
    if (m_worker != nullptr)
    {
        qDebug() << "Поток уже работает!";
        return;
    }

    m_worker = new Worker(); // Без родителя
    m_worker->moveToThread(m_thread);

    connect(m_thread, &QThread::started, m_worker, &Worker::doHeavyWork);
    connect(m_worker, &Worker::progressChanged, this, &MainBox::handleProgress);

    // Когда воркер завершит цикл, он сам отправит сигнал finished на свое удаление
    connect(m_worker, &Worker::finished, m_worker, &Worker::deleteLater);

    // Как только воркер физически сотрется, зануляем указатель
    connect(m_worker, &Worker::destroyed, this, [this](){ m_worker = nullptr; });

    if (!m_thread->isRunning())
    {
        m_thread->start();
    }
    else
    {
        QMetaObject::invokeMethod(m_worker, "doHeavyWork");
    }
}

void MainBox::stopThread(void)
{
    // Если воркера нет или поток уже спит, останавливать нечего
    if (!m_worker || !m_thread->isRunning()) return;

    qDebug() << "Запрос на экстренную остановку вычислений...";

    // 1. Вежливо просим воркера выйти из цикла for
    m_thread->requestInterruption();

    // 2. Просим сам поток завершить свой внутренний цикл ожидания событий Qt
    m_thread->quit();

    // 3. ЖЕЛЕЗНОЕ ОЖИДАНИЕ: Главный GUI-поток останавливается и ждет,
    // пока поток в Linux сделает последний вздох.
    // Это занимает долей миллисекунды, но гарантирует, что QThread не сотрется на бегу!
    m_thread->wait();

    // Чистим указатель, так как воркер удалится через deleteLater()
    m_worker = nullptr;

    qDebug() << "Поток безопасно остановлен. Программу можно закрывать.";
}

void MainBox::handleProgress(int value)
{
    qDebug() << "Прогресс, пойманный в интерфейсе:" << value << "%";
}
//--------------------------------------------------------------------------------
#include "test_classes.hpp"

bool MainBox::test(void)
{
    emit trace(Q_FUNC_INFO);

#if 1
    if (m_remoteConsole->startServer())
    {
        emit info("Удаленная консоль успешно запущена.");
    }
    else
    {
        emit error("Не удалось запустить сервер. Порт занят?");
    }
#endif

#if 0
    startThread();
#endif

#if 0
    if (m_sshServer->startServer())
    {
        emit info("==================================================");
        emit info("SSH-сервер успешно запущен на порту 2222.");
        emit info("Имя пользователя: admin");
        emit info("Пароль: my_secure_password");
        emit info("Для подключения из Linux используйте команду:");
        emit info("ssh admin@<IP_АДРЕС_WINDOWS_МАШИНЫ> -p 2222");
        emit info("==================================================");
    }
    else
    {
        emit error("Не удалось запустить SSH-сервер. Проверьте приватный RSA-ключ.");
        return false;
    }
    emit info("SSH-сервер запушен");
#endif

#if 0
    qRegisterMetaType<CyberWidget>("CyberWidget");
    createWidgetByName();
#endif

#if 0
    // RotateWidget *w = new RotateWidget();
    // DrawWidget *w = new DrawWidget();
    LineWidget *w = new LineWidget();
    // TestWidget *w = new TestWidget();
    // FractalWidget *w = new FractalWidget();
    w->show();
#endif

#if 0
    emit info("Info");
    emit debug("Debug");
    emit error("Error");
    emit trace("Trace");
    emit colorLog("yellow on blue", Qt::yellow, Qt::blue);
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test2(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("Test2");

#if 1
    stopThread();
#endif

#if 0
    emit info(QString("value: %1")
              .arg(Connection::get_double_value()));
#endif

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::test_style(void)
{
    emit trace(Q_FUNC_INFO);
    emit info("test_style");

    // QApplication::setStyle(new Custom_test_style("fusion"));
    // QApplication::setStyle(new Custom_MFC_style("fusion"));
    QApplication::setStyle(new Custom_Aqua_style("fusion"));

    // 2. СБРАСЫВАЕМ ГЛОБАЛЬНУЮ ПАЛИТРУ
    // Если в старой теме палитра приложения была изменена, её нужно вернуть к дефолту стиля Fusion,
    // чтобы ваш метод polish() в стиле заново накатил правильные цвета на чистый холст.
    QApplication::setPalette(QApplication::style()->standardPalette());

    // 3. ПРИНУДИТЕЛЬНОЕ ОБНОВЛЕНИЕ ВСЕХ ВИДЖЕТОВ
    // Проходим по абсолютно всем виджетам приложения и заставляем их сбросить старый графический кеш
    for (QWidget *widget : QApplication::allWidgets()) {

        // Оповещаем виджет, что стиль поменялся (это вызовет unpolish для старого стиля и polish для нового)
        QEvent changeEvent(QEvent::StyleChange);
        QApplication::sendEvent(widget, &changeEvent);

        // Сбрасываем внутренний кеш геометрии и размеров (чтобы применился ваш pixelMetric)
        widget->updateGeometry();

        // Ставим виджет в очередь на полную физическую перерисовку экрана
        widget->update();
    }

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::run_cube_widget(void)
{
    emit trace(Q_FUNC_INFO);

    // CubeWidget *cw = new CubeWidget();
    // cw->show();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::run_ogl_widget(void)
{
    emit trace(Q_FUNC_INFO);

    // ORL_widget *ow = new ORL_widget();
    // ow->show();

    return true;
}
//--------------------------------------------------------------------------------
bool MainBox::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void MainBox::load_setting(void)
{
#ifdef QT_DEBUG
    qDebug() << "MainBox::load_setting";
#endif

    //---
    int value;
    if(load_int("index", &value))
    {
        cb_test->setCurrentIndex(value);
    }
    //---

    if(sb_test)
    {
        int value = 0;
        if(load_int("sb_test", &value))
        {
            sb_test->setValue(value);
        }
    }
    if(cb_block)
    {
        int value = 0;
        load_int("cb_block", &value);
        bool block_is_checked = value;
        //cb_block->clicked(block_is_checked);
        cb_block->setChecked(block_is_checked);
    }
}
//--------------------------------------------------------------------------------
void MainBox::save_setting(void)
{
#ifdef QT_DEBUG
    qDebug() << "MainBox::save_setting";
#endif

    save_int("index", cb_test->currentIndex());

    if(sb_test)
    {
        save_int("sb_test", sb_test->value());
    }
    if(cb_block)
    {
        save_int("cb_block", cb_block->isChecked());
    }
}
//--------------------------------------------------------------------------------
