//--------------------------------------------------------------------------------
#include <iostream>
#include <SDL.h>
#include <SDL/SDL_image.h>
#include "SDLWidget.h"

#include <QKeyEvent>

#ifdef Q_WS_X11
#   include <X11/Xlib.h>
#endif
//--------------------------------------------------------------------------------
#ifdef QT_DEBUG
#   include <QDebug>
#endif
//--------------------------------------------------------------------------------
#include <QTimer>
QSDLScreenWidget::QSDLScreenWidget(QWidget *parent) :
    MyWidget(parent)
{
    // Turn off double buffering for this widget. Double buffering
    // interferes with the ability for SDL to be properly displayed
    // on the QWidget.
    //
    setAttribute(Qt::WA_PaintOnScreen);

    setFocusPolicy(Qt::StrongFocus);

    player_move_up = false;
    player_move_down = false;
    player_move_left = false;
    player_move_right = false;

    //state = RIGHT;
    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    //timer->start(100);

    background = IMG_Load(":/image/game_back.jpg");

    image_1 = IMG_Load(":/image/1.png");
    image_2 = IMG_Load(":/image/2.png");
    image_3 = IMG_Load(":/image/3.png");
    image_4 = IMG_Load(":/image/4.png");
    image_5 = IMG_Load(":/image/5.png");
    image_6 = IMG_Load(":/image/6.png");
    image_7 = IMG_Load(":/image/7.png");
    image_8 = IMG_Load(":/image/8.png");

    current_image = image_1;

    player_x = 0;
    player_y = 0;
}
//--------------------------------------------------------------------------------
void QSDLScreenWidget::resizeEvent(QResizeEvent *)
{
    // We could get a resize event at any time, so clean previous mode.
    // You do this because if you don't you wind up with two windows
    // on the desktop: the Qt application and the SDL window. This keeps
    // the SDL region synchronized inside the Qt widget and the subsequent
    // application.
    //
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    // Set the new video mode with the new window size
    //
    char variable[64];
    //sprintf(variable, "SDL_WINDOWID=0x%lx", winId());
    sprintf(variable, "SDL_WINDOWID=0x%l", winId());
    putenv(variable);

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
        return;
    }

    screen = SDL_SetVideoMode(width(), height(), 0, 0);

    if (!screen)
    {
        std::cerr << "Unable to set video mode: " << SDL_GetError() << std::endl;
        return;
    }
}
//--------------------------------------------------------------------------------
void QSDLScreenWidget::draw_image(SDL_Surface *image, int pos_x, int pos_y)
{
    if(!screen) return;
    if(!image)  return;

    SDL_Rect dst;
    dst.x = static_cast<Sint16>(pos_x);
    dst.y = static_cast<Sint16>(pos_y);
    dst.w = static_cast<Uint16>(image->w);
    dst.h = static_cast<Uint16>(image->h);
    SDL_BlitSurface(image, nullptr, screen, &dst);
}
//--------------------------------------------------------------------------------
void QSDLScreenWidget::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Left:
        player_move_left = false;
        break;

    case Qt::Key_Right:
        player_move_right = false;
        break;

    case Qt::Key_Up:
        player_move_up = false;
        break;

    case Qt::Key_Down:
        player_move_down = false;
        break;
    }
    test();
}
//--------------------------------------------------------------------------------
void QSDLScreenWidget::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
    case Qt::Key_Left:
        player_move_left = true;
        break;

    case Qt::Key_Right:
        player_move_right = true;
        break;

    case Qt::Key_Up:
        player_move_up = true;
        break;

    case Qt::Key_Down:
        player_move_down = true;
        break;
    }
    test();
}
//--------------------------------------------------------------------------------
void QSDLScreenWidget::update(void)
{
    switch(state)
    {
    case UP:
        player_y-=10;
        if(player_y < 0)
        {
            state = RIGHT;
            current_image = image_1;
        }
        break;

    case DOWN:
        player_y+=10;
        if(player_y > 500)
        {
            state = LEFT;
            current_image = image_2;
        }
        break;

    case LEFT:
        player_x-=10;
        if(player_x < 0)
        {
            state = UP;
            current_image = image_3;
        }
        break;

    case RIGHT:
        player_x+=10;
        if(player_x > 500)
        {
            state = DOWN;
            current_image = image_4;
        }
        break;
    }

    repaint();
}
//--------------------------------------------------------------------------------
void QSDLScreenWidget::test(void)
{
    if (screen)
    {
        //SDL_FillRect(screen, NULL, 0);

        SDL_BlitSurface(background, nullptr, screen, nullptr);
#if 1
        if(player_move_up)    player_y-=10;
        if(player_move_down)  player_y+=10;
        if(player_move_left)  player_x-=10;
        if(player_move_right) player_x+=10;

        draw_image(current_image, player_x, player_y);
#else
        int pos_x = 0;
        draw_image(image_1, pos_x, 0);  pos_x += image_1->w;
        draw_image(image_2, pos_x, 0);  pos_x += image_2->w;
        draw_image(image_3, pos_x, 0);  pos_x += image_3->w;
        draw_image(image_4, pos_x, 0);  pos_x += image_4->w;
        draw_image(image_5, pos_x, 0);  pos_x += image_5->w;
        draw_image(image_6, pos_x, 0);  pos_x += image_6->w;
        draw_image(image_7, pos_x, 0);  pos_x += image_7->w;
        draw_image(image_8, pos_x, 0);  pos_x += image_8->w;
#endif
        SDL_Flip(screen);
        SDL_GetKeyState(nullptr);
    }
}
//--------------------------------------------------------------------------------
void QSDLScreenWidget::paintEvent(QPaintEvent *)
{
#ifdef Q_WS_X11
    // Make sure we're not conflicting with drawing from the Qt library
    //
    XSync(QX11Info::display(), FALSE);
#endif

    if (screen)
    {
#ifdef QT_DEBUG
        qDebug() << "paintEvent";
#endif
        SDL_FillRect(screen, nullptr, 0);

        SDL_BlitSurface(background, nullptr, screen, nullptr);
#if 1
        if(player_move_up)    player_y-=10;
        if(player_move_down)  player_y+=10;
        if(player_move_left)  player_x-=10;
        if(player_move_right) player_x+=10;

        draw_image(current_image, player_x, player_y);
#else
        int pos_x = 0;
        draw_image(image_1, pos_x, 0);  pos_x += image_1->w;
        draw_image(image_2, pos_x, 0);  pos_x += image_2->w;
        draw_image(image_3, pos_x, 0);  pos_x += image_3->w;
        draw_image(image_4, pos_x, 0);  pos_x += image_4->w;
        draw_image(image_5, pos_x, 0);  pos_x += image_5->w;
        draw_image(image_6, pos_x, 0);  pos_x += image_6->w;
        draw_image(image_7, pos_x, 0);  pos_x += image_7->w;
        draw_image(image_8, pos_x, 0);  pos_x += image_8->w;
#endif
        SDL_Flip(screen);
    }
}
//--------------------------------------------------------------------------------
void QSDLScreenWidget::updateText(void)
{

}
//--------------------------------------------------------------------------------
bool QSDLScreenWidget::programm_is_exit(void)
{
    return true;
}
//--------------------------------------------------------------------------------
void QSDLScreenWidget::load_setting(void)
{

}
//--------------------------------------------------------------------------------
void QSDLScreenWidget::save_setting(void)
{

}
//--------------------------------------------------------------------------------
