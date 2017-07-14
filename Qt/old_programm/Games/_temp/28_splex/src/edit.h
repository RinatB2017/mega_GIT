//Разработано aureli0,NeonLight_Child,Ph0Eni}{,Zinoviev54,imanavhod-это все я:)
//http://sourceforge.net/projects/swibplex/
//последний билд от 23.12.2009

//============
//=[РЕДАКТОР]=-процедуры
//============
void Edit_Init()//клавиши управления редактором
 {
  Init_Game();

  score_for_exit=0;
  edit_mode=1;
  num_texture=1;

  x_cursor=1;
  y_cursor=1;
 }
 
void Edit_Controls()//клавиши управления редактором
 {
//управление курсором
  if (event.key.keysym.sym==SDLK_UP) {y_cursor=y_cursor-1;} 
  if (event.key.keysym.sym==SDLK_DOWN) {y_cursor=y_cursor+1;} 
  if (event.key.keysym.sym==SDLK_LEFT) {x_cursor=x_cursor-1;} 
  if (event.key.keysym.sym==SDLK_RIGHT) {x_cursor=x_cursor+1;}

//ограничение передвижения курсора по уровню
  if (x_cursor>=lev_width) {x_cursor=lev_width;} 
  if (x_cursor<=0) {x_cursor=0;}  
  if (y_cursor>=lev_height) {y_cursor=lev_height;} 
  if (y_cursor<=0) {y_cursor=0;}    

//переключение элементов уровня 
  if (event.key.keysym.sym==SDLK_q) {num_texture=num_texture+1;}
  if (event.key.keysym.sym==SDLK_e) {num_texture=num_texture-1;}

//прокручивание очков выхода 
  if (event.key.keysym.sym==SDLK_z) {score_for_exit=score_for_exit+1;}
  if (event.key.keysym.sym==SDLK_x) {score_for_exit=score_for_exit-1;}

//переключение номера уровня 
  if (event.key.keysym.sym==SDLK_c) {level_num=level_num+1;}
  if (event.key.keysym.sym==SDLK_v) {level_num=level_num-1;}

//происходит переключение режимов редактора
  if (event.key.keysym.sym==SDLK_F1) {edit_mode=1;}//статичные lev
  if (event.key.keysym.sym==SDLK_F2) {edit_mode=2;}//динамические obj

//устанавливаем элемент
  if (event.key.keysym.sym==SDLK_SPACE) 
   {
    if (edit_mode==1)//статичные lev
     {  
      for(i=0;i<lev_num_max;i++) 
       {
        if (lev[i].unit==0 and 
            lev_matrix[x_cursor][y_cursor]==0) 
         {
          lev[i].x=x_cursor;
          lev[i].y=y_cursor;
          lev[i].unit=num_texture;

          if (lev[i].unit==2) {lev[i].exit_score=score_for_exit;}//выход
          if (lev[i].unit==3) {lev[i].exit_kills=score_for_exit;}//выход

          break;
         }
       }
     }
    if (edit_mode==2)//динамические obj
     {  
      for(j=0; j < obj_num_max; j++) 
       {
        if (obj[j].unit==0 and 
            obj_matrix[x_cursor][y_cursor]==0) 
         {
          obj[j].x=x_cursor;
          obj[j].y=y_cursor;
          obj[j].unit=num_texture;
          break;
         }
       }
     }
   }//конец "устанавливаем элемент"

//стираем элемент
  if ( event.key.keysym.sym == SDLK_DELETE )
   { 
    if (edit_mode==1)//статичные lev
     {     
      for(i=0; i < lev_num_max; i++) 
       {
        if (lev[i].unit>0 and lev[i].x==x_cursor and lev[i].y==y_cursor and
            lev_matrix[x_cursor][y_cursor]==lev[i].unit) 
         {
          kill_lev(i);        
          break;     
         }
       }
     }

    if (edit_mode==2)//динамические obj
     {     
      for(j=0; j < obj_num_max; j++) 
       {
        if (obj[j].unit>0 and obj[j].x==x_cursor and obj[j].y==y_cursor and
            obj_matrix[x_cursor][y_cursor]==obj[j].unit) 
         {
          kill_obj(j);
          break;
         }
       }
     }
   }//конец "стираем элемент"
 
//сохранение уровня
  if ( event.key.keysym.sym == SDLK_F5 )
   { 
    Save_Level(level_num);
   } 

//загрузка уровня
  if ( event.key.keysym.sym == SDLK_F7 )
   { 
    Init_Game();
    Load_Level(level_num);
   }

  if (keys[SDLK_F3]) //отразить уровень по вертикали
   {
    mirror=true;
    Mirror();
   }//конец условия

 }

void Cursor_Draw()//Рисуем курсор редактора 
 {
  delta=delta+game_speed*5;
  if (delta>360) {delta=0;}  

  if (edit_mode==1) {glBindTexture( GL_TEXTURE_2D, lev_texture[num_texture] );}
  if (edit_mode==2) {glBindTexture( GL_TEXTURE_2D, obj_texture[num_texture] );}

  DrawTXT (x_cursor*grid+x_cam,
           y_cursor*grid+y_cam,
           grid,grid,0,1);
  
  glBindTexture( GL_TEXTURE_2D, cursor_texture);
  DrawTXT (x_cursor*grid+x_cam,
           y_cursor*grid+y_cam,
           grid+5*sin(delta*3.142/180),
           grid+5*sin(delta*3.142/180),delta,1);  
 }

void Other_Draw()//вывод прочего 
 {
  //ХУД
  glBindTexture(GL_TEXTURE_2D,other_texture[3]);
  DrawTXT(0,
          0,
          win_DX/2,//512
          win_DY/12,//64
          0,0); 

  //выводим номер уровня,выводим режим редактора,выводим номер элемента
  sprintf(txt_tmp,"c/v:lev=%i f1,f2:mode=%i q/e:elm=%i",level_num,edit_mode,num_texture);//номер уровня
  GlTxt(win_DX/2,//512
        win_DY/48,//16,
        fonts[2],txtColorWhite,txt_tmp, t_txt);
  glBindTexture( GL_TEXTURE_2D, t_txt);
  DrawTXT(win_DX/22.756,//45,
          win_DY/76.8,//10
          win_DX/2,//512
          win_DY/48,//16,
          0,0);

  //выводим количество очков на выход,выводим координаты курсора
  sprintf(txt_tmp,"z/x:e-scr=%i x=%i,y=%i",score_for_exit,x_cursor,y_cursor);
  GlTxt(win_DX/2,//512,
        win_DY/48,//16,
        fonts[2],txtColorWhite,txt_tmp, t_txt);
  glBindTexture( GL_TEXTURE_2D, t_txt);
  DrawTXT(win_DX/22.756,//45
          win_DY/30.72,//25,
          win_DX/2,//512,
          win_DY/48,//16,
          0,0);

  //выводим остальные клавиши
  GlTxt(win_DX/2,//512,
        win_DY/48,//16,
        fonts[2],txtColorWhite,"w,a,s,d,spc,del,f5/f7", t_txt);
  glBindTexture( GL_TEXTURE_2D, t_txt);
  DrawTXT(win_DX/22.756,//45
          win_DY/19.2,//40
          win_DX/2,//512
          win_DY/48,//16,
          0,0);
 }
