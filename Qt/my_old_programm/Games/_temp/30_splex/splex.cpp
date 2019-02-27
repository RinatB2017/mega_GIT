//http://swibplex.sourceforge.com

#ifdef __WIN32__
#include <windows.h>
#endif

#include "util.h"//мой модуль
#include "init.h"//описание почти всех игровых переменных
#include "other.h"//модуль всего остального
#include "game.h"
#include "effects.h"//модуль эффектов, взрыва, тьмы
#include "edit.h"//игровой редактор


void Reset_Buttons(int a, int b)
 {
  for(int j=a; j<=b; j++) 
   {
    Reset_Button(j);//Чтобы при последущем входе не было глюков с отжатием кнопки
   }	  
 }

void Title_Init(bool with_music)
 {
  if (with_music==true)//Если музыка в меню отличается от музыки в игре
   {
    Mix_VolumeMusic(music_volume);//громкость музыки(0-128)
    stopmusic();//останавливаем музыку
    track=1;
    music(track,-1);
   }

  //Чтобы при последущем входе не было глюков с отжатием кнопки  
  Reset_Buttons(index_button_level_down,index_rbutton_sound_vol);
  
  //Инициализация кнопок
  Init_Buttons();
  Init_ButtonsVar();
  
  //Инициация параметров эффекта пламени
  Init_Flame();
  
  LoadLevelList();//загружаем таблицу имен уровней
  LoadBestTime();//загружаем таблицу лучшего времени
 }

void Init_GRAPH()//инициализация всего,что завязано на размерах экрана
 {
  init_sdl_opengl(win_DX, win_DY, pal, full_screen);
  resize_window(win_DX,win_DY);
  
  //Текст и иконка в титульнике окна программы
  SDL_WM_SetCaption("SwibPlex. ver>1.0 20.07.2014","SwibPlex");
  SDL_WM_SetIcon(SDL_LoadBMP("data/img/other/icon.bmp"),NULL);

  //Устанавливаем режим 2D для OpenGL
  gl2dMode(win_DX,win_DY);
  //Очищаем экран перед рисованием
  glClear(GL_COLOR_BUFFER_BIT);
  
  glClearColor(0,0,0,0); //задний цвет фона черный (R,G,B,alpha)
  glEnable(GL_TEXTURE_2D); //надо для вывода текстуры

  //Включаем альфа канал
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);
  
  tmp_win_DX=win_DX;
  tmp_win_DY=win_DY;
  tmp_pal=pal;
  tmp_full_screen=full_screen;

 }

//Главный BEGIN
#ifdef __WIN32__
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
                   {
#else
int main(int argc, char **argv){
#endif
  cout << "Loading, please wait ... " << endl; 
  
  LoadConfFile();//грузим конфигурационный файл

  Randomize();//randomize;-включить счетчик случайных чисел <iostream.h>

  InitSound();//Инициализация аудио
  Load_SFX();//грузим музыку и звуки

  Mix_VolumeMusic(music_volume);//громкость музыки(0-128)
  Mix_Volume(-1,sound_volume);//-1  этот канал идет звук

  Init_GRAPH();//Инициализация видео режима

  Load_Game_Textures();//загрузка игровых текстур
  
  //Инициируем шрифты
  TTF_Init();
  //Генерим текстовые текстуры
  glGenTextures(1, &t_txt);//не в цикле а то прогу можно убить
  //Грузим шрифты
  TTF_LoadFont();
  
  InitFps();//инициализация ФПС


  Title_Init(true);//Инициализация главного меню

  //Игровая сложность
  diff_game_mode=2;
  
  //В начале игры видим только титульник и гланое меню
  title_show=1;
  title_menu=1;
  
//=================
//=[ОСНОВНОЙ ЦИКЛ]=
//=================
  while(done == 0)
   {
//Подсчет ФПС и таймеров
    SolveFps();
    
    //cout << "title_show=" << title_show << endl;
    //cout << "title_menu=" << title_menu << endl;

    //cout << "timer=" << timer << endl;

//Вводится задержка для соблюдения определенного ФПС
    SDL_Delay(delay);

    Mix_VolumeMusic(music_volume);//громкость музыки(0-128)
    Mix_Volume(-1, sound_volume);//-1  этот канал идет звук

    if (Mix_PlayingMusic()==false)//если музыка не играет во время игры, то считаем что закончился трек
     {
      //выбираем случайно какой музыкальный трек играть
      old_track=track;
      track=random(track_max);
      if (track==old_track) {track=track+1;}//чтобы трек не повторялся подряд- берем следущий
      if (track>track_max) {track==track_max;}//число треков не может быть больше максимального значения
      music(track,0);//играем выбранный трек
     }

    //========
    //=[ИГРА]=
    //========
    //Инициация и рефреш
    if (game_init==1)//инициация игры
     {
      title_show=0;//отключаем титульник
      title_menu=0;//отключаем главное меню		 
	
	  stopmusic();//останавливаем музыку	 
      Mix_VolumeMusic(music_volume);//громкость музыки(0-128)
      old_track=track;
      track=random(track_max);//выбираем случайно какой музыкальный трек играть
      if (track==old_track) {track=track+1;}//чтобы трек не повторялся подряд,следущий
      music(track,0);//играем выбранный трек

      level_num=default_level_num;//начинаем с первого уровня
      Start_LevelNum(level_num);
      
      game_init=0;//инициация игры завершена
      game_show=1;//включаем игру
     }

    //Рестарт игры
    if (game_show==1 and
        game_restart==1 and
        game_menu==1)
     {
      Start_LevelNum(level_num);     
      game_restart=0;//инициация игры завершена
      game_menu=0;
     }

    //Выходи из игры в главное меню
    if (game_exit==1 and 
        game_menu==1)
     {
      game_show=0;
      game_menu=0;
      game_exit=0;

      Title_Init(true);
      
      //В начале игры видим только титульник и гланое меню
      title_show=1;
      title_menu=1;
     }

    //============
    //=[РЕДАКТОР]=
    //============
    //Инициация и рефреш
    if (edit_init==1)//инициация игры
     {
      title_show=0;//отключаем титульник
      title_menu=0;//отключаем главное меню

      Edit_Init();
      level_num=default_level_num;//начинаем с первого уровня
      
      edit_init=0;//инициация редактора завершена
      edit_show=1;//включаем редактор
     }

    //Выход из редактора в главное меню
    if (edit_exit==1 and 
        edit_menu==1)
     {
      edit_show=0;
      edit_menu=0;
      edit_exit=0;

      Title_Init(true);
      
      //В начале игры видим только титульник и гланое меню
      title_show=1;
      title_menu=1;
     }

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
        //Перемотка уровня по умолчанию
        KeyButton_int(index_button_level_down,event,mouse_X,mouse_Y,&default_level_num,-1,1,level_max);
        KeyButton_int(index_button_level_up,event,mouse_X,mouse_Y,&default_level_num,+1,level_max,1);
        //Начать игру
        KeyButton_int(index_button_new_game,event,mouse_X,mouse_Y,&game_init,1,1,0);

        //Включить редактор
        KeyButton_int(index_button_edit,event,mouse_X,mouse_Y,&edit_init,1,1,0);

        //Остальное
        KeyRadioButton(index_rbutton_delay,
                       &delay,
                       event,
                       mouse_X,
                       mouse_Y);

        KeyRadioButton(index_rbutton_music_vol,
                       &music_volume,
                       event,
                       mouse_X,
                       mouse_Y);

        KeyRadioButton(index_rbutton_sound_vol,
                       &sound_volume,
                       event,
                       mouse_X,
                       mouse_Y);

        KeyButton_int(index_button_exit,event,mouse_X,mouse_Y,&pre_done,1,1,0);		   

       }

      //========
      //=[ИГРА] кнопки меню
      //======== +
      //============
      //=[РЕДАКТОР]=-кнопки меню
      //============
      if (game_show==1 and //в меню во время игры
          game_menu==1 or 
          edit_show==1 and //в меню во время редактора
          edit_menu==1)
          
       {	    
        //В игре
        if (game_show==1 and edit_show==0)
         {
          KeyButton_int(index_button_resume,event,mouse_X,mouse_Y,&game_menu,-1,0,1);
          //Bернуться в главное меню
          KeyButton_int(index_button_return,event,mouse_X,mouse_Y,&game_exit,1,1,0);
         }

        //B редакторе
        if (edit_show==1 and game_show==0)
         {
          KeyButton_int(index_button_resume,event,mouse_X,mouse_Y,&edit_menu,-1,0,1);
          //Bернуться в главное меню
          KeyButton_int(index_button_return,event,mouse_X,mouse_Y,&edit_exit,1,1,0);
         }         
         
        //Bнопки из опций
        KeyRadioButton(index_rbutton_delay,
                       &delay,
                       event,
                       mouse_X,
                       mouse_Y);

        KeyRadioButton(index_rbutton_music_vol,
                       &music_volume,
                       event,
                       mouse_X,
                       mouse_Y);

        KeyRadioButton(index_rbutton_sound_vol,
                       &sound_volume,
                       event,
                       mouse_X,
                       mouse_Y);
       }  

      //Ввод текста 
	  //Input(event,&name,100,&input_true);
      //cout<<name<<endl;
        
      //Управление с клавиатуры
      if (event.type==SDL_KEYDOWN)//еcли нажата кнопка на клавиатуре-однократное нажатие
       {
		if (event.key.keysym.sym==SDLK_F9) {track_change=1;}//статичные lev   

        if (event.key.keysym.sym==SDLK_u)//временнно
         {
	      game_init=1;
	      diff_game_mode=2;
	     }

        //========
        //=[ИГРА]=-однократное нажатие
        //========
        if (game_show==1) //во время игры
         {		 
          if (event.key.keysym.sym==SDLK_ESCAPE)//в игре можем включить меню по нажатию ESC
           {
            game_menu=not game_menu;
            
            Reset_Buttons(index_button_resume,index_rbutton_sound_vol);//чтобы при последущем входе не было глюков с отжатием кнопки
           }

          if (game_menu==0)//когда меню отключено
           {
            KeyClick_Player();//управление колобком
           }
         }

        //============
        //=[РЕДАКТОР]=-однократное нажатие
        //============
        if (edit_show==1) //во время редактирования
         {
          if (event.key.keysym.sym==SDLK_ESCAPE)//в редакторе можем включить меню по нажатию ESC
           {
            edit_menu=not edit_menu;
            
            Reset_Buttons(index_button_resume,index_rbutton_sound_vol);//чтобы при последущем входе не было глюков с отжатием кнопки
           }

          if (edit_menu==0)//когда меню отключено
           {
            Edit_Controls();//клавиши управления редактором
           }
         }

       }
     
     }//Конец условия на однократное нажатие

    keys = SDL_GetKeyState(NULL);

    //========
    //=[ИГРА]=-постоянное нажатие и обработка
    //========
    if (game_show==1 and//во время игры вне меню
        game_menu==0)
     {
      Game_Timer();//игровой таймер
      
      Matrix_ReFresh();//обновляем матрицы
      
      Render_PlayerColLevObj();//Обработка столконвений колобка c lev и obj
      KeyDown_Player();

      Render_PlayerMove();//Обработка движений колобка
      Mirror();
      Render_ObjColExit();//Если вихрь заходит на спец выход то дохнет, а очки на выход уменьшаються
      Render_ObjColTeleport();//Взаимодействие объекта с телепортом
      Render_ObjCheckMove();//Проверка возможности перемещения объекта
      Render_ObjColPlayer();//Взаимодействие объектов типа obj=1,2 с колобком
      Render_ObjMove();//Движения и псевдо АИ(поведение) объектов, анимация и контрол угла поворота

      Render_PlayerDead();//Повреждение колобка от бомбы и вихря
      Render_PlayerEatObj();//Колобок ест бонусы (obj=1) и собирает при этом очки
      Render_ObjKill();//Смерть объектов
      Render_ObjAfterKill();
      
      Level_Control_Doors();//текстура lev для разного состояния дверей
     }

    //============
    //=[РЕДАКТОР]=-постоянное нажатие и обработка
    //============
    if (edit_show==1 and//во время редактирования вне меню
        edit_menu==0)
     {
      Matrix_ReFresh();//обновляем матрицы		 
		 
      Cam_Controls_KeyDown();//управление камерой с клавиатуры
     }

    //========
    //=[ИГРА]=-вывод графики
    //========
    if (game_show==1)//во время игры

     { 
      //Задний фон
      glColor4f(1,1,1,1);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,game_back_TX);
      DrawRubberPoly(win_DX/2,
                     win_DY/2,
                     win_DX,
                     win_DY,
                     0,
                     1);		 
      Flame();

      Draw_Level();//Рисуем уровень
      Draw_LevelKal();
      Draw_Portals();//Рисуем телепорты
      Draw_Obj();//Рисуем объекты
      Draw_Player();//Рисуем колобка
      Draw_Effects();//Рисуем эффекты
      Draw_HUD();//Рисуем ХУД
     }

    //============
    //=[РЕДАКТОР]=-вывод графики
    //============
    if (edit_show==1)//во время редактора

     {
      //Задний фон
      glColor4f(1,1,1,1);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,game_back_TX);
      DrawRubberPoly(win_DX/2,
                     win_DY/2,
                     win_DX,
                     win_DY,
                     0,
                     1);		 
      Draw_Level();//Рисуем уровень
      Draw_Obj();//Рисуем объекты
      Cursor_Draw();//Рисуем курсор редактора
      Other_Draw();//вывод прочего
     }

    //Выводим номер музыкального трека и фпс
    if (show_fps==1)
     {
	  glColor4f(1,1,1,1);	 
      DrawText(win_DX-win_DX/5-win_DX/30,
               win_DY/76.8,//10
               win_DX/5,//128,
               win_DY/48,//16,
               1,//font_index
               //"name="+name+", track="+IntToStr(track)+", fps="+IntToStr(fps),
               "track="+IntToStr(track)+", fps="+IntToStr(fps),
               0,//delta
	           0);//center              
      }
      
     if (timer<=start_time and //логотип
         title_show==1 and
         title_menu==1)
       {
        //Задний фон
        glColor4f(1,1,1,1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D,logo_back_TX);
        DrawRubberPoly(win_DX/2,
                       win_DY/2,
                       win_DX,
                       win_DY,
                       0,
                       1);
       }
      
    //Вывод кнопок главного меню в титульнике
    if (timer>start_time and //разглядеть портрет на заднем фоне
        title_show==1 and
        title_menu==1)
     {
      //Задний фон
      glColor4f(1,1,1,1);
      glEnable(GL_TEXTURE_2D);
      glBindTexture(GL_TEXTURE_2D,game_back_TX);
      DrawRubberPoly(win_DX/2,
                     win_DY/2,
                     win_DX,
                     win_DY,
                     0,
                     1);                               		 
		 
      //Выводим название игры
      glColor4f(1,1,1,1);
      DrawText(win_DX/4.2,//x
	           0+2.5*win_DY/(12),//y
	           win_DX/1.8,//dX
	           win_DY/12,//dY
               2,//font_index
               "-=SwibPlex=-",
               0,//delta
	           0);//center		 
      //Выводим номер текущего уровня
      glColor4f(1,1,1,1);
      DrawText(win_DX/2-win_DX/8,//x
	           0+5.0*win_DY/(12)-1.0*win_DY/(12),//y
	           win_DX/3,//dX
	           win_DY/12,//dY
               1,//font_index
               "Level "+IntToStr(default_level_num),
               0,//delta
	           0);//center
      DrawText(win_DX/2-win_DX/8,//x
	           0+5.5*win_DY/(12)-1.0*win_DY/(12),//y
	           win_DX/3,//dX
	           win_DY/12,//dY
               1,//font_index
               level_name[default_level_num],
               0,//delta
	           0);//center	           
      DrawText(win_DX/2-win_DX/8,//x
	           0+6.0*win_DY/(12)-1.0*win_DY/(12),//y
	           win_DX/3,//dX
	           win_DY/12,//dY
               1,//font_index
               "Best time "+IntToStr(best_time[default_level_num]) + " sec." ,
               0,//delta
	           0);//center
		   
	  //Координаты кнопок
	  Button[index_button_level_down].x=win_DX/2-((win_DX/(1.28*2))-win_DX/(9.482*7))/2;
	  Button[index_button_level_down].y=0+7.0*0.5*win_DY/(6.86)-1.0*win_DY/(12);
	  Button[index_button_level_up].x=win_DX/2+((win_DX/(1.28*2))-win_DX/(9.482*7))/2;
	  Button[index_button_level_up].y=0+7.0*0.5*win_DY/(6.86)-1.0*win_DY/(12);
	  Button[index_button_new_game].x=win_DX/2;
	  Button[index_button_new_game].y=0+8.0*0.5*win_DY/(6.86)-1.0*win_DY/(12);
	  Button[index_button_edit].x=win_DX/2;
	  Button[index_button_edit].y=0+9.0*0.5*win_DY/(6.86)-1.0*win_DY/(12);
      RButton[index_rbutton_delay].x=win_DX/2;   
      RButton[index_rbutton_delay].y=0+10.0*0.5*win_DY/(6.86)-1.0*win_DY/(12);  
      RButton[index_rbutton_music_vol].x=win_DX/2;    
      RButton[index_rbutton_music_vol].y=0+11.0*0.5*win_DY/(6.86)-1.0*win_DY/(12);    
      RButton[index_rbutton_sound_vol].x=win_DX/2;   
      RButton[index_rbutton_sound_vol].y=0+12.0*0.5*win_DY/(6.86)-1.0*win_DY/(12);
      Button[index_button_exit].x=win_DX/2;
      Button[index_button_exit].y=0+13.0*0.5*win_DY/(6.86)-1.0*win_DY/(12); 
	    
      //Рисуем кнопки
      DrawButton(index_button_level_up,1,1,1,1);LightButton(index_button_level_up,cursor_TX,1,1,1,0.3);
      DrawButton(index_button_level_down,1,1,1,1);LightButton(index_button_level_down,cursor_TX,1,1,1,0.3);

      DrawButton(index_button_new_game,1,1,1,1);LightButton(index_button_new_game,cursor_TX,1,1,1,0.3);
      DrawButton(index_button_edit,1,1,1,1);LightButton(index_button_edit,cursor_TX,1,1,1,0.3);
      DrawRadioButton(index_rbutton_delay,cursor_TX,1,1,1,1,1,1,1,0.3);LightRadioButton(index_rbutton_delay,cursor_TX,1,1,1,0.3);
      DrawRadioButton(index_rbutton_music_vol,cursor_TX,1,1,1,1,1,1,1,0.3);LightRadioButton(index_rbutton_music_vol,cursor_TX,1,1,1,0.3);
      DrawRadioButton(index_rbutton_sound_vol,cursor_TX,1,1,1,1,1,1,1,0.3);LightRadioButton(index_rbutton_sound_vol,cursor_TX,1,1,1,0.3);
      DrawButton(index_button_exit,1,1,1,1);LightButton(index_button_exit,cursor_TX,1,1,1,0.3);		 
     }

    //========
    //=[ИГРА]=-вывод меню
    //========
    //Вывод кнопок меню в игре
    if (game_show==1 and
        game_menu==1 or 
        edit_show==1 and
        edit_menu==1)
     {
	  //Координаты кнопок
	  Button[index_button_resume].x=win_DX/2;
	  Button[index_button_resume].y=0+5.0*0.5*win_DY/(6.86);
      RButton[index_rbutton_delay].x=win_DX/2;   
      RButton[index_rbutton_delay].y=0+6.0*0.5*win_DY/(6.86);  
      RButton[index_rbutton_music_vol].x=win_DX/2;    
      RButton[index_rbutton_music_vol].y=0+7.0*0.5*win_DY/(6.86);    
      RButton[index_rbutton_sound_vol].x=win_DX/2;   
      RButton[index_rbutton_sound_vol].y=0+8.0*0.5*win_DY/(6.86);
      Button[index_button_return].x=win_DX/2;
	  Button[index_button_return].y=0+9.0*0.5*win_DY/(6.86);		 
      
      DrawButton(index_button_resume,1,1,1,1);LightButton(index_button_resume,cursor_TX,1,1,1,0.3);
      DrawRadioButton(index_rbutton_delay,cursor_TX,1,1,1,1,1,1,1,0.3);LightRadioButton(index_rbutton_delay,cursor_TX,1,1,1,0.3);
      DrawRadioButton(index_rbutton_music_vol,cursor_TX,1,1,1,1,1,1,1,0.3);LightRadioButton(index_rbutton_music_vol,cursor_TX,1,1,1,0.3);
      DrawRadioButton(index_rbutton_sound_vol,cursor_TX,1,1,1,1,1,1,1,0.3);LightRadioButton(index_rbutton_sound_vol,cursor_TX,1,1,1,0.3);
      DrawButton(index_button_return,1,1,1,1);LightButton(index_button_return,cursor_TX,1,1,1,0.3);	
     }

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
