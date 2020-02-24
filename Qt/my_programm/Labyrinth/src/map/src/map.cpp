/*********************************************************************************
**                                                                              **
**     Copyright (C) 2018                                                       **
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
#include "qxmlputget.h"
#include "defines.hpp"
#include "map.hpp"
//--------------------------------------------------------------------------------
enum {
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};
//--------------------------------------------------------------------------------
Map::Map(QWidget *parent) :
    MyWidget(parent)
{
    init();
}
//--------------------------------------------------------------------------------
Map::~Map()
{

}
//--------------------------------------------------------------------------------
void Map::init(void)
{
    createTimer();

    grid_map = new QGridLayout();
    grid_map->setMargin(0);
    grid_map->setSpacing(0);
    setLayout(grid_map);
    setSizePolicy(QSizePolicy::Fixed,   QSizePolicy::Fixed);
}
//--------------------------------------------------------------------------------
void Map::createTimer(void)
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}
//--------------------------------------------------------------------------------
void Map::init_id_map(void)
{
    for(int y=0; y<MAX_HEIGHT; y++)
    {
        for(int x=0; x<MAX_WIDTH; x++)
        {
            id_map[x][y] = -1;
        }
    }
}
//--------------------------------------------------------------------------------
void Map::new_map(int max_x, int max_y)
{
    if(max_x < MIN_WIDTH) max_x = MIN_WIDTH;
    if(max_x > MAX_WIDTH) max_x = MAX_WIDTH;
    if(max_y < MIN_HEIGHT) max_y = MIN_HEIGHT;
    if(max_y > MAX_HEIGHT) max_y = MAX_HEIGHT;

    this->max_x = max_x;
    this->max_y = max_y;

    //---
    QLayoutItem *child;
    while ((child = grid_map->takeAt(0)) != nullptr)
    {
        grid_map->removeItem(child);
        if (child->widget())
            delete child->widget();
        if (child->layout())
            delete child->layout();
        delete child;
    }

    // заполнение карты
    for(int y=1; y<(max_y-1); y++)
    {
        for(int x=1; x<(max_x-1); x++)
        {
            add_item(x, y, SPACE_ID);
        }
    }
    //заполнение бордюра карты
    QPixmap p_wall;
    p_wall.load(":/images/13.png");

    for(int x=0; x<max_x; x++)
    {
        add_item(x, 0, WALL_ID);
        add_item(x, max_y-1, WALL_ID);
    }
    for(int y=1; y<(max_y-1); y++)
    {
        add_item(0, y, WALL_ID);
        add_item(max_x-1, y, WALL_ID);
    }
}
//--------------------------------------------------------------------------------
bool Map::load_map(const QString &filename)
{
    if(filename.isEmpty())
    {
        emit error("load_map: filename is empty!");
        return false;
    }

    emit info(QString("load_map(%1)").arg(filename));

    direction_move   = RIGHT;
    player_x = 1;
    player_y = 1;

    QXmlGet *xmlGet = new QXmlGet();
    bool ok = xmlGet->load(filename);
    if(!ok)
    {
        emit error(QString("file %1 not load!").arg(filename));
        return false;
    }

    ok = xmlGet->find("width");
    if(!ok)
    {
        emit error("find width not found!");
        return false;
    }

    int w = xmlGet->getInt(-1);
    if(w<0)
    {
        emit error("width < 0");
        return false;
    }

    ok = xmlGet->find("height");
    if(!ok)
    {
        emit error("find height not found");
        return false;
    }

    int h = xmlGet->getInt(-1);
    if(h<0)
    {
        emit error("height < 0");
        return false;
    }

    emit info(QString("width  %1").arg(w));
    emit info(QString("height %1").arg(h));

    max_x = w;
    max_y = h;

    int y = 0;
    ok = xmlGet->find("items");
    while(ok)
    {
        QString temp = xmlGet->getString("");
        emit debug(QString("find %1").arg(temp));
        QByteArray ba;

        QByteArray ta;
        ta.clear();
        ta.append(temp);
        emit trace(ta.toHex());

        ba = QByteArray::fromHex(ta);
        emit trace(ba.toHex());
        for(int x=0; x<ba.length(); x++)
        {
            if(ba.at(x) ==  PLAYER_ID)
            {
                player_x = x;
                player_y = y;

                begin_player_x = x;
                begin_player_y = y;
            }
            add_item(x, y, ba.at(x));
        }

        y++;
        ok = xmlGet->findNext("items");
    }
    return true;
}
//--------------------------------------------------------------------------------
int Map::get_id(int x, int y)
{
    return id_map[x][y];
}
//--------------------------------------------------------------------------------
bool Map::add_item(int x, int y, int id)
{
    if(x < 0)   return false;
    if(y < 0)   return false;
    if(x > MAX_WIDTH)   return false;
    if(y > MAX_HEIGHT)  return false;

    QPixmap pixmap;
    QLabel *label;
    bool ok = false;

    ok = pixmap.load(QString(":/images/%1.png").arg(id));
    if(ok)
    {
        label = new QLabel();
        label->setProperty(PROPERTY_ID, id);
        label->setProperty(PROPERTY_X, x);
        label->setProperty(PROPERTY_Y, y);
        label->installEventFilter(this);
        label->setPixmap(pixmap);

        id_map[x][y] = id;
        grid_map->addWidget(label, y, x);
    }

    return ok;
}
//--------------------------------------------------------------------------------
bool Map::put_picture(int id, int x, int y)
{
    emit debug(QString("put_picture(%1, %2, %3)")
               .arg(id)
               .arg(x)
               .arg(y));

    QLayoutItem *item = grid_map->itemAtPosition(y, x);
    Q_CHECK_PTR(item);

    QLabel *label = dynamic_cast<QLabel*>(item->widget());
    Q_CHECK_PTR(label);

    emit debug(QString("put_picture: PROPERTY_X %1 PROPERTY_Y %2 PROPERTY_ID %3")
               .arg(label->property(PROPERTY_X).toInt())
               .arg(label->property(PROPERTY_Y).toInt())
               .arg(label->property(PROPERTY_ID).toInt()));

    QString filename = QString(":/images/%1.png").arg(id);
    emit debug(filename);

    QPixmap pixmap;
    pixmap.load(filename);

    label->setPixmap(pixmap);
    label->setProperty(PROPERTY_ID, id);
    label->setProperty(PROPERTY_X, x);
    label->setProperty(PROPERTY_Y, y);
    add_item(x, y, id);
    return true;
}
//--------------------------------------------------------------------------------
bool Map::save_map(const QString &filename)
{
    if(filename.isEmpty())
    {
        emit error("save_map: filename is empty!");
        return false;
    }

    QXmlPut *xmlPut = new QXmlPut("Map");
    Q_CHECK_PTR(xmlPut);

    int w = columnCount();
    int h = rowCount();

    emit info(QString("save width  %1").arg(w));
    emit info(QString("save height %1").arg(h));

    xmlPut->putInt("width",  w);
    xmlPut->putInt("height", h);

    for(int y=0; y<h; y++)
    {
        QByteArray ba;
        ba.clear();

        QString temp;
        temp.clear();
        for(int x=0; x<w; x++)
        {
            int id = get_id(x, y);
            if(id == PLAYER_ID)
            {
                emit info("PLAYER FOUND");
            }
            if(id == START_ID)
            {
                emit info("START FOUND");
            }
            if(id == EXIT_ID)
            {
                emit info("EXIT FOUND");
            }
            ba.append(static_cast<char>(id));
            temp.append(QString("%1 ").arg(id));
        }
        xmlPut->putString("items", ba.toHex());
    }

    xmlPut->save(filename);
    return true;
}
//--------------------------------------------------------------------------------
bool Map::find_start(int *x, int *y)
{
    int cnt = 0;
    for(int y=0; y<rowCount(); y++)
    {
        for(int x=0; x<columnCount(); x++)
        {
            int id = get_id(x, y);
            if(id == START_ID)
            {
                start_x = x;
                start_y = y;
                cnt++;
            }
        }
    }
    if(cnt == 1)
    {
        *x = start_x;
        *y = start_y;
    }
    return (cnt == 1);
}
//--------------------------------------------------------------------------------
bool Map::find_player(int *x, int *y)
{
    int cnt = 0;
    for(int y=0; y<rowCount(); y++)
    {
        for(int x=0; x<columnCount(); x++)
        {
            int id = get_id(x, y);
            if(id == PLAYER_ID)
            {
                player_x = x;
                player_y = y;
                cnt++;
            }
        }
    }
    if(cnt == 1)
    {
        *x = player_x;
        *y = player_y;
    }
    return (cnt == 1);
}
//--------------------------------------------------------------------------------
QString Map::get_id_string(int id)
{
    QString temp;
    switch(id)
    {
    case PLAYER_ID: temp = QString("PLAYER (%1)").arg(id);  break;
    case WALL_ID:   temp = QString("WALL (%1)").arg(id);    break;
    case SPACE_ID:  temp = QString("SPACE (%1)").arg(id);   break;
    case START_ID:  temp = QString("START (%1)").arg(id);   break;
    case EXIT_ID:   temp = QString("EXIT (%1)").arg(id);    break;

    default:
        temp = QString("ID %1").arg(id);
        break;
    }
    return temp;
}
//--------------------------------------------------------------------------------
bool Map::start(int interval_ms)
{
    bool ok = false;
    int x = 0;
    int y = 0;

    ok = find_start(&x, &y);
    if(!ok)
    {
        QMessageBox::critical(this, "Ошибка!", QString("Объект %1 не найден!").arg(get_id_string(START_ID)));
        return false;
    }
    ok = find_player(&x, &y);
    if(!ok)
    {
        QMessageBox::critical(this, "Ошибка!", QString("Объект %1 не найден!").arg(get_id_string(PLAYER_ID)));
        return false;
    }
    timer->start(interval_ms);
    unsetCursor();

    return true;
}
//--------------------------------------------------------------------------------
void Map::update(void)
{
#if 0
    int x = rand() % 100;
    switch(x)
    {
    case 0: direction_move = UP;    break;
    case 1: direction_move = DOWN;  break;
    case 2: direction_move = LEFT;  break;
    case 3: direction_move = RIGHT; break;
    default:
        break;
    }
#endif

    switch(direction_move)
    {
    case UP:
        //emit info("move UP");
        player_move_up();
        break;

    case DOWN:
        //emit info("move DOWN");
        player_move_down();
        break;

    case LEFT:
        //emit info("move LEFT");
        player_move_left();
        break;

    case RIGHT:
        //emit info("move RIGHT");
        player_move_right();
        break;

    default:
        direction_move = RIGHT;
        break;
    }
}
//--------------------------------------------------------------------------------
void Map::stop(void)
{
    timer->stop();
    unsetCursor();
}
//--------------------------------------------------------------------------------
void Map::refresh(void)
{
    emit info("refresh");
}
//--------------------------------------------------------------------------------
int Map::rowCount(void)
{
    return max_y;
}
//--------------------------------------------------------------------------------
int Map::columnCount(void)
{
    return max_x;
}
//--------------------------------------------------------------------------------
void Map::check_victory(void)
{
    QString temp = QString("Цель достигнута! (%1)").arg(cnt_move);
    emit info(temp);
    QMessageBox::information(this, "Победа!", temp);
    emit victory();
}
//--------------------------------------------------------------------------------
void Map::player_move_up(void)
{
    int id_victory = get_id(player_x, player_y-1);
    if(id_victory == EXIT_ID)
    {
        timer->stop();
        check_victory();
        return;
    }

    int id_left = get_id(player_x-1, player_y);
    if(id_left == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_x--;
        put_picture(PLAYER_ID, player_x, player_y);
        direction_move = LEFT;
        cnt_move++;
        return;
    }

    int id_up = get_id(player_x, player_y-1);
    if(id_up == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_y--;
        put_picture(PLAYER_ID, player_x, player_y);
        cnt_move++;
    }
    else
    {
        direction_move = RIGHT;
    }
}
//--------------------------------------------------------------------------------
void Map::player_move_down(void)
{
    int id_victory = get_id(player_x, player_y+1);
    if(id_victory == EXIT_ID)
    {
        timer->stop();
        check_victory();
        return;
    }

    int id_right = get_id(player_x+1, player_y);
    if(id_right == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_x++;
        put_picture(PLAYER_ID, player_x, player_y);
        direction_move = RIGHT;
        cnt_move++;
        return;
    }

    int id_down = get_id(player_x, player_y+1);
    if(id_down == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_y++;
        put_picture(PLAYER_ID, player_x, player_y);
        cnt_move++;
    }
    else
    {
        direction_move = LEFT;
    }
}
//--------------------------------------------------------------------------------
void Map::player_move_left(void)
{
    int id_victory = get_id(player_x-1, player_y);
    if(id_victory == EXIT_ID)
    {
        timer->stop();
        check_victory();
        return;
    }

    int id_down = get_id(player_x, player_y+1);
    if(id_down == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_y++;
        put_picture(PLAYER_ID, player_x, player_y);
        direction_move = DOWN;
        cnt_move++;
        return;
    }

    int id_left = get_id(player_x-1, player_y);
    if(id_left == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_x--;
        put_picture(PLAYER_ID, player_x, player_y);
        cnt_move++;
    }
    else
    {
        direction_move = UP;
    }
}
//--------------------------------------------------------------------------------
void Map::player_move_right(void)
{
    int id_victory = get_id(player_x+1, player_y);
    if(id_victory == EXIT_ID)
    {
        timer->stop();
        check_victory();
        return;
    }

    int id_up = get_id(player_x, player_y-1);
    if(id_up == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_y--;
        put_picture(PLAYER_ID, player_x, player_y);
        direction_move = UP;
        cnt_move++;
        return;
    }

    int id_right = get_id(player_x+1, player_y);
    if(id_right == SPACE_ID)
    {
        put_picture(SPACE_ID, player_x, player_y);
        player_x++;
        put_picture(PLAYER_ID, player_x, player_y);
        cnt_move++;
    }
    else
    {
        direction_move = DOWN;
    }
}
//--------------------------------------------------------------------------------
bool Map::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress)
    {
        if(id > 0)
        {
            QLabel *label = static_cast<QLabel *>(obj);
            if(label)
            {
                label->setPixmap(cursor().pixmap());
                label->setProperty(PROPERTY_ID, id);

                int x = label->property(PROPERTY_X).toInt();
                int y = label->property(PROPERTY_Y).toInt();
                id_map[x][y] = id;
            }
        }
        return true;
    }
    return QObject::eventFilter(obj, event);
}
//--------------------------------------------------------------------------------
void Map::set_cursor(void)
{
    QToolButton *btn = dynamic_cast<QToolButton*>(sender());
    if(!btn)
    {
        emit error("set_cursor");
        return;
    }

    id = btn->property(PROPERTY_ID).toInt();
    setCursor(QCursor(btn->icon().pixmap(32, 32)));
}
//--------------------------------------------------------------------------------
QPixmap Map::rotate(const QString &filename, int angle)
{
    QPixmap shipPixels(filename);
    QPixmap rotatePixmap(shipPixels.size());
    rotatePixmap.fill(Qt::transparent);

    QPainter p(&rotatePixmap);
    p.setRenderHint(QPainter::Antialiasing); // сглаживание
    p.setRenderHint(QPainter::SmoothPixmapTransform);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.translate(rotatePixmap.size().width() / 2, rotatePixmap.size().height() / 2);
    p.rotate(angle); // градус
    p.translate(-rotatePixmap.size().width() / 2, -rotatePixmap.size().height() / 2);
    p.drawPixmap(0, 0, shipPixels);
    p.end();
    shipPixels = rotatePixmap;
    return shipPixels;
}
//--------------------------------------------------------------------------------
void Map::remove_player(void)
{
    for(int y=0; y<rowCount(); y++)
    {
        for(int x=0; x<columnCount(); x++)
        {
            int id = get_id(x, y);
            if(id == PLAYER_ID)
            {
                emit trace(QString("find player %1 %2")
                           .arg(x)
                           .arg(y));
                put_picture(SPACE_ID, x, y);
            }
        }
    }
}
//--------------------------------------------------------------------------------
bool Map::set_player(int pos_x, int pos_y)
{
    if(get_id(pos_x, pos_y) != SPACE_ID)
    {
        emit error(QString("set_player: id_map %1")
                   .arg(get_id(pos_x, pos_y)));
        return false;
    }

    player_x = pos_x;
    player_y = pos_y;

    id_map[player_x][player_y] = PLAYER_ID;
    put_picture(PLAYER_ID, player_x, player_y);

    return true;
}
//--------------------------------------------------------------------------------
bool Map::set_begin_player(void)
{
    emit debug(QString("begin_player_x %1").arg(begin_player_x));
    emit debug(QString("begin_player_y %1").arg(begin_player_y));

    player_x = begin_player_x;
    player_y = begin_player_y;

    id_map[player_x][player_y] = PLAYER_ID;
    put_picture(PLAYER_ID, player_x, player_y);

    return true;
}
//--------------------------------------------------------------------------------
void Map::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool Map::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void Map::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void Map::save_setting(void)
{

}
//--------------------------------------------------------------------------------
