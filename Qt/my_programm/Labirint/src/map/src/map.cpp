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
#include <QtWidgets>
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
    grid_map = new QGridLayout(this);
    Q_CHECK_PTR(grid_map);

    grid_map->setMargin(0);
    grid_map->setSpacing(0);

#if 1
    for(int y=0; y<25; y++)
    {
        for(int x=0; x<25; x++)
        {
            add_item(x, y, SPACE_ID);
        }
    }
#endif

    createTimer();

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
#if 1
    QLayoutItem *child;
    while ((child = grid_map->takeAt(0)) != 0)
    {
        grid_map->removeItem(child);
        if (child->widget())
            delete child->widget();
        if (child->layout())
            delete child->layout();
        delete child;
    }
#endif
    //---
    //FIXME
    //grid_map->setColumnCount(max_x);
    //grid_map->setRowCount(max_y);

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
    }
    for(int x=0; x<max_x; x++)
    {
        add_item(x, max_y-1, WALL_ID);
    }
    for(int y=1; y<(max_y-1); y++)
    {
        add_item(0, y, WALL_ID);
    }
    for(int y=1; y<(max_y-1); y++)
    {
        add_item(max_x-1, y, WALL_ID);
    }
    //TODO
#if 0
    add_item(0,         0,          START_ID);
    add_item(max_x-1,   0,          START_ID);
    add_item(0,         max_y-1,    START_ID);
    add_item(max_x-1,   max_y-1,    START_ID);
#endif
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

    init_id_map();

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

    new_map(w, h);

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
            add_item(x, y, ba.at(x));
        }

        y++;
        ok = xmlGet->findNext("items");
    }
    return true;
}
//--------------------------------------------------------------------------------
uint8_t Map::get_id(int x, int y)
{
    return id_map[x][y];
}
//--------------------------------------------------------------------------------
bool Map::add_item(int x, int y, int id)
{
    if(x < 0)   return false;
    if(y < 0)   return false;
    if(x >= MAX_WIDTH)   return false;
    if(y >= MAX_HEIGHT)  return false;

    QPixmap pixmap;
    QLabel *label = 0;
    bool ok = false;

    switch (id)
    {
    case PLAYER_ID:
    case WALL_ID:
    case SPACE_ID:
    case START_ID:
    case EXIT_ID:
        pixmap.load(QString(":/images/%1.png").arg(id));

        label = new QLabel();
        label->setProperty(PROPERTY_ID, id);
        label->installEventFilter(this);
        label->setPixmap(pixmap);

        id_map[x][y] = id;
        grid_map->addWidget(label, y, x);
        ok = true;
        break;

    default:
        emit error(QString("item %1 is not valid").arg(id));
        break;
    }
    return ok;
}
//--------------------------------------------------------------------------------
void Map::put_picture(int id, int x, int y)
{
    add_item(x, y, id);
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

    xmlPut->putInt("width",  grid_map->columnCount());
    xmlPut->putInt("height", grid_map->rowCount());

    for(int y=0; y<grid_map->rowCount(); y++)
    {
        QByteArray ba;
        ba.clear();

        QString temp;
        temp.clear();
        for(int x=0; x<grid_map->columnCount(); x++)
        {
            QLayoutItem *item = grid_map->itemAtPosition(y, x);
            //TODO Q_CHECK_PTR(item);
            if(item)
            {
                QWidget *w = item->widget();
                Q_CHECK_PTR(w);
                if(w)
                {
                    bool ok = false;
                    int id = w->property(PROPERTY_ID).toInt(&ok);
                    if(ok == false)
                    {
                        emit error("OK is false");
                    }
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
                    ba.append((char)id);
                    temp.append(QString("%1 ").arg(id));
                }
            }
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
    for(int y=0; y<grid_map->rowCount(); y++)
    {
        for(int x=0; x<grid_map->columnCount(); x++)
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
    for(int y=0; y<grid_map->rowCount(); y++)
    {
        for(int x=0; x<grid_map->columnCount(); x++)
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
void Map::start(unsigned int interval_ms)
{
    bool ok = false;
    int x = 0;
    int y = 0;

    ok = find_start(&x, &y);
    if(!ok)
    {
        QMessageBox::critical(this, "Ошибка!", QString("Объект %1 не найден!").arg(START_ID));
        return;
    }
    ok = find_player(&x, &y);
    if(!ok)
    {
        QMessageBox::critical(this, "Ошибка!", QString("Объект %1 не найден!").arg(PLAYER_ID));
        return;
    }
    timer->start(interval_ms);
    unsetCursor();
}
//--------------------------------------------------------------------------------
void Map::update(void)
{
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
    return max_x;
    //return grid_map->rowCount();
}
//--------------------------------------------------------------------------------
int Map::columnCount(void)
{
    return max_y;
    //return grid_map->columnCount();
}
//--------------------------------------------------------------------------------
void Map::player_move_up(void)
{
    int id_victory = get_id(player_x, player_y-1);
    if(id_victory == EXIT_ID)
    {
        timer->stop();
        QString temp = QString("Цель достигнута! (%1)").arg(cnt_move);
        emit info(temp);
        QMessageBox::information(this, "Победа!", temp);
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
        QString temp = QString("Цель достигнута! (%1)").arg(cnt_move);
        emit info(temp);
        QMessageBox::information(this, "Победа!", temp);
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
        QString temp = QString("Цель достигнута! (%1)").arg(cnt_move);
        emit info(temp);
        QMessageBox::information(this, "Победа!", temp);
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
        QString temp = QString("Цель достигнута! (%1)").arg(cnt_move);
        emit info(temp);
        QMessageBox::information(this, "Победа!", temp);
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
            QLabel *label = (QLabel *)obj;
            if(label)
            {
                label->setPixmap(cursor().pixmap());
                label->setProperty(PROPERTY_ID, id);
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
void Map::updateText(void)
{

}
//--------------------------------------------------------------------------------
