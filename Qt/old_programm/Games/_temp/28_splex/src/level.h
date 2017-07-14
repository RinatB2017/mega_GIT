//Разработано aureli0,NeonLight_Child,Ph0Eni}{,Zinoviev54,imanavhod-это все я:)
//http://sourceforge.net/projects/swibplex/
//последний билд от 23.12.2009

void kill_lev(int i)//уничтожить элемент уровня
 {
  lev_matrix[lev[i].x][lev[i].y]=0;
  lev[i].x=0;
  lev[i].y=0;
  lev[i].unit=0;

  lev[i].exit_score=0;
  lev[i].exit_kills=0;
 }

void Init_Lev_Doors()//инициализация состояния дверей
 {
  for(j=0; j < lev_num_max; j++) 
   {
    if (lev[j].unit==4) {lev_door_open[j]=true;}//дверь открыта
    if (lev[j].unit==12) {lev_door_open[j]=false;}//дверь закрыта
   }
 }

void Init_Lev_Lights()//инициализация состояния светильников
 {
  for(j=0; j < lev_num_max; j++) 
   {
    if (lev[j].unit==31) {lev_light_push[j]=true;}//дверь открыта
    if (lev[j].unit==30) {lev_light_push[j]=false;}//дверь закрыта
   }
 }
 
 void Level_Control_Doors()//текстура lev для разного состояния дверей
 {
  for(j=0; j < lev_num_max; j++) 
   {
    if (lev[j].unit==4 and//дверь открыта
        lev_matrix[lev[j].x][lev[j].y]==lev[j].unit or
        lev[j].unit==12 and//дверь открыта
        lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
     {
      if (lev_door_open[j]==true) {lev[j].unit=4;} //дверь открыта
      if (lev_door_open[j]==false) {lev[j].unit=12;} //дверь закрыта
     }
   }
 }

void Level_Control_Lights()//текстура lev для разного состояния дверей
 {
  for(j=0; j < lev_num_max; j++) 
   {
    if (lev[j].unit==31 and//лампочка вкл
        lev_matrix[lev[j].x][lev[j].y]==lev[j].unit or
        lev[j].unit==30 and//лампочка выкл
        lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
     {
      if (lev_light_push[j]==true) {lev[j].unit=31;} //лампочка вкл
      if (lev_light_push[j]==false) {lev[j].unit=30;} //лампочка выкл
     }
   }
 }
   
void Level_Draw()//Рисуем уровень
 { 
  for(i=0; i < lev_num_max; i++) 
   {
    //рисуем кал(нули) после того как lev=1
    //если элемент уровня существует и попадает в окно вывода-его рисуем
    if (game_show==1 and //в игре
        diff_game_mode==1 and //при обычном режиме сложности
        lev[i].unit==0 and
        p_kal[i]==true and//если это кал 
        lev[i].x*grid+x_cam>0-grid and
        lev[i].x*grid+x_cam<win_DX+grid and 
        lev[i].y*grid+y_cam>0-grid and
        lev[i].y*grid+y_cam<win_DY+grid or

        game_show==1 and //в игре
        diff_game_mode==2 and //при повышеном режиме сложности
        dark_matrix[lev[i].x][lev[i].y]==0 and
        lev[i].unit==0 and
        p_kal[i]==true and//если это кал 
        lev[i].x*grid+x_cam>0-grid and
        lev[i].x*grid+x_cam<win_DX+grid and 
        lev[i].y*grid+y_cam>0-grid and
        lev[i].y*grid+y_cam<win_DY+grid or

        game_show==1 and //в игре 
        diff_game_mode==2 and //при повышеном режиме сложности
        lev[i].unit==0 and
        p_kal[i]==true and//если это кал 
        lev[i].x>p_x-dark_radius and
        lev[i].x<p_x+dark_radius and 
        lev[i].y>p_y-dark_radius and
        lev[i].y<p_y+dark_radius)
     {
      glBindTexture(GL_TEXTURE_2D,other_texture[1]);
      DrawTXT (lev[i].x*grid+x_cam,
               lev[i].y*grid+y_cam,
               grid,grid,0,1);  
     }
    //Рисуем уровень
    if (game_show==1 and //в игре
        diff_game_mode==1 and //при обычном режиме сложности
        lev[i].unit>0 and
        lev[i].x*grid+x_cam>0-grid and
        lev[i].x*grid+x_cam<win_DX+grid and 
        lev[i].y*grid+y_cam>0-grid and
        lev[i].y*grid+y_cam<win_DY+grid or 

        game_show==1 and //в игре
        diff_game_mode==2 and //при повышенном режиме сложности
        dark_matrix[lev[i].x][lev[i].y]==0 and
        lev[i].unit>0 and
        lev[i].x*grid+x_cam>0-grid and
        lev[i].x*grid+x_cam<win_DX+grid and 
        lev[i].y*grid+y_cam>0-grid and
        lev[i].y*grid+y_cam<win_DY+grid or 

        game_show==1 and //в игре
        diff_game_mode==2 and //при повышенном режиме сложности
        lev[i].unit>0 and
        lev[i].x>p_x-dark_radius and
        lev[i].x<p_x+dark_radius and 
        lev[i].y>p_y-dark_radius and
        lev[i].y<p_y+dark_radius or

        edit_show==1 and //в редакторе
        lev[i].unit>0 and
        lev[i].x*grid+x_cam>0-grid and
        lev[i].x*grid+x_cam<win_DX+grid and 
        lev[i].y*grid+y_cam>0-grid and
        lev[i].y*grid+y_cam<win_DY+grid)
     {
      //соответствующему элементу уровня свои текстуры
      glBindTexture(GL_TEXTURE_2D,lev_texture[lev[i].unit]);
      if (lev[i].unit==2 and p_score>=lev[i].exit_score or //если можно войти в выход
          lev[i].unit==3 and lev[i].exit_kills==0) 
       {glBindTexture(GL_TEXTURE_2D,other_texture[4]);}//дверь с уровня открыта

      if (lev[i].unit==29 and lev_button_push[i]==true) 
       {glBindTexture(GL_TEXTURE_2D,other_texture[5]);}//кнопка нажата

      DrawTXT (lev[i].x*grid+x_cam,
               lev[i].y*grid+y_cam,
               grid,grid,0,1);  

      //если дверь на выход, то на ней выводим количество очков для выхода из уровн
      if (lev[i].unit==2 and //количество очков на выход
          lev[i].exit_score>0 and
          p_score<lev[i].exit_score) 
       {
        sprintf(txt_tmp,"%i",lev[i].exit_score);
        glColor4f(0,
                  0,
                  1,
                  0.5+0.01*(rand()%50+1));
        GlTxt(1.1*grid,//32,
              grid,//16,
              fonts[5],txtColorWhite,txt_tmp, t_txt);
        glBindTexture(GL_TEXTURE_2D,t_txt);
        DrawTXT (lev[i].x*grid+x_cam,
                 (lev[i].y-1)*grid+y_cam,
                 grid,
                 grid,
                 0,1); 
       }//конец условия

      if (lev[i].unit==3 and//количество убийств на выход
          lev[i].exit_kills>0) 
       {
        sprintf(txt_tmp,"%i",lev[i].exit_kills);
        glColor4f(1,
                  0,
                  0,
                  0.5+0.01*(rand()%50+1));
        GlTxt(1.1*grid,//32,
              grid,//16,
              fonts[5],txtColorWhite,txt_tmp, t_txt);
        glBindTexture(GL_TEXTURE_2D,t_txt);
        DrawTXT (lev[i].x*grid+x_cam,
                 (lev[i].y-1)*grid+y_cam,
                 grid,
                 grid,
                 0,1); 
       }//конец условия
      glColor4f(1,1,1,1);
     }//конец условия
   }//конец цикла i

 }
