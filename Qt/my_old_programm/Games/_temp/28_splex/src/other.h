//Разработано aureli0,NeonLight_Child,Ph0Eni}{,Zinoviev54,imanavhod-это все я:)
//http://sourceforge.net/projects/swibplex/
//последний билд от 23.12.2009

void SaveConfFile()//сохранить файл конфигурации
 {
  ofstream data_file;//файловый поток
  //открываем файл данных
  sprintf(buff,"splex.cfg");
  data_file.open(buff); 

  if(data_file.is_open())
   {
    data_file << "win_DX=" << tmp_win_DX <<endl;
    data_file << "win_DY=" << tmp_win_DY <<endl;
    data_file << "pal=" << tmp_pal <<endl;
    data_file << "full_screen=" << tmp_full_screen <<endl;
    data_file << "delay=" << delay <<endl;
    data_file << "game_speed=" << game_speed <<endl;
    data_file << "grid=" << grid <<endl;
    data_file << "flame_summ=" << flame_summ <<endl;
    data_file << "music_volume=" << music_volume <<endl;
    data_file << "sound_volume=" << sound_volume <<endl;
    data_file << "default_level_num=" << default_level_num <<endl;
    data_file << "level_max=" << level_max <<endl;
    data_file << "track_max=" << track_max <<endl;
    data_file << "show_fps=" << show_fps <<endl;
    data_file.close();
   } 
  else 
   {
    cout << "Could not open ' ' for writing." << endl;
    done=1;
   }
 }

void LoadConfFile()//грузим конфигурационный файл
 {
  //Загружаем параметры карты
  ifstream data_file;//файловый поток
  string line,//строчка в файле
         set,//определитель
         val;//значение

  //открываем файл данных
  sprintf(buff,"splex.cfg");
  data_file.open(buff);
 
  if(data_file.is_open())//если файл открылся, то
   {
    while(!data_file.eof())//пока не дойдем до конца файла
     {
      getline(data_file, line);//забиваем содержание файла в строчку
 
      if(line.find('=') != string::npos)//ищем в полученной строчке параметры  
       {
        set=line.substr(0,line.find('='));//от 0 символа в строке до "="
        val=line.substr(line.find('=')+1);//значение после "="
       
        if (set=="win_DX")
           {win_DX=atoi(val.data());} 
        else 

        if (set=="win_DY")
           {win_DY=atoi(val.data());} 
        else 

        if (set=="pal")
           {pal=atoi(val.data());} 
        else 

        if (set=="full_screen")
           {full_screen=atoi(val.data());} 
        else 

        if (set=="delay")
           {delay=atoi(val.data());} 
        else 

        if (set=="game_speed")
           {game_speed=atoi(val.data());} 
        else 

        if (set=="grid")
           {grid=atoi(val.data());} 
        else 

        if (set=="flame_summ")
           {flame_summ=atoi(val.data());} 
        else 

        if (set=="music_volume")
           {music_volume=atoi(val.data());} 
        else 

       if (set=="sound_volume")           
          {sound_volume=atoi(val.data());} 
        else 

       if (set=="default_level_num")           
          {default_level_num=atoi(val.data());} 
        else 

       if (set=="level_max")           
          {level_max=atoi(val.data());} 
        else 

       if (set=="track_max")           
          {track_max=atoi(val.data());} 
        else 
        
       if (set=="show_fps")           
          {show_fps=atoi(val.data());} 
        else  

         //если не понимаем чтото то ругаемся на это
         {
          cout << "Sho ce take? '"<<set<<"'" << endl;
         }

       }//конец условия "ищем в полученной строчке параметры"
     }//конец while
    data_file.close();//закрываем файл
   }//конец условия 

  else 
   {
    cout << "Configa nema :(" << endl;
    done=1;
   }
 }

void SaveBestTime()//сохраняем результаты лучшего времени
 {
  ofstream data_file;//файловый поток
  int i;

  //открываем файл данных
  sprintf(buff,"btime.txt");
  data_file.open(buff); 

  if(data_file.is_open())
   {
    for(i=1; i <= level_max; i++) 
     {
      data_file <<"best_time=" << best_time[i] <<endl;
     }
    data_file.close();
   } 
  else 
   {
    cout << "Could not open ' ' for writing." << endl;
    done=1;
   }
 }

void LoadBestTime()//грузим файл лучшего времени
 {
  //Загружаем параметры карты
  ifstream data_file;//файловый поток
  string line,//строчка в файле
         set,//определитель
         val;//значение
  int i;

  //открываем файл данных
  sprintf(buff,"btime.txt");
  data_file.open(buff);
 
  if(data_file.is_open())//если файл открылся, то
   {
   i=1;
    while(!data_file.eof())//пока не дойдем до конца файла
     {
      getline(data_file, line);//забиваем содержание файла в строчку
 
      if(line.find('=') != string::npos)//ищем в полученной строчке параметры  
       {
        set=line.substr(0,line.find('='));//от 0 символа в строке до "="
        val=line.substr(line.find('=')+1);//значение после "="

        if (set=="best_time")//~номер строки это номер уровня
           {
            best_time[i]=atoi(val.data());
            if (i<=level_max) {i=i+1;}         
           } 
        else 
        //если не понимаем чтото то ругаемся на это
         {
          cout << "I did not understand the '"<<set<<"'" << endl;
         }
       }//конец условия "ищем в полученной строчке параметры"
     }//конец while
    data_file.close();//закрываем файл
   }//конец условия 

  else 
   {
    cout << "Best Time file not found" << endl;
    done=1;
   }
 }

void Save_Level(int level_num)//сохраняем уровень
 {
  sprintf(buff,"data/map/lev%i.dat",level_num);
  fp=fopen(buff,"w");//статичные lev
  fwrite((char *)lev, sizeof(struct lev_type), lev_num_max, fp);
  fclose (fp); 

  sprintf(buff,"data/map/obj%i.dat",level_num);//грузим уровень
  fp=fopen(buff,"w");//динамические obj
  fwrite((char *)obj, sizeof(struct obj_type), obj_num_max, fp);
  fclose (fp); 
 }

void Load_Level(int level_num)
 {
  sprintf(buff,"data/map/lev%i.dat",level_num);
  fp=fopen(buff,"r");
  fread((char *)lev, sizeof(struct lev_type), lev_num_max, fp);
  fclose (fp); 

  sprintf(buff,"data/map/obj%i.dat",level_num);
  fp=fopen(buff,"r");
  fread((char *)obj, sizeof(struct obj_type), obj_num_max, fp);
  fclose (fp);
 }

void LoadLevelList()//грузим список названий уровней
 {
  //Загружаем параметры карты
  ifstream data_file;//файловый поток
  string line,//строчка в файле
         set,//определитель
         val;//значение
  int i;

  //открываем файл данных
  sprintf(buff,"level.lst");
  data_file.open(buff);
 
  if(data_file.is_open())//если файл открылся, то
   {
   i=1;
    while(!data_file.eof())//пока не дойдем до конца файла
     {
      getline(data_file, line);//забиваем содержание файла в строчку
 
      if(line.find('=') != string::npos)//ищем в полученной строчке параметры  
       {
        set=line.substr(0,line.find('='));//от 0 символа в строке до "="
        val=line.substr(line.find('=')+1);//значение после "="

        if (set=="name")//~номер строки это номер уровня
           { 
            level_name[i]=val.data();
            if (i<=level_max) {i=i+1;}                      
           } 
        else 
        //если не понимаем чтото то ругаемся на это
         {
          cout << "I did not understand the '"<<set<<"'" << endl;
         }
       }//конец условия "ищем в полученной строчке параметры"
     }//конец while
    data_file.close();//закрываем файл
   }//конец условия 
  else 
   {
    cout << "Level List file not found" << endl;
    done=1;
   }
 }

void Game_Timer()
 {
  //игровой таймер
  game_now = SDL_GetTicks();
  if ((game_now-game_then)>1000 or game_now<game_then)
   {
    game_then = SDL_GetTicks();
    game_timer=game_timer+1;//игровой секундный таймер
   } 
 }

void Cam_Controls_KeyDown()//управление камерой с клавиатуры
 {
  //управление камерой
  if (keys[SDLK_w]) {y_cam=y_cam+0.15*grid;}
  if (keys[SDLK_s]) {y_cam=y_cam-0.15*grid;} 
  if (keys[SDLK_a]) {x_cam=x_cam+0.15*grid;} 
  if (keys[SDLK_d]) {x_cam=x_cam-0.15*grid;}

  //перемещение игрового экрана-камеры
  if ((x_cursor*grid+x_cam)>(win_DX-0.3333*win_DX)) {x_cam=x_cam-0.15*grid;}//смещение на 0.15 от grid
  if ((x_cursor*grid+x_cam)<(0+0.3333*win_DX)) {x_cam=x_cam+0.15*grid;}
  if ((y_cursor*grid+y_cam)>(win_DY-0.3333*win_DY)) {y_cam=y_cam-0.15*grid;}
  if ((y_cursor*grid+y_cam)<(0+0.3333*win_DY)) {y_cam=y_cam+0.15*grid;}
 }

//грузим игровые текстуры
void Load_Game_Textures()
 {
  //Загрузка текстур колобка
  for(i=1;i<8+1;i++)
   {
    sprintf(buff,"data/img/player/%i.png",i);
    load(buff, player_texture[i]);
   }

  //Загрузка текстур объектов
  for(i=1;i<9+1;i++)
   {
    sprintf(buff,"data/img/obj/%i.png",i);
    load(buff, obj_texture[i]);
   }

   //Загрузка анимационных текстур объектов 
  for(i=1;i<6+1;i++)
   {
    sprintf(buff,"data/img/obj/7_%i.png",i);
    load(buff, obj_anim_texture[i]);
   }

  //Загрузка текстур уровня
  for(i=1;i<31+1;i++)
   {
    sprintf(buff,"data/img/lev/%i.png",i);
    load(buff, lev_texture[i]);
   }
  
  //Загрузка текстур эффектов
  for(i=1;i<16+1;i++)//взрыв
   {
    sprintf(buff,"data/img/gfx/light/%i.png",i);
    load(buff, eff_texture[1][i]);
   }

  for(i=1;i<20+1;i++)//молния
   {
    sprintf(buff,"data/img/gfx/boom/%i.png",i);
    load(buff, eff_texture[2][i]);
   }

  for(i=1;i<8+1;i++)//молния
   {
    sprintf(buff,"data/img/gfx/blood/%i.png",i);
    load(buff, eff_texture[3][i]);
   }
  load("data/img/gfx/swirl/swirl.png", t_texture);//текстура телепорта

  //текстуры тьмы и света
  load("data/img/gfx/dark/dark.png", dark_texture);//текстура тьмы

  //Загрузка других текстур
  load("data/img/other/kal.png", other_texture[1]);
  load("data/img/other/livebar.png", other_texture[2]);
  load("data/img/other/hud.png", other_texture[3]);
  load("data/img/other/open_exit.png", other_texture[4]);
  load("data/img/other/button_on.png", other_texture[5]);
 }

void Load_SFX()//грузим музыку и звуки
 {
  loadmusic("./data/music/menu.s3m",1);//грузим файл в плэейлист с номером 1

  //Составление трек листа
  for(i=2;i<track_max+1;i++)
   {
    sprintf(buff,"./data/music/%i.xm",i);
    loadmusic(buff,i);
   }

  loadsound("./data/sound/boom.wav",1);  
  loadsound("./data/sound/bonus.wav",2);  
  loadsound("./data/sound/superbonus.wav",3);  
  loadsound("./data/sound/health.wav",4);  
  loadsound("./data/sound/silver.wav",5);  
  loadsound("./data/sound/gold.wav",6);  
  loadsound("./data/sound/teleport.wav",7); 
  loadsound("./data/sound/hrum.wav",8);
  loadsound("./data/sound/opendoor.wav",9);
  loadsound("./data/sound/glass.wav",10);
  loadsound("./data/sound/blood.wav",11);
  loadsound("./data/sound/fall.wav",12);
 }        

//начальные значения всех переменных при старте
void Init_Game() 
 {
//игровой таймер
  game_now=0;
  game_then=0;
  game_timer=0;

 //параметры колобка при старте
  p_x=1;//место старта колобка по умолчанию
  p_y=1;
  p_x_tmp=p_x;
  p_y_tmp=p_y;
  p_move_x=0;
  p_move_y=0;
  p_move_stage=0;

  p_vector=4;//смотрим влево
  p_frame_start=3;//стартовый кадр
  p_frame=p_frame_start;
  p_frame_count=0;
  p_face_frame_count=0;

  p_score=0;
  p_silver=0;
  p_gold=0;

  p_live_max=45;
  p_live=p_live_max;
  
  for(i=0;i<lev_num_max;i++) 
   {
    p_kal[i]=0;
   }

  p_move_left=false;//движется влево
  p_move_right=false;
  p_move_up=false;   
  p_move_down=false;
  p_move_left_v=false;//может ли двигаьтся влево 
  p_move_right_v=false;     
  p_move_up_v=false;     
  p_move_down_v=false; 
  p_kick_left=false; 
  p_kick_right=false;
  p_true=true;//определяет жив ли колобок

//начальные параметры телепорта
  t1x=0;
  t1y=0;
  t1y_tmp=t1y;
  t2x=0;
  t2y=0;//координаты входа и выхода из телепорта
  t2y_tmp=t2y;
  t1_true=false;
  t2_true=false;//определяет есть ли телепорты

//начальные параметры переменных элементов уровня
  for(j=0;j<lev_num_max;j++) 
   {
    lev[j].x=0;
    lev[j].y=0;
    lev[j].unit=0;
    lev[j].exit_score=0;
    lev[j].exit_kills=0;
   }

//начальные параметры переменных динамических объектов
  for(j=0;j<obj_num_max;j++) 
   {
    obj[j].x=0;
    obj[j].y=0;
    obj[j].unit=0;

    obj_index[j]=0;//индекс объекта
    obj_move_x[j]=0;//смещение
    obj_move_y[j]=0;
    obj_delta[j]=0;//угол поворота относительно центра
    obj_sigma[j]=0;
    obj_rotate[j]=0;//определяет тип кручения объекта 1 - при движении, 2- все время
    obj_AI_param1[j]=1;//для перемещения объекта - псевдо АИ
    obj_AI_param2[j]=0;
    obj_sleep_count[j]=0;//таймер покоя, после падения = 5,затем вычитается до 0
    
    obj_nuke_count[j]=0;//счетчик до взрыва объекта

    obj_move[j]=false;//объект движется
    obj_move_up[j]=false;
    obj_move_down[j]=false;
    obj_move_left[j]=false;
    obj_move_right[j]=false;
    obj_move_up_v[j]=false;//объект может двинутся вверх
    obj_move_down_v[j]=false;
    obj_move_left_v[j]=false;
    obj_move_right_v[j]=false;
     
     //для проверок на сваливаемость для элементов OBJ 1,2
    obj_move_up_left_v[j]=false;//вверх-влево
    obj_move_up_right_v[j]=false;//вверх-вправо
    obj_move_down_left_v[j]=false;//вниз-влево
    obj_move_down_right_v[j]=false;//вниз-вправо
    
    obj_move_kick_left_mark[j]=false;//отмечен для тычка слева
    obj_move_kick_right_mark[j]=false;//отмечен для тычка справа

    obj_dead[j]=0;//мертв ли объект
    obj_nuke[j]=false;//объект на подрыве

    obj_anim[j]=false;
    obj_frame[j]=0;//текущий кадр анимации
    obj_frame_count[j]=0;//счетчик задержки между кадрами

    obj_angle_finish[j]=0;//финальный угол поворота
    obj_angle_count[j]=0;
    obj_d1[j]=0;
    obj_d2[j]=0;
    obj_napr[j]=0;  
   }//конец цикла

  obj_nuke_index=0;//индекс взрывающегося объекта
  obj_kick_index=0;

//обнуление матриц
  for(j=0;j<lev_width+1;j++)
  for(k=0;k<lev_height+1;k++)
   {
    obj_matrix[j][k]=0;
    lev_matrix[j][k]=0;
    obj_matrix_tmp[j][k]=0;
    lev_matrix_tmp[j][k]=0;
   }

//обнуление состояния дверей
  for(i=0; i < lev_num_max; i++) 
   {
    lev_door_open[i]=false;//двери закрыты
    lev_button_push[i]=false;//кнопки не нажаты
    lev_light_push[i]=false;//светильники выключены 
   }

//остальное
  if (diff_game_mode==2) 
   {
    lev_dark_radius=3;//радиус света от фонарей
    dark_radius=3;//видимый радиус Свиборгом
   }

  x_cam=0;
  y_cam=0;
  
  delta=0;//для телепортов и курсоров

 }

void Mirror()//отражение уровня по вертикали
 {
  if (mirror==true)
   {
    mirror=false;
    mirror_true=not mirror_true;
//для игрока
    p_y_tmp=lev_height-p_y;
    p_y=p_y_tmp;
//для телепортов
    if (t1_true==true){t1y_tmp=lev_height-t1y;t1y=t1y_tmp;}
    if (t2_true==true){t2y_tmp=lev_height-t2y;t2y=t2y_tmp;}
//для lev
    bool lev_mirror[obj_num_max];

    for(j=0; j < lev_num_max; j++)//для односторонних переходов вверх или вниз!!! 
     {
      if (lev[j].unit==22 and lev_mirror[j]==false) 
       {
        lev[j].unit=23;
        lev_matrix[lev[j].x][lev[j].y]=lev[j].unit;
        lev_mirror[j]=true;
       }
      if (lev[j].unit==23 and lev_mirror[j]==false) 
       {
        lev[j].unit=22;
        lev_matrix[lev[j].x][lev[j].y]=lev[j].unit;
        lev_mirror[j]=true;
       }
     }//конец переходов

    for(j=0; j < lev_num_max; j++) 
    for(k=0; k <=lev_height; k++) 
     {
      if (lev[j].y==k) 
       {
        lev_tmp[j].y=lev_height-k;
        lev_matrix_tmp[lev[j].x][lev[j].y]=lev_matrix[lev[j].x][lev_tmp[j].y];
        lev_matrix[lev[j].x][lev[j].y]=0;
       }
     }
    for(j=0; j < lev_num_max; j++) 
     {
      lev[j].y=lev_tmp[j].y;
      lev_matrix[lev[j].x][lev[j].y]=lev_matrix_tmp[lev[j].x][lev[j].y];
     }

//для obj
    for(j=0; j < obj_num_max; j++) 
    for(k=0; k <=lev_height; k++) 
     {
      if (obj[j].y==k) 
       {
        obj_tmp[j].y=lev_height-k;
        obj_matrix_tmp[obj[j].x][obj[j].y]=obj_matrix[obj[j].x][obj_tmp[j].y];
         
        for(l=0; l <=lev_width; l++)//затираем матрицу 
        for(m=0; m <=lev_height; m++) 
         {
          obj_matrix[l][m]=0;
         }

        obj_move[j]=false;
        obj_move_up[j]=false;//стираем объект из матрицы объектов
        obj_move_down[j]=false;
        obj_move_left[j]=false;
        obj_move_right[j]=false;

        obj_move_up_v[j]=false;//объект может двинутся вверх
        obj_move_down_v[j]=false;
        obj_move_left_v[j]=false;
        obj_move_right_v[j]=false;
        obj_move_up_left_v[j]=false;//вверх-влево
        obj_move_up_right_v[j]=false;//вверх-вправо
        obj_move_down_left_v[j]=false;//вниз-влево
        obj_move_down_right_v[j]=false;//вниз-вправо
    
        obj_move_kick_left_mark[j]=false;
        obj_move_kick_right_mark[j]=false;
         
        obj_move_x[j]=0;
        obj_move_y[j]=0;

        obj_rotate[j]=0;
        obj_delta[j]=0;
        obj_sigma[j]=0;

        if (obj[j].unit==9)//меняем направления вражин-вертикальной хрени
         {
          if (obj_AI_param1[j]==1) {obj_AI_param1[j]=2; goto metka1;}
          if (obj_AI_param1[j]==2) {obj_AI_param1[j]=1; goto metka1;}
          metka1:;
         }

       } 
     }//конец цикла

    for(j=0; j < obj_num_max; j++) 
     {
      obj[j].y=obj_tmp[j].y;
      obj_matrix[obj[j].x][obj[j].y]=obj_matrix_tmp[obj[j].x][obj[j].y];
     }
 
//для тьмы
    if (diff_game_mode==2) //в сложном ночном режиме игры
     {
      for(i=0; i <= lev_width; i++) 
      for(j=0; j <= lev_height;j++)
       {
        dark_matrix_tmp[i][j]=dark_matrix[i][lev_height-j];
       }

      for(i=0; i <= lev_width; i++) 
      for(j=0; j <= lev_height;j++)
       {
        dark_matrix[i][j]=dark_matrix_tmp[i][j];
       }
     }

   }//конец условия
 }

void Matrix_ReFresh()//Рефреш матриц
 {
  //обновляем матрицу уровня
  for(i=0; i < lev_num_max; i++) 
   {
    lev_matrix[lev[i].x][lev[i].y]=lev[i].unit;
   }

  //обновляем матрицу объектов    
  for(j=0; j < obj_num_max; j++) 
   { 
    obj_index[j]=j;
    if (obj_dead[j]==0) {obj_matrix[obj[j].x][obj[j].y]=obj[j].unit;}
    if (obj_dead[j]>0) {obj_matrix[obj[j].x][obj[j].y]=0;}
   }
 }

 void HUD_Draw()//Рисуем ХУД
 {
  //ХУД
  glBindTexture(GL_TEXTURE_2D,other_texture[3]);
  DrawTXT(0,0,
          win_DX/2,//512
          win_DY/12,//64
          0,0); 

  //значок молекулы
  glBindTexture(GL_TEXTURE_2D,obj_texture[1]);
  DrawTXT(win_DX/37.93,//27,
          win_DY/30.72,//25
          win_DX/51.2,//20,
          win_DY/38.4,//20,
          0,1);

  //лайв бар
  if (p_live>0)
   {     
    glBindTexture(GL_TEXTURE_2D,other_texture[2]);
    DrawTXT(win_DX/22.756,//45,
            win_DY/27.429,//28,
            p_live*(win_DX/102.4/*10*/),
            win_DY/38.4,
            0,0); 
   } 

  //выводим число очков    
  sprintf(txt_tmp,"%i",p_score);
  GlTxt(win_DX/20.48,//50,
        win_DY/48,//16,
        fonts[2],txtColorWhite,txt_tmp,t_txt);
  glBindTexture(GL_TEXTURE_2D,t_txt);
  DrawTXT(win_DX/64,//16,
          win_DY/21.33,//36
          win_DX/20.48,//50,
          win_DY/48,//16,
          0,0);

  //выводим номер уровня
  sprintf(txt_tmp,"lev=%i s=%i g=%i time=%is (best=%is)",
          level_num,p_silver,p_gold,game_timer,best_time[level_num]);//номер уровня
  GlTxt(win_DX/2,//140
        win_DY/48,//16,
        fonts[2],txtColorWhite,txt_tmp,t_txt);
  glBindTexture(GL_TEXTURE_2D,t_txt);
  DrawTXT(win_DX/22.756,//45
          win_DY/76.8,//10
          win_DX/2,//140
          win_DY/48,//16,
          0,0);
 }   






