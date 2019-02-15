//--------------------------------------------------------------------------------
#ifndef SDLWIDGET_H
#define SDLWIDGET_H
//--------------------------------------------------------------------------------
#include <Qt>
#include <QWidget>
//--------------------------------------------------------------------------------
struct SDL_Surface;

class QKeyEvent;
//--------------------------------------------------------------------------------
class QSDLScreenWidget : public QWidget
{
    Q_OBJECT
public:
    QSDLScreenWidget(QWidget *parent = nullptr);
    void draw_image(SDL_Surface *image, int pos_x, int pos_y);

private slots:
    void update(void);

private:
    SDL_Surface *screen;

    SDL_Surface *background;

    int player_x;
    int player_y;

    SDL_Surface *current_image;
    SDL_Surface *image_1;
    SDL_Surface *image_2;
    SDL_Surface *image_3;
    SDL_Surface *image_4;
    SDL_Surface *image_5;
    SDL_Surface *image_6;
    SDL_Surface *image_7;
    SDL_Surface *image_8;

    enum {
        UP = 0,
        DOWN,
        LEFT,
        RIGHT
    };
    bool player_move_up;
    bool player_move_down;
    bool player_move_left;
    bool player_move_right;
    int state;

    void test(void);

protected:
#if 1
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
#endif
    void resizeEvent(QResizeEvent *);
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif
