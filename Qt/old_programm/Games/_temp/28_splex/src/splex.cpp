//Разработано aureli0,NeonLight_Child,Ph0Eni}{,Zinoviev54,imanavhod-это все я:)
//http://swibplex.sourceforge.com
//последний билд от 23.12.2009

#ifdef __WIN32__
#include <windows.h>
#endif

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include <SDL_opengl.h>

using namespace std;//ХЗ что это но с ним заработал строковый тип string

const
int start_time=1;//через это время появится меню

int win_DX,//=1024,//размеры окна по X
win_DY,//=768;//размеры окна по Y
pal,//палитра
full_screen,//на полный экран или нет
delay,//задержка
game_speed,
flame_summ,//количество частиц огня
default_level_num,//номер уровня по умолчанию
music_volume,
sound_volume,

level_max,//максимальное число уровней

track_max,//максимальное число трековб 1-ый трек-музыка меню
track,//текущий трек
old_track,//старый трек
track_change,//переключить трек

show_fps,

tmp_win_DX,//тоже самое что и раньше, только для опций
tmp_win_DY,
tmp_pal,
tmp_full_screen,

done=0,//переменная отвечающая за окончание Великого рендера
pre_done=0,//перед выходом что сделать

title_show,//титульник
title_menu,//главное меню

game_init,//инициация для новой игры
game_show,//игра
game_menu,//меню в игре
game_return,//возвращение в игру
game_restart,//рестарт игры
game_exit,//выход из игры втитульник

edit_init,//аналогично в редакторе
edit_show,
edit_menu,
edit_return,
edit_exit,

option_init,//для опций
option_show,
option_return,
option_exit,

info_init,//для опций
info_show,
info_return,
info_exit,

level_num,//текущий номер уровня
diff_game_mode,//игровая сложность

i,j,k,l,m,n,o,p;//переменные циклов

//мышь
float mouse_X,//координаты мышки
mouse_Y;
bool mouse_LB,mouse_RB;//о нажиме на левую и правую кнопки мыши

SDL_Surface *screen,//Экран
*surface;//SDL поверхность
SDL_Event event;//SDL событие
Uint8* keys;//для управления клавиатурой

#include "graph.h"//модуль графики
#include "sm2.h"//звуковой модуль
#include "var.h"//описание почти всех игровых переменных
#include "level.h"//модуль уровня(статика)
#include "obj.h"//модуль объектов(динамика)
#include "other.h"//модуль всего остального
#include "effects.h"//модуль эффектов, взрыва, тьмы
#include "player.h"//модуль игрока 
#include "edit.h"//игровой редактор

void Title_Init(bool with_music)
{
    if (with_music==true)
    {
        Mix_VolumeMusic(music_volume);//громкость музыки(0-128)
        stopmusic();//останавливаем музыку
        track=1;
        music(track);
    }

    //Загрузка заднего фона  титульника
    load("data/img/bground/main_back.jpg",background_texture);
    //В начале игры видим только титульник и гланое меню
    title_show=1;
    title_menu=1;

    Reset_Button(1);//чтобы при последущем входе не было глюков с отжатием кнопки
    Reset_Button(2);
    Reset_Button(3);
    Reset_Button(4);
    Reset_Button(5);
    Reset_Button(6);
    Reset_Button(7);
}

void Init_GRAPH()//инициализация всего,что завязано на размерах экрана
{
    //Инициализация SDL(видео)
    if (SDL_Init(SDL_INIT_VIDEO)!=0)
    {
        printf("Unable to initialize SDL: %s\n",SDL_GetError());
        done=1;
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

    if (full_screen==0)
    {
        //screen=SDL_SetVideoMode(win_DX,win_DY,pal,SDL_OPENGL|SDL_RESIZABLE);
        screen=SDL_SetVideoMode(win_DX,win_DY,pal,0);
    }//экран OpenGl на полный экран
    if (full_screen==1)
    {
        screen=SDL_SetVideoMode(win_DX,win_DY,pal,SDL_OPENGL|SDL_FULLSCREEN|SDL_RESIZABLE);
    }//экран OpenGl в окне

    resizeWindow(win_DX,win_DY);

    if (!screen)
    {
        printf("Unable to set video mode: %s\n",SDL_GetError());
        done=1;
    }

    //Текст и иконка в титульнике окна программы
    SDL_WM_SetCaption("SwibPlex. Last build 23.12.2009","SwibPlex");
    SDL_WM_SetIcon(SDL_LoadBMP("data/img/other/icon.bmp"),NULL);

    //Устанавливаем режим 2D для OpenGL
    gl2dMode(win_DX,win_DY);

    Load_Game_Textures();//загрузка игровых текстур

    //Инициализация кнопок
    //кнопки основного меню
    InitButton(1,//номер кнопки
               "New Game",//имя кнопки
               "data/img/buttons/button1.jpg",//адрес картинки
               win_DX/2,//координаты
               0+4*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    InitButton(2,//номер кнопки
               "Level Editor",//имя кнопки
               "data/img/buttons/button2.jpg",//адрес картинки
               win_DX/2,//координаты
               0+5*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    InitButton(3,//номер кнопки
               "Option",//имя кнопки
               "data/img/buttons/button3.jpg",//адрес картинки
               win_DX/2,//координаты
               0+7*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    InitButton(4,//номер кнопки
               "About",//имя кнопки
               "data/img/buttons/button4.jpg",//адрес картинки
               win_DX/2,//координаты
               0+8*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    InitButton(5,//номер кнопки
               "Exit To System",//имя кнопки
               "data/img/buttons/button5.jpg",//адрес картинки
               win_DX/2,//координаты
               0+9*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    //стрелки перемотки текущего уровня
    InitButton(6,//номер кнопки
               "Level -",//имя кнопки
               "data/img/buttons/button6.jpg",//адрес картинки
               win_DX/2-0.25*win_DX,//координаты
               0+6*win_DY/12,
               0.9*win_DX/10.34,//800 размеры
               0.9*win_DY/12,//64
               true);

    InitButton(7,//номер кнопки
               "Level +",//имя кнопки
               "data/img/buttons/button7.jpg",//адрес картинки
               win_DX/2+0.25*win_DX,//координаты
               0+6*win_DY/12,
               0.9*win_DX/10.34,//размеры
               0.9*win_DY/12,//64
               true);

    InitRadioButton(10,//номер кнопки поля,
                    diff_game_mode,
                    "Dificult",
                    "data/img/buttons/button10.jpg",//текстура,
                    2,//количество сегментов выбора
                    2,
                    win_DX/2,//координаты
                    0+3*win_DY/12,
                    win_DX/1.46,//700 размеры
                    win_DY/12,//64
                    true);




    //новые кнопки в меню игры

    InitButton(11,//номер кнопки
               "Restart game",//имя кнопки
               "data/img/buttons/button11.jpg",//адрес картинки
               win_DX/2,//координаты
               0+6.5*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);


    InitButton(12,//номер кнопки
               "Return To Game",//имя кнопки
               "data/img/buttons/button12.jpg",//адрес картинки
               win_DX/2,//координаты
               0+3.2*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    InitButton(13,//номер кнопки
               "Return to Main Menu",//имя кнопки
               "data/img/buttons/button13.jpg",//адрес картинки
               win_DX/2,//координаты
               0+10.9*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    //новые кнопки в меню редактора - c 21 номера
    InitButton(21,//номер кнопки
               "Return To Editor",//имя кнопки
               "data/img/buttons/button12.jpg",//адрес картинки
               win_DX/2,//координаты
               0+4*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    InitButton(22,//номер кнопки
               "Return to Main Menu",//имя кнопки
               "data/img/buttons/button13.jpg",//адрес картинки
               win_DX/2,//координаты
               0+5*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    tmp_win_DX=win_DX;//записываем дополнительно для появления курсоров на нужных местах
    tmp_win_DY=win_DY;
    tmp_pal=pal;
    tmp_full_screen=full_screen;

    InitRadioButton(1,//номер кнопки поля,
                    tmp_win_DX,
                    "Win_DX&Win_DY",
                    "data/img/buttons/opt_but1.jpg",//текстура,
                    6,//количество сегментов выбора
                    2,
                    win_DX/2,//координаты
                    0+1*win_DY/12,
                    win_DX/1.46,//700 размеры
                    win_DY/12,//64
                    true);

    InitRadioButton(2,//номер кнопки поля,
                    tmp_pal,
                    "Pal",
                    "data/img/buttons/opt_but2.jpg",//текстура,
                    2,//количество сегментов выбора
                    2,
                    win_DX/2,//координаты
                    0+2.1*win_DY/12,
                    win_DX/1.46,//700 размеры
                    win_DY/12,//64
                    true);

    InitRadioButton(3,//номер кнопки поля,
                    tmp_full_screen,
                    "Full_Screen",
                    "data/img/buttons/opt_but3.jpg",//текстура,
                    2,//количество сегментов выбора
                    1,
                    win_DX/2,//координаты
                    0+3.2*win_DY/12,
                    win_DX/1.46,//700 размеры
                    win_DY/12,//64
                    true);

    InitRadioButton(4,//номер кнопки поля,
                    delay,
                    "delay",
                    "data/img/buttons/opt_but4.jpg",//текстура,
                    20,//количество сегментов выбора
                    2,
                    win_DX/2,//координаты
                    0+4.3*win_DY/12,
                    win_DX/1.46,//700 размеры
                    win_DY/12,//64
                    true);

    InitRadioButton(5,//номер кнопки поля,
                    game_speed,
                    "grid",
                    "data/img/buttons/opt_but5.jpg",//текстура,
                    6,//количество сегментов выбора
                    2,
                    win_DX/2,//координаты
                    0+5.4*win_DY/12,
                    win_DX/1.46,//700 размеры
                    win_DY/12,//64
                    true);


    InitRadioButton(6,//номер кнопки поля,
                    grid,
                    "grid",
                    "data/img/buttons/opt_but6.jpg",//текстура,
                    8,//количество сегментов выбора
                    2,
                    win_DX/2,//координаты
                    0+6.5*win_DY/12,
                    win_DX/1.46,//700 размеры
                    win_DY/12,//64
                    true);

    InitRadioButton(7,//номер кнопки поля,
                    flame_summ,
                    "flame",
                    "data/img/buttons/opt_but7.jpg",//текстура,
                    10,//количество сегментов выбора
                    2,
                    win_DX/2,//координаты
                    0+7.6*win_DY/12,
                    win_DX/1.46,//700 размеры
                    win_DY/12,//64
                    true);

    InitRadioButton(8,//номер кнопки поля,
                    music_volume,
                    "music_volume",
                    "data/img/buttons/opt_but8.jpg",//текстура,
                    10,//количество сегментов выбора
                    2,
                    win_DX/2,//координаты
                    0+8.7*win_DY/12,
                    win_DX/1.46,//700 размеры
                    win_DY/12,//64
                    true);

    InitRadioButton(9,//номер кнопки поля,
                    sound_volume,
                    "sound_volume",
                    "data/img/buttons/opt_but9.jpg",//текстура,
                    10,//количество сегментов выбора
                    2,
                    win_DX/2,//координаты
                    0+9.8*win_DY/12,
                    win_DX/1.46,//700 размеры
                    win_DY/12,//64
                    true);

    //новые кнопки в меню опций - c 31 номера
    InitButton(31,//номер кнопки
               "Return to Main Menu",//имя кнопки
               "data/img/buttons/button13.jpg",//адрес картинки
               win_DX/2,//координаты
               0+10.9*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    //кнопки ИНФО
    InitButton(61,//номер кнопки
               "Return",//имя кнопки
               "data/img/buttons/button13.jpg",//адрес картинки
               win_DX/2,//координаты
               0+10.9*win_DY/12,
               win_DX/1.46,//700 размеры
               win_DY/12,//64
               true);

    Init_Flame();//инициация параметров эффекта пламени

    //Очищаем экран перед рисованием
    glClear(GL_COLOR_BUFFER_BIT);


}

//Главный BEGIN
#ifdef __WIN32__
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
#else
int main(int argc, char **argv)
{
#endif
    // {

    srand(time(NULL));//randomize;-включить счетчик случайных чисел <iostream.h>

    LoadConfFile();//грузим конфигурационный файл
    InitSound();//Инициализация аудио
    Load_SFX();//грузим музыку и звуки
    Mix_VolumeMusic(music_volume);//громкость музыки(0-128)
    Mix_Volume(-1,sound_volume);//-1  этот канал идет звук

    //Значения радио кнопок для ОПЦИЙ
    //разрешение экрана
    RButton[1].case_value[1]=640;
    RButton[1].case_value[2]=800;
    RButton[1].case_value[3]=1024;
    RButton[1].case_value[4]=1152;
    RButton[1].case_value[5]=1280;
    RButton[1].case_value[6]=1680;
    //палитра
    RButton[2].case_value[1]=16;
    RButton[2].case_value[2]=32;
    //полный экран
    RButton[3].case_value[1]=0;
    RButton[3].case_value[2]=1;
    //задержка
    RButton[4].case_value[1]=0;
    RButton[4].case_value[2]=5;
    RButton[4].case_value[3]=10;
    RButton[4].case_value[4]=15;
    RButton[4].case_value[5]=20;
    RButton[4].case_value[6]=25;
    RButton[4].case_value[7]=30;
    RButton[4].case_value[8]=35;
    RButton[4].case_value[9]=40;
    RButton[4].case_value[10]=45;
    RButton[4].case_value[11]=50;
    RButton[4].case_value[12]=55;
    RButton[4].case_value[13]=60;
    RButton[4].case_value[14]=65;
    RButton[4].case_value[15]=70;
    RButton[4].case_value[16]=75;
    RButton[4].case_value[17]=80;
    RButton[4].case_value[18]=85;
    RButton[4].case_value[19]=90;
    RButton[4].case_value[20]=95;
    //скорость игры
    RButton[5].case_value[1]=1;
    RButton[5].case_value[2]=2;
    RButton[5].case_value[3]=4;
    RButton[5].case_value[4]=6;
    RButton[5].case_value[5]=8;
    RButton[5].case_value[6]=10;
    //игровая сетка
    RButton[6].case_value[1]=16;
    RButton[6].case_value[2]=24;
    RButton[6].case_value[3]=32;
    RButton[6].case_value[4]=48;
    RButton[6].case_value[5]=64;
    RButton[6].case_value[6]=96;
    RButton[6].case_value[7]=128;
    RButton[6].case_value[8]=256;
    //количество частиц огня
    RButton[7].case_value[1]=0;
    RButton[7].case_value[2]=100;
    RButton[7].case_value[3]=200;
    RButton[7].case_value[4]=300;
    RButton[7].case_value[5]=400;
    RButton[7].case_value[6]=500;
    RButton[7].case_value[7]=600;
    RButton[7].case_value[8]=700;
    RButton[7].case_value[9]=800;
    RButton[7].case_value[10]=900;
    //громкость музыки
    RButton[8].case_value[1]=0;
    RButton[8].case_value[2]=4;
    RButton[8].case_value[3]=16;
    RButton[8].case_value[4]=32;
    RButton[8].case_value[5]=48;
    RButton[8].case_value[6]=64;
    RButton[8].case_value[7]=80;
    RButton[8].case_value[8]=96;
    RButton[8].case_value[9]=112;
    RButton[8].case_value[10]=128;
    //громкость звуков
    RButton[9].case_value[1]=0;
    RButton[9].case_value[2]=4;
    RButton[9].case_value[3]=16;
    RButton[9].case_value[4]=32;
    RButton[9].case_value[5]=48;
    RButton[9].case_value[6]=64;
    RButton[9].case_value[7]=80;
    RButton[9].case_value[8]=96;
    RButton[9].case_value[9]=112;
    RButton[9].case_value[10]=128;

    //игровая сложность
    diff_game_mode=1;
    RButton[10].case_value[1]=1;
    RButton[10].case_value[2]=2;

    Init_GRAPH();//загрузка графики зависимой от размера экрана и инициализация видео режима

    //Инициируем шрифты
    TTF_Init();
    //генерим текстовые текстуры
    glGenTextures(1, &t_txt);//не в цикле а то прогу можно убить
    //грузим шрифт
    TTF_LoadFont(grid);

    InitFps();//инициализация ФПС

    Title_Init(true);//инициализация главного меню

    load("data/img/other/cursor.png", cursor_texture);//Текстура курсора

    LoadLevelList();//загружаем таблицу имен уровней
    LoadBestTime();//загружаем таблицу лучшего времени

    glClearColor(0,0,0,0); //задний цвет фона черный (R,G,B,alpha)
    glEnable(GL_TEXTURE_2D); //надо для вывода текстуры
    //Включаем альфа канал
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    //=================
    //=[ОСНОВНОЙ ЦИКЛ]=
    //=================
    while(done == 0)
    {
        Mix_VolumeMusic(music_volume);//громкость музыки(0-128)
        Mix_Volume(-1, sound_volume);//-1  этот канал идет звук




        if (track_change==1 or
                /*game_show==1 and//пока пашет только при mp3 :(*/
                Mix_PlayingMusic()==false)//если музыка не играет во время игры, то считаем что закончился трек
        {
            //выбираем случайно какой музыкальный трек играть
            old_track=track;
            track=rand()%track_max+1;
            if (track==old_track) {track=track+1;}//чтобы трек не повторялся подряд- берем следущий
            if (track>track_max) {track==track_max;}//число треков не может быть больше максимального значения

            music(track);//играем выбранный трек

            if (track_change==1) {track_change=0;}

        }

        //Подсчет ФПС
        SolveFps();

        //Вводится задержка для соблюдения определенного ФПС
        SDL_Delay(delay);

        //========
        //=[ИГРА]=-инициация и рефреш
        //========
        if (game_init==1)//инициация игры
        {
            Mix_VolumeMusic(music_volume);//громкость музыки(0-128)

            stopmusic();//останавливаем музыку
            //выбираем случайно какой музыкальный трек играть
            old_track=track;
            track=rand()%track_max+1;
            if (track==old_track) {track=track+1;}//чтобы трек не повторялся подряд,следущий

            music(track);//играем выбранный трек

            title_show=0;//отключаем титульник
            title_menu=0;//отключаем главное меню

            load("data/img/bground/game_back.jpg", background_texture);//Загрузка заднего игрового фона

            Init_Game();
            level_num=default_level_num;//начинаем с первого уровня
            Load_Level(level_num);

            Init_Lev_Doors();//инициализация состояния дверей

            Init_Lev_Lights();//инициализация состояния ламп
            Dark_Init();

            Player_Start();

            game_init=0;//инициация игры завершена
            game_show=1;//включаем игру
        }

        //Рестарт игры
        //========
        //=[ИГРА]=-инициация и рефреш
        //========
        if (game_show==1 and
                game_restart==1 and
                game_menu==1)
        {
            Init_Game();
            Load_Level(level_num);
            Init_Lev_Doors();//инициализация состояния дверей
            Init_Lev_Lights();//инициализация состояния ламп
            Dark_Init();
            Player_Start();
            game_restart=0;//инициация игры завершена
            game_menu=0;
        }

        if (game_show==1 and
                game_menu==0) //во время игры-рефреш
        {
            Matrix_ReFresh();//обновляем матрицы
        }

        if (game_exit==1 and //если выходим из игры
                game_menu==1)
        {
            Reset_Button(11);//чтобы при последущем входе не было глюков с отжатием кнопки
            Reset_Button(12);//чтобы при последущем входе не было глюков с отжатием кнопки
            Reset_Button(13);//чтобы при последущем входе не было глюков с отжатием кнопки


            game_show=0;
            game_menu=0;
            game_exit=0;

            Title_Init(true);
        }
        //========
        //========

        //============
        //=[РЕДАКТОР]=-инициация и рефреш
        //============
        if (edit_init==1)//инициация игры
        {
            title_show=0;//отключаем титульник
            title_menu=0;//отключаем главное меню

            load("data/img/bground/game_back.jpg", background_texture);//Загрузка заднего игрового фона

            Edit_Init();
            level_num=default_level_num;//начинаем с первого уровня
            edit_init=0;//инициация редактора завершена
            edit_show=1;//включаем редактор
        }

        if (edit_show==1 and
                edit_menu==0) //во время игры-рефреш
        {
            Matrix_ReFresh();//обновляем матрицы
        }

        if (edit_exit==1 and //если выходим из редактора
                edit_menu==1)
        {
            Reset_Button(21);//чтобы при последущем входе не было глюков с отжатием кнопки
            Reset_Button(22);//чтобы при последущем входе не было глюков с отжатием кнопки

            edit_show=0;
            edit_menu=0;
            edit_exit=0;

            Title_Init(false);
        }
        //========
        //========

        //========
        //=[ОПЦИИ]=-инициация
        //========
        if (option_init==1)//инициация игры
        {
            title_show=0;//отключаем титульник
            title_menu=0;//отключаем главное меню

            load("data/img/bground/game_back.jpg", background_texture);//Загрузка заднего фона опций

            option_init=0;//инициация опций завершена
            option_show=1;//включаем опции
        }

        if (option_exit==1)//если выходим из опций
        {
            if (tmp_win_DX!=win_DX or //если менялось разрешение экрана
                    tmp_pal!=pal or //палитра
                    tmp_full_screen!=full_screen)//режим отображения экрана
            {

                win_DX=tmp_win_DX;//меняем окно экрана
                win_DY=tmp_win_DY;
                pal=tmp_pal;
                full_screen=tmp_full_screen;

                TTF_LoadFont(grid);

                Init_GRAPH();//перезагружаем графику
            }

            Reset_Button(31);//чтобы при последущем входе не было глюков с отжатием кнопки
            option_show=0;
            option_exit=0;

            Title_Init(false);

        }
        //========
        //========

        //========
        //=[ИНФО]=-инициация
        //========
        if (info_init==1)//инициация игры
        {
            title_show=0;//отключаем титульник
            title_menu=0;//отключаем главное меню

            load("data/img/bground/info_back.jpg", background_texture);//Загрузка заднего фона опций

            info_init=0;//инициация опций завершена
            info_show=1;//включаем опции
        }

        if (info_exit==1)//если выходим из опций
        {
            Reset_Button(61);//чтобы при последущем входе не было глюков с отжатием кнопки
            info_show=0;
            info_exit=0;

            Title_Init(false);
        }
        //========
        //========

        //Обрабатываем событие с клавиатуры
        while(SDL_PollEvent(&event))
        {
            if (event.type==SDL_QUIT) {pre_done=1;}//на выход по закрытию программы нажав на кнопку на окне

            //Управление мышкой
            if(event.type == SDL_MOUSEMOTION)//получаем мышиные координаты
            {
                mouse_X = event.motion.x;
                mouse_Y = event.motion.y;
            }

            //Обработка сигналов с мыши
            if (event.type==SDL_MOUSEBUTTONDOWN and
                    event.button.state==SDL_PRESSED)//однократное нажатие на кнопку мыши
            {
                if(event.button.button == SDL_BUTTON_LEFT)//если была нажата левая кнопка
                {mouse_LB=true;}

                if(event.button.button == SDL_BUTTON_RIGHT)//если была нажата правая кнопка
                {mouse_RB=true;}
            }

            //Управление кнопками в главном меню титульника
            if (timer>start_time and //разглядеть портрет на заднем фоне
                    title_show==1 and
                    title_menu==1)
            {
                //начать игру
                KeyButton_int(1,event,mouse_X,mouse_Y,&game_init,1,1,0);
                //включить редактор
                KeyButton_int(2,event,mouse_X,mouse_Y,&edit_init,1,1,0);
                //включить опции
                KeyButton_int(3,event,mouse_X,mouse_Y,&option_init,1,1,0);
                //включить инфу
                KeyButton_int(4,event,mouse_X,mouse_Y,&info_init,1,1,0);
                //выход в систему
                KeyButton_int(5,event,mouse_X,mouse_Y,&pre_done,1,1,0);
                //перемотка уровня по умолчанию
                KeyButton_int(6,event,mouse_X,mouse_Y,&default_level_num,-1,1,level_max);
                KeyButton_int(7,event,mouse_X,mouse_Y,&default_level_num,+1,level_max,1);

                //игровая сложность
                KeyRadioButton(10,
                               &diff_game_mode,
                               event,
                               mouse_X,
                               mouse_Y);

            }

            //========
            //=[ИГРА]=-кнопки меню
            //========
            if (game_show==1 and //в меню во время игры
                    game_menu==1)
            {
                //рестарт игры
                KeyButton_int(11,event,mouse_X,mouse_Y,&game_restart,1,1,0);

                //вернуться в игру
                KeyButton_int(12,event,mouse_X,mouse_Y,&game_menu,-1,0,1);

                //вернуться в главное меню
                KeyButton_int(13,event,mouse_X,mouse_Y,&game_exit,1,1,0);

                KeyRadioButton(4,
                               &delay,
                               event,
                               mouse_X,
                               mouse_Y);

                KeyRadioButton(5,
                               &game_speed,
                               event,
                               mouse_X,
                               mouse_Y);

                KeyRadioButton(7,
                               &flame_summ,
                               event,
                               mouse_X,
                               mouse_Y);

                KeyRadioButton(8,
                               &music_volume,
                               event,
                               mouse_X,
                               mouse_Y);

                KeyRadioButton(9,
                               &sound_volume,
                               event,
                               mouse_X,
                               mouse_Y);
            }
            //========
            //========


            //============
            //=[РЕДАКТОР]=-кнопки меню
            //============
            if (edit_show==1 and //в меню во время игры
                    edit_menu==1)
            {
                //вернуться в игру
                KeyButton_int(21,event,mouse_X,mouse_Y,&edit_menu,-1,0,1);
                //вернуться в главное меню
                KeyButton_int(22,event,mouse_X,mouse_Y,&edit_exit,1,1,0);
            }
            //========
            //========

            //============
            //=[ОПЦИИ]=-кнопки меню
            //============
            if (option_show==1)
            {
                //вернуться в главное меню
                KeyButton_int(31,event,mouse_X,mouse_Y,&option_exit,1,1,0);

                KeyRadioButton(1,
                               &tmp_win_DX,
                               event,
                               mouse_X,
                               mouse_Y);
                if (tmp_win_DX==640) {tmp_win_DY=480;}//размеру окна по X соответствует
                if (tmp_win_DX==800) {tmp_win_DY=600;}
                if (tmp_win_DX==1024) {tmp_win_DY=768;}
                if (tmp_win_DX==1152) {tmp_win_DY=864;}
                if (tmp_win_DX==1280) {tmp_win_DY=960;}
                if (tmp_win_DX==1680) {tmp_win_DY=1050;}

                KeyRadioButton(2,
                               &tmp_pal,
                               event,
                               mouse_X,
                               mouse_Y);

                KeyRadioButton(3,
                               &tmp_full_screen,
                               event,
                               mouse_X,
                               mouse_Y);

                KeyRadioButton(4,
                               &delay,
                               event,
                               mouse_X,
                               mouse_Y);

                KeyRadioButton(5,
                               &game_speed,
                               event,
                               mouse_X,
                               mouse_Y);

                old_grid=grid;
                KeyRadioButton(6,
                               &grid,
                               event,
                               mouse_X,
                               mouse_Y);

                if (old_grid!=grid)
                {
                    fonts[5]=TTF_OpenFont("data/ttf/font2.ttf",grid);
                }//перерубаем текст который зависит от сетки

                KeyRadioButton(7,
                               &flame_summ,
                               event,
                               mouse_X,
                               mouse_Y);

                KeyRadioButton(8,
                               &music_volume,
                               event,
                               mouse_X,
                               mouse_Y);

                KeyRadioButton(9,
                               &sound_volume,
                               event,
                               mouse_X,
                               mouse_Y);
            }
            //========
            //========


            //============
            //=[ИНФО]=-кнопки меню
            //============
            if (info_show==1)
            {
                //вернуться в главное меню
                KeyButton_int(61,event,mouse_X,mouse_Y,&info_exit,1,1,0);
            }
            //========
            //========


            //Управление с клавиатуры
            if (event.type==SDL_KEYDOWN)//еcли нажата кнопка на клавиатуре-однократное нажатие
            {
                if (event.key.keysym.sym==SDLK_F9) {track_change=1;}//статичные lev

                //========
                //=[ИГРА]=-однократное нажатие
                //========
                if (game_show==1) //во время игры
                {
                    if (event.key.keysym.sym==SDLK_ESCAPE)//в игре можем включить меню по нажатию ESC
                    {
                        game_menu=not game_menu;
                        Reset_Button(11);//чтобы при последущем входе не было глюков с отжатием кнопки
                        Reset_Button(12);//чтобы при последущем входе не было глюков с отжатием кнопки
                        Reset_Button(13);//чтобы при последущем входе не было глюков с отжатием кнопки

                    }

                    if (game_menu==0)//когда меню отключено
                    {
                        Player_Controls_KeyClick();//управление колобком
                    }

                }
                //========
                //========

                //============
                //=[РЕДАКТОР]=-однократное нажатие
                //============
                if (edit_show==1) //во время редактирования
                {
                    if (event.key.keysym.sym==SDLK_ESCAPE)//в редакторе можем включить меню по нажатию ESC
                    {
                        edit_menu=not edit_menu;
                        Reset_Button(21);//чтобы при последущем входе не было глюков с отжатием кнопки
                        Reset_Button(22);//чтобы при последущем входе не было глюков с отжатием кнопки
                    }

                    if (edit_menu==0)//когда меню отключено
                    {
                        Edit_Controls();//клавиши управления редактором
                    }

                }
                //========
                //========

                //============
                //=[ОПЦИИ]=-однократное нажатие клавы
                //============
                if (option_show==1) //во время опций
                {
                    if (event.key.keysym.sym==SDLK_ESCAPE)//возврат в главное меню
                    {
                        Reset_Button(31);//чтобы при последущем входе не было глюков с отжатием кнопки
                        option_exit=1;
                    }
                }
                //========
                //========

                //============
                //=[ИНФО]=-однократное нажатие клавы
                //============
                if (info_show==1) //во время опций
                {
                    if (event.key.keysym.sym==SDLK_ESCAPE)//возврат в главное меню
                    {
                        Reset_Button(61);//чтобы при последущем входе не было глюков с отжатием кнопки
                        info_exit=1;
                    }
                }
                //========
                //========

            }
        }

        keys = SDL_GetKeyState(NULL);

        //========
        //=[ИГРА]=-постоянное нажатие и обработка
        //========
        if (game_show==1 and//во время игры вне меню
                game_menu==0)
        {
            Game_Timer();//игровой таймер

            Player_Render_Collision_Lev_Obj();//Обработка столконвений колобка c lev и obj
            Player_Controls_KeyDown();

            Player_Render_Move();//Обработка движений колобка
            Mirror();
            Obj_Render_Exit();//если вихрь заходит на спец выход то дохнет, а очки на выход уменьшаються
            Obj_Render_Teleport();//Взаимодействие объекта с телепортом
            Obj_Render_Stage1();//Проверка возможности перемещения объекта
            Obj_Render_Stage2();//Взаимодействие объектов типа obj=1,2 с колобком
            Obj_Render_Stage3();//Старт движения и псевдо АИ(поведение) объектов
            Obj_Render_Stage4();//Анимация и движение объектов

            Player_Render_Dead();//Повреждение колобка от бомбы и вихря
            Player_Eat_Obj();//Колобок ест бонусы (obj=1) и собирает при этом очки
            Obj_Render_Dead();//Смерть объектов

            Level_Control_Doors();//текстура lev для разного состояния дверей


        }
        //========
        //========


        //============
        //=[РЕДАКТОР]=-постоянное нажатие и обработка
        //============
        if (edit_show==1 and//во время редактирования вне меню
                edit_menu==0)
        {
            Cam_Controls_KeyDown();//управление камерой с клавиатуры
        }
        //========
        //========

        //Задний фон
        glColor4f(1,1,1,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,background_texture);
        DrawTXT(win_DX/2,win_DY/2,win_DX,win_DY,0,1);

        //========
        //=[ИГРА]=-вывод графики
        //========
        if (game_show==1)//во время игры

        {
            Flame();
            Level_Draw();//Рисуем уровень
            Portals_Draw();//Рисуем телепорты
            Obj_Draw();//Рисуем объекты
            Player_Draw();//Рисуем колобка
            Effect_Draw();//Рисуем эффекты

            Dark_Draw();//рисуем тьму

            HUD_Draw();//Рисуем ХУД
        }
        //========
        //========

        //============
        //=[РЕДАКТОР]=-вывод графики
        //============
        if (edit_show==1)//во время редактора

        {
            Level_Draw();//Рисуем уровень
            Obj_Draw();//Рисуем объекты
            Cursor_Draw();//Рисуем курсор редактора
            Other_Draw();//вывод прочего
        }
        //========
        //========

        //выводим номер музыкального трека и фпс
        if (show_fps==1)
        {
            sprintf(txt_tmp,"track=%i,fps=%i",track,fps);
            GlTxt(win_DX/5,//128,
                  win_DY/48,//16,
                  fonts[2],txtColorWhite,txt_tmp, t_txt);
            glBindTexture( GL_TEXTURE_2D, t_txt);
            DrawTXT(win_DX-win_DX/5-win_DX/30,
                    win_DY/76.8,//10
                    win_DX/5,//128,
                    win_DY/48,//16,
                    0,0);
        }

        //Вывод кнопок главного меню в титульнике
        if (timer>start_time and //разглядеть портрет на заднем фоне
                title_show==1 and
                title_menu==1)
        {
            //выводим номер текущего уровня
            sprintf(txt_tmp,"Level %i. Best time %is.",default_level_num,best_time[default_level_num]);//номер уровня
            GlTxt(win_DX/2.5,
                  win_DY/24,//32,
                  fonts[3],txtColorWhite,txt_tmp, t_txt);
            glBindTexture( GL_TEXTURE_2D, t_txt);
            DrawTXT(win_DX/2,
                    win_DY/2-win_DY/48,//-16,
                    win_DX/2.5,
                    win_DY/24,//32,
                    0,1);

            //выводим имя уровня из файла
            char *cstring;
            cstring=(char *)level_name[default_level_num].c_str();
            GlTxt(win_DX/2.5,//win_DX/3.94,//260,
                  win_DY/24,//32,
                  fonts[3],txtColorWhite,cstring, t_txt);
            glBindTexture( GL_TEXTURE_2D, t_txt);
            DrawTXT(win_DX/2,
                    win_DY/2+win_DY/48,//+16,
                    win_DX/2.5,//win_DX/3.94,//260,
                    win_DY/24,//32,
                    0,1);

            //Рисуем кнопки
            DrawButton(1,1,1,1,0.6);LightButton(1,cursor_texture,1,1,1,1);
            DrawButton(2,1,1,1,0.6);LightButton(2,cursor_texture,1,1,1,1);
            DrawButton(3,1,1,1,0.6);LightButton(3,cursor_texture,1,1,1,1);
            DrawButton(4,1,1,1,0.6);LightButton(4,cursor_texture,1,1,1,1);
            DrawButton(5,1,1,1,0.6);LightButton(5,cursor_texture,1,1,1,1);
            DrawButton(6,1,1,1,0.6);LightButton(6,cursor_texture,1,1,1,1);
            DrawButton(7,1,1,1,0.6);LightButton(7,cursor_texture,1,1,1,1);
            DrawRadioButton(10,cursor_texture,1,1,1,0.6);LightRadioButton(10,cursor_texture,1,1,1,1);
        }

        //========
        //=[ИГРА]=-вывод меню
        //========
        //Вывод кнопок меню в игре
        if (game_show==1 and
                game_menu==1)
        {

            //выводим номер текущего уровня
            GlTxt(win_DX/2.4,//win_DX/3.94,//260,
                  win_DY/12, //64
                  fonts[4],txtColorWhite,"-=SwibPlex=-", t_txt);
            glBindTexture( GL_TEXTURE_2D, t_txt);
            DrawTXT(win_DX/2,
                    0+1*win_DY/12,
                    win_DX/2.4,//win_DX/3.94,//260,
                    win_DY/12, //64
                    0,1);

            //выводим номер текущего уровня
            sprintf(txt_tmp,"Level %i. Best time %is.",default_level_num,best_time[default_level_num]);//номер уровня
            GlTxt(win_DX/2.5,
                  win_DY/24,//32,
                  fonts[3],txtColorWhite,txt_tmp, t_txt);
            glBindTexture( GL_TEXTURE_2D, t_txt);
            //    DrawTXT(win_DX/2,
            //            0+6.5*win_DY/12-win_DY/48,
            //           win_DX/2.5,
            //            win_DY/24,//32,
            //            0,1);
            DrawTXT(win_DX/2,
                    0+1.8*win_DY/12,
                    win_DX/2.4,//win_DX/3.94,//260,
                    win_DY/24, //64
                    0,1);

            //выводим имя уровня из файла
            char *cstring;
            cstring=(char *)level_name[default_level_num].c_str();
            GlTxt(win_DX/2.5,//win_DX/3.94,//260,
                  win_DY/24,//32,
                  fonts[3],txtColorWhite,cstring, t_txt);
            glBindTexture( GL_TEXTURE_2D, t_txt);
            DrawTXT(win_DX/2,
                    0+1.8*win_DY/12+win_DY/24,
                    win_DX/2.4,//win_DX/3.94,//260,
                    win_DY/24, //64
                    0,1);

            DrawButton(11,1,1,1,0.6);LightButton(11,cursor_texture,1,1,1,1);
            DrawButton(12,1,1,1,0.6);LightButton(12,cursor_texture,1,1,1,1);
            DrawButton(13,1,1,1,0.6);LightButton(13,cursor_texture,1,1,1,1);

            DrawRadioButton(4,cursor_texture,1,1,1,0.6);LightRadioButton(4,cursor_texture,1,1,1,1);
            DrawRadioButton(5,cursor_texture,1,1,1,0.6);LightRadioButton(5,cursor_texture,1,1,1,1);

            DrawRadioButton(7,cursor_texture,1,1,1,0.6);LightRadioButton(7,cursor_texture,1,1,1,1);
            DrawRadioButton(8,cursor_texture,1,1,1,0.6);LightRadioButton(8,cursor_texture,1,1,1,1);
            DrawRadioButton(9,cursor_texture,1,1,1,0.6);LightRadioButton(9,cursor_texture,1,1,1,1);
        }
        //========
        //========

        //============
        //=[РЕДАКТОР]=-вывод меню
        //============
        //Вывод кнопок меню в игре
        if (edit_show==1 and
                edit_menu==1)
        {
            //Рисуем кнопки
            DrawButton(21,1,1,1,0.6);LightButton(21,cursor_texture,1,1,1,1);
            DrawButton(22,1,1,1,0.6);LightButton(22,cursor_texture,1,1,1,1);
        }
        //========
        //========


        //============
        //=[ОПЦИИ]=-вывод меню
        //============
        //Вывод кнопок меню в опциях
        if (option_show==1)
        {
            Flame();
            //Рисуем кнопки
            DrawButton(31,1,1,1,0.6);LightButton(31,cursor_texture,1,1,1,1);
            DrawRadioButton(1,cursor_texture,1,1,1,0.6);LightRadioButton(1,cursor_texture,1,1,1,1);
            DrawRadioButton(2,cursor_texture,1,1,1,0.6);LightRadioButton(2,cursor_texture,1,1,1,1);
            DrawRadioButton(3,cursor_texture,1,1,1,0.6);LightRadioButton(3,cursor_texture,1,1,1,1);
            DrawRadioButton(4,cursor_texture,1,1,1,0.6);LightRadioButton(4,cursor_texture,1,1,1,1);
            DrawRadioButton(5,cursor_texture,1,1,1,0.6);LightRadioButton(5,cursor_texture,1,1,1,1);
            DrawRadioButton(6,cursor_texture,1,1,1,0.6);LightRadioButton(6,cursor_texture,1,1,1,1);
            DrawRadioButton(7,cursor_texture,1,1,1,0.6);LightRadioButton(7,cursor_texture,1,1,1,1);
            DrawRadioButton(8,cursor_texture,1,1,1,0.6);LightRadioButton(8,cursor_texture,1,1,1,1);
            DrawRadioButton(9,cursor_texture,1,1,1,0.6);LightRadioButton(9,cursor_texture,1,1,1,1);
        }
        //========
        //========

        //============
        //=[ИНФО]=-вывод кнопок
        //============
        //Вывод кнопок меню в опциях
        if (info_show==1)
        {
            DrawButton(61,1,1,1,0.6);LightButton(61,cursor_texture,1,1,1,1);
        }
        //========
        //========

        //ВЫХОД!
        if (pre_done==1)
        {
            SaveConfFile();//сохраняем конфиг
            done=1;//выходим
        }
        SDL_GL_SwapBuffers();

    }//конец великого РЕНДЕРА

    //отключить SDL
    SDL_Quit();
    return 0;
}//конец главного BEGIN

