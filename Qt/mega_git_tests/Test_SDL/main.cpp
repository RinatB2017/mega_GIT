//--------------------------------------------------------------------------------
#include <iostream>
#include <QWidget>
#include <SDL.h>
#include <SDL/SDL_image.h>
//--------------------------------------------------------------------------------
SDL_Surface *screen;

SDL_Surface *background;

SDL_Surface *player;
SDL_Surface *player_old;
int player_x = 32;
int player_y = 32;
int player_x_old = 0;
int player_y_old = 0;

int state_alien = 0;
SDL_Surface *current_alien;
SDL_Surface *alien_1;
SDL_Surface *alien_2;
SDL_Surface *alien_3;
SDL_Surface *alien_4;

SDL_Surface *wall;
SDL_Surface *field;

SDL_Event event;    //SDL событие
bool player_move_up = false;
bool player_move_down = false;
bool player_move_left = false;
bool player_move_right = false;

int cnt_draw_alien = 0;
//--------------------------------------------------------------------------------
void init(void)
{
    screen = 0;
    SDL_QuitSubSystem(SDL_INIT_VIDEO);

    // Set the new video mode with the new window size
    //
    //char variable[64];
    //sprintf(variable, "SDL_WINDOWID=0x%lx", QWidget::winId());
    //putenv(variable);

    if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Unable to init SDL: " << SDL_GetError() << std::endl;
        return;
    }

    screen = SDL_SetVideoMode(800, 800, 0, 0);

    if (!screen)
    {
        std::cerr << "Unable to set video mode: " << SDL_GetError() << std::endl;
        return;
    }
    
    background = IMG_Load("game_back.jpg");
    player = IMG_Load("31.png");
    player_old = IMG_Load("13.png");

    alien_1 = IMG_Load("7_1.png");
    alien_2 = IMG_Load("7_2.png");
    alien_3 = IMG_Load("7_3.png");
    alien_4 = IMG_Load("7_4.png");

    wall = IMG_Load("13.png");
    field = IMG_Load("5.png");
}
//--------------------------------------------------------------------------------
void draw_image(SDL_Surface *image, int pos_x, int pos_y)
{
    if(!screen) return;
    if(!image)  return;

    SDL_Rect dst;
    dst.x = pos_x;
    dst.y = pos_y;
    dst.w = image->w;
    dst.h = image->h;
    SDL_BlitSurface(image, NULL, screen, &dst);
}
//--------------------------------------------------------------------------------
void copy_image(SDL_Surface *image, int pos_x, int pos_y)
{
    if(!screen) return;
    if(!image)  return;

    SDL_Rect src;
    src.x = pos_x;
    src.y = pos_y;
    src.w = image->w;
    src.h = image->h;

//    SDL_Rect dst;
//    dst.x = 0;
//    dst.y = 0;
//    dst.w = image->w;
//    dst.h = image->h;
    SDL_BlitSurface(screen, &src, image, NULL);
}
//--------------------------------------------------------------------------------
#if 0
void copy_image(SDL_Surface *image,
                SDL_Surface *buffer,
                int pos_x, int pos_y)
{
    if(!screen) return;
    if(!image)  return;

    SDL_Rect dst;
    dst.x = pos_x;
    dst.y = pos_y;
    dst.w = image->w;
    dst.h = image->h;
    SDL_BlitSurface(image, NULL, buffer, &dst);
}
void draw_map(void)
{
    SDL_Surface *buffer = 0;
    buffer = SDL_CreateRGBSurface(SDL_HWSURFACE,25*32,25*32,32,0x00FF0000,0x0000FF00,0x000000FF,0x00000000);

    for(int n=0; n<25; n++)
    {
        copy_image(wall, buffer, n*wall->w, 0);
    }
    for(int y=1; y<24; y++)
    {
        copy_image(wall, buffer, 0, y*wall->h);
        for(int x=1; x<24; x++)
        {
            copy_image(field, buffer, x*wall->w, y*wall->h);
        }
        copy_image(wall, buffer, 24*wall->w, y*wall->h);
    }
    for(int n=0; n<25; n++)
    {
        copy_image(wall, buffer, n*wall->w, 24*wall->h);
    }
    draw_image(buffer, 0, 0);
}
#endif
//--------------------------------------------------------------------------------
bool draw(void)
{
    bool ok = false;
    if(player_move_up)    ok = true;
    if(player_move_down)  ok = true;
    if(player_move_left)  ok = true;
    if(player_move_right) ok = true;
    return ok;
}
//--------------------------------------------------------------------------------
bool keyboard(void)
{
    bool ok = false;
    while(SDL_PollEvent(&event))
    {
        if (event.type==SDL_KEYDOWN)
        {
            switch((int)event.key.keysym.sym)
            {
            case SDLK_LEFT:     player_move_left = true;    break;
            case SDLK_RIGHT:    player_move_right = true;   break;
            case SDLK_UP:       player_move_up = true;      break;
            case SDLK_DOWN:     player_move_down = true;    break;

            case SDLK_ESCAPE:
                ok = true;
                break;
            }
        }
        if (event.type==SDL_KEYUP)
        {
            switch((int)event.key.keysym.sym)
            {
            case SDLK_LEFT:     player_move_left = false;    break;
            case SDLK_RIGHT:    player_move_right = false;   break;
            case SDLK_UP:       player_move_up = false;      break;
            case SDLK_DOWN:     player_move_down = false;    break;

            case SDLK_ESCAPE:
                ok = true;
                break;
            }
        }
    }

    if(player_move_left)  player_x-=10;
    if(player_move_right) player_x+=10;
    if(player_move_up)    player_y-=10;
    if(player_move_down)  player_y+=10;

    if(player_x<0)     player_x=0;
    if(player_x>24*32) player_x=24*32;
    if(player_y<0)     player_y=0;
    if(player_y>24*32) player_y=24*32;

    return ok;
}
//--------------------------------------------------------------------------------
void draw_background(void)
{
    SDL_BlitSurface(background, NULL, screen, 0);
}
//--------------------------------------------------------------------------------
void clear_screen(void)
{
    SDL_FillRect(screen, NULL, 0);
}
//--------------------------------------------------------------------------------
void draw_player(void)
{
#ifdef SINGLE_APP
    if((player_x_old == player_x) && (player_y_old == player_y)) return;

    draw_image(player_old, player_x_old, player_y_old);
    copy_image(player_old, player_x, player_y);
    player_x_old = player_x;
    player_y_old = player_y;
#endif
    draw_image(player, player_x, player_y);
}
//--------------------------------------------------------------------------------
void draw_alien(void)
{
    cnt_draw_alien++;
    if(cnt_draw_alien>10)
    {
        cnt_draw_alien=0;
        switch(state_alien)
        {
        case 0: current_alien = alien_1; state_alien++; break;
        case 1: current_alien = alien_2; state_alien++; break;
        case 2: current_alien = alien_3; state_alien++; break;
        case 3: current_alien = alien_4; state_alien++; break;
        case 5: current_alien = alien_3; state_alien++; break;
        case 6: current_alien = alien_2; state_alien++; break;

        default:
            state_alien = 0;
            break;
        }
    }
    draw_image(current_alien, 200, 200);
}
//--------------------------------------------------------------------------------
void draw_map(void)
{
    for(int n=0; n<25; n++)
    {
        draw_image(wall, n*wall->w, 0);
    }
    for(int y=1; y<24; y++)
    {
        draw_image(wall, 0, y*wall->h);
        for(int x=1; x<24; x++)
        {
            draw_image(field, x*wall->w, y*wall->h);
        }
        draw_image(wall, 24*wall->w, y*wall->h);
    }
    for(int n=0; n<25; n++)
    {
        draw_image(wall, n*wall->w, 24*wall->h);
    }
}
//--------------------------------------------------------------------------------
void test(void)
{
    int buf[10][10] = { 0 };
    buf[5][5] = 5;
    for(int y=0; y<10; y++)
    {
        for(int x=0; x<10; x++)
        {
            std::cerr << buf[x][y];
        }
        std::cerr << "\n";
    }
}
//--------------------------------------------------------------------------------
int main(void)
{
    init();

    test();

    //draw_background();
    draw_map();
    draw_alien();
    draw_player();
    SDL_Flip(screen);

    bool is_exit = false;
    while(!is_exit)
    {
        is_exit = keyboard();

        //draw_background();
        //draw_map();
        draw_alien();
        draw_player();
        SDL_Flip(screen);
        //--

        SDL_GetKeyState(NULL);
    }

    //SDL_Delay(10000);
    return 1;
}
