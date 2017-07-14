//http://swibplex.sourceforge.com

//============
//=[РЕДАКТОР]=-процедуры
//============
void Edit_Init()//клавиши управления редактором
 {
  Reset_Game();

  score_for_exit=0;
  edit_mode=1;
  num_texture=1;

  x_cursor=1;
  y_cursor=1;
  
  dark_volume=1;
 }
 
void Edit_Controls()//клавиши управления редактором
 {
//управление курсором
  if (event.key.keysym.sym==SDLK_UP) {y_cursor=y_cursor-1;} 
  if (event.key.keysym.sym==SDLK_DOWN) {y_cursor=y_cursor+1;} 
  if (event.key.keysym.sym==SDLK_LEFT) {x_cursor=x_cursor-1;} 
  if (event.key.keysym.sym==SDLK_RIGHT) {x_cursor=x_cursor+1;}

//ограничение передвижения курсора по уровню
  if (x_cursor>=grid_X_MAX) {x_cursor=grid_X_MAX;} 
  if (x_cursor<=0) {x_cursor=0;}  
  if (y_cursor>=grid_Y_MAX) {y_cursor=grid_Y_MAX;} 
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
      for(int i=0; i<lev_max;i++) 
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
      for(int j=0; j<obj_max; j++) 
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
  if (event.key.keysym.sym == SDLK_DELETE)
   { 
    if (edit_mode==1)//статичные lev
     {     
      for(int i=0; i<lev_max; i++) 
       {
        if (lev[i].unit>0 and lev[i].x==x_cursor and lev[i].y==y_cursor and
            lev_matrix[x_cursor][y_cursor]==lev[i].unit) 
         {
          Kill_OneLev(i);        
          break;     
         }
       }
     }

    if (edit_mode==2)//динамические obj
     {     
      for(int j=0; j<obj_max; j++) 
       {
        if (obj[j].unit>0 and obj[j].x==x_cursor and obj[j].y==y_cursor and
            obj_matrix[x_cursor][y_cursor]==obj[j].unit) 
         {
          Kill_OneObj(j);
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
    Reset_Game();
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

  DrawRubberPoly(x_cursor*grid+x_cam,
                 y_cursor*grid+y_cam,
                 grid,
                 grid,
                 0,
                 1);
  
  glBindTexture(GL_TEXTURE_2D, cursor_TX);
  DrawRubberPoly(x_cursor*grid+x_cam,
                 y_cursor*grid+y_cam,
                 grid+5*sin(delta*3.142/180),
                 grid+5*sin(delta*3.142/180),
                 delta,
                 1);
 }

void Other_Draw()//вывод прочего 
 {
  glColor4f(1,1,1,1);	 
  //ХУД
  glBindTexture(GL_TEXTURE_2D,other_texture[3]);
  DrawRubberPoly(0,
                 0,
                 win_DX/2,//512
                 win_DY/12,//64
                 0,
                 0); 

  //Выводим номер уровня,выводим режим редактора,выводим номер элемента
  DrawText(win_DX/22.756,//45,
	       win_DY/76.8,//10
	       win_DX/2,//512
	       win_DY/48,//16,
           1,//font_index
           "c/v:lev="+IntToStr(level_num)+", f1/f2:mode="+IntToStr(edit_mode)+", q/e:elm="+IntToStr(num_texture),
           0,//delta
	       0);//center  

  //Выводим количество очков на выход,выводим координаты курсора
  DrawText(win_DX/22.756,//45
	       win_DY/30.72,//25,
	       win_DX/2,//512,
	       win_DY/48,//16,
           1,//font_index
           "z/x:e-scr="+IntToStr(score_for_exit)+", x="+IntToStr(x_cursor)+", y="+IntToStr(y_cursor),
           0,//delta
	       0);//center

  //Выводим остальные клавиши
  DrawText(win_DX/22.756,//45
           win_DY/19.2,//40
           win_DX/2,//512
           win_DY/48,//16,
           1,//font_index
           "w,a,s,d,spc,del,f5/f7",
           0,//delta
	       0);//center 
 }
