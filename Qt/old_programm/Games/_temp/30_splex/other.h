//http://swibplex.sourceforge.com

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
          cout << "??? '"<<set<<"'" << endl;
         }

       }//конец условия "ищем в полученной строчке параметры"
     }//конец while
    data_file.close();//закрываем файл
   }//конец условия 

  else 
   {
    cout << "Config file 'splex.cfg' not found" << endl;
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
    for(int i=1; i <= level_max; i++) 
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
  fwrite((char *)lev, sizeof(struct lev_type), lev_max, fp);
  fclose (fp); 

  sprintf(buff,"data/map/obj%i.dat",level_num);//грузим уровень
  fp=fopen(buff,"w");//динамические obj
  fwrite((char *)obj, sizeof(struct obj_type), obj_max, fp);
  fclose (fp); 
 }

void Load_Level(int level_num)
 {
  sprintf(buff,"data/map/lev%i.dat",level_num);
  fp=fopen(buff,"r");
  fread((char *)lev, sizeof(struct lev_type), lev_max, fp);
  fclose (fp); 

  sprintf(buff,"data/map/obj%i.dat",level_num);
  fp=fopen(buff,"r");
  fread((char *)obj, sizeof(struct obj_type), obj_max, fp);
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

//Грузим игровые текстуры
void Load_Game_Textures()
 { 
	 
  //Загрузка заднего фона  титульника
  load_LINEAR("data/img/bground/main_back.jpg",logo_back_TX);
  load_LINEAR("data/img/other/cursor.png", cursor_TX);//Текстура курсора
  load_LINEAR("data/img/bground/game_back.jpg", game_back_TX);//Загрузка заднего игрового фона
	 
  //Загрузка текстур колобка
  for(int i=1;i<8+1;i++)
   {
    sprintf(buff,"data/img/player/%i.png",i);
    load_NEAREST(buff, player_texture[i]);
   }

  //Загрузка текстур объектов
  for(int i=1;i<9+1;i++)
   {
    sprintf(buff,"data/img/obj/%i.png",i);
    load_NEAREST(buff, obj_texture[i]);
   }

   //Загрузка анимационных текстур объектов 
  for(int i=1;i<6+1;i++)
   {
    sprintf(buff,"data/img/obj/7_%i.png",i);
    load_NEAREST(buff, obj_anim_texture[i]);
   }

  //Загрузка текстур уровня
  for(int i=1;i<31+1;i++)
   {
    sprintf(buff,"data/img/lev/%i.png",i);
    load_NEAREST(buff, lev_texture[i]);
   }
  
  //Загрузка текстур эффектов
  for(int i=1;i<16+1;i++)//взрыв
   {
    sprintf(buff,"data/img/gfx/light/%i.png",i);
    load_NEAREST(buff, eff_texture[1][i]);
   }

  for(int i=1;i<20+1;i++)//молния
   {
    sprintf(buff,"data/img/gfx/boom/%i.png",i);
    load_NEAREST(buff, eff_texture[2][i]);
   }

  for(int i=1;i<8+1;i++)//молния
   {
    sprintf(buff,"data/img/gfx/blood/%i.png",i);
    load_NEAREST(buff, eff_texture[3][i]);
   }
  load_NEAREST("data/img/gfx/swirl/swirl.png", t_texture);//текстура телепорта

  //текстуры тьмы и света
  //load_NEAREST("data/img/gfx/dark/dark.png", dark_texture);//текстура тьмы

  //Загрузка других текстур
  load_NEAREST("data/img/other/kal.png", other_texture[1]);
  load_NEAREST("data/img/other/livebar.png", other_texture[2]);
  load_NEAREST("data/img/other/hud.png", other_texture[3]);
  load_NEAREST("data/img/other/open_exit.png", other_texture[4]);
  load_NEAREST("data/img/other/button_on.png", other_texture[5]);
 }

void Load_SFX()//грузим музыку и звуки
 {
  loadmusic("./data/music/menu.s3m",1);//грузим файл в плэейлист с номером 1

  //Составление трек листа
  for(int i=2;i<track_max+1;i++)
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










