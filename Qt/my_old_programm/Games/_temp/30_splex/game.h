//http://sourceforge.net/projects/swibplex/

void Matrix_ReFresh()//Рефреш матриц
 {
  //обновляем матрицу уровня
  for(int i=0; i<lev_max; i++) 
   {
    lev_matrix[lev[i].x][lev[i].y]=lev[i].unit;
   }

  //обновляем матрицу объектов    
  for(int j=0; j<obj_max; j++) 
   { 
    obj_index[j]=j;
    if (obj_dead[j]==0) {obj_matrix[obj[j].x][obj[j].y]=obj[j].unit;}
    if (obj_dead[j]>0) {obj_matrix[obj[j].x][obj[j].y]=0;}
   }
 }

//Сброс всех переменных при игры
void Reset_Game() 
 {
  //Игровой таймер
  game_now=0;
  game_then=0;
  game_timer=0;
 
  Reset_Player();
  Reset_Lev();
  Reset_Obj();

  obj_nuke_index=0;//индекс взрывающегося объекта
  obj_kick_index=0;

  //Обнуление матриц
  for(int j=0; j<grid_X_MAX+1; j++)
  for(int k=0; k<grid_Y_MAX+1; k++)
   {
    obj_matrix[j][k]=0;
    lev_matrix[j][k]=0;
    obj_matrix_tmp[j][k]=0;
    lev_matrix_tmp[j][k]=0;
   }

  //Обнуление состояния дверей
  for(int i=0; i<lev_max; i++) 
   {
    lev_door_open[i]=false;//двери закрыты
    lev_button_push[i]=false;//кнопки не нажаты
    lev_light_push[i]=false;//светильники выключены 
   }

  //Остальное
  x_cam=0;
  y_cam=0;
  delta=0;//для телепортов и курсоров
 }

void Init_Lev_Doors()//инициализация состояния дверей
 {
  for(int j=0; j<lev_max; j++) 
   {
    if (lev[j].unit==index_door_open) {lev_door_open[j]=true;}//дверь открыта
    if (lev[j].unit==index_door_close) {lev_door_open[j]=false;}//дверь закрыта
   }
 }

void Init_Lev_Lights()//инициализация состояния светильников и освещения в целом
 {
  for(int j=0; j<lev_max; j++) 
   {
    if (lev[j].unit==index_lamp_on) {lev_light_push[j]=true;}//лампа включена
    if (lev[j].unit==index_lamp_off) {lev_light_push[j]=false;}//лампа выключена
   }
  
  if (diff_game_mode==index_game_mode_easy) {dark_volume=1;}
  if (diff_game_mode==index_game_mode_hard) {dark_volume=0.01;}
 }
 
void Level_Control_Doors()//текстура lev для разного состояния дверей
 {
  for(int j=0; j<lev_max; j++) 
   {
    if (lev[j].unit==index_door_open and//дверь открыта
        lev_matrix[lev[j].x][lev[j].y]==lev[j].unit or
        lev[j].unit==index_door_close and//дверь открыта
        lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
     {
      if (lev_door_open[j]==true) {lev[j].unit=index_door_open;} //дверь открыта
      if (lev_door_open[j]==false) {lev[j].unit=index_door_close;} //дверь закрыта
     }
   }
 }

void Level_Control_Lights(int j)//текстура lev для разного состояния дверей
 {
  if (lev[j].unit==index_lamp_on and//лампочка вкл
      lev_matrix[lev[j].x][lev[j].y]==lev[j].unit or
      lev[j].unit==index_lamp_off and//лампочка выкл
      lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
   {
    if (lev_light_push[j]==true)
     {
      lev[j].unit=index_lamp_on;//лампочка вкл
	  
      if (diff_game_mode==index_game_mode_hard)//при сложном режиме сложности
       {
	    //Находим лампу и включаем ее
	    for (int k=1; k<lmap_max; k++) 
         {
	      if (lmap_matrix[lev[j].x][lev[j].y]==0 and
	          lmap[k].undead==0)
	       {	 
		    Summon_LmapOne(k,lev[j].x,lev[j].y);
	        break;
		   }
		 }     
	   }	
	 }
	 
    if (lev_light_push[j]==false) 
     {
	  lev[j].unit=index_lamp_off;
	  
	  if (diff_game_mode==index_game_mode_hard)//при сложном режиме сложности
       {
	    //Находим лампу и гасим ее
	    for (int k=1; k<lmap_max; k++) 
         {
	      if (lmap_matrix[lev[j].x][lev[j].y]==k)
	       {	 
		    Kill_LmapOne(k);
	        break;
		   }   
	     }
       }
	 } //лампочка выкл
   }
 }

void Render_Lmap()
 {
  if (diff_game_mode==index_game_mode_hard)//при сложном режиме сложности
   {	 
    for(int i=0; i<lev_max; i++) 
     {
      if (lev[i].unit==index_start_pos)//1-ый элемент-старт игрока
       {
        Summon_LmapOne(0,lev[i].x,lev[i].y);
       }
     }//конец цикла
	
    for (int i=0; i<lev_max; i++) 
    for (int j=1; j<lmap_max; j++) 
     {
      if (lev[i].unit==index_lamp_on and //лампочка вкл //31
          lmap[j].undead==0)
       {
	    Summon_LmapOne(j,lev[i].x,lev[i].y);
	    //cout << j << ", " << lmap[j].x << ", " << lmap[j].y << endl;
	    break;
	   }     
	 }  
   }	 

 }

//Рисуем кал
void Draw_LevelKal()
 {
  for(int i=0; i<lev_max; i++) 
   { 
    //Рисуем уровень
    if (game_show==1 and //в игре
        //diff_game_mode==index_game_mode_hard and //при сложном режиме сложности
        p_kal[i]==true and
        lev[i].unit==index_kal and//0
        lev[i].x*grid+x_cam>0-grid and
        lev[i].x*grid+x_cam<win_DX+grid and 
        lev[i].y*grid+y_cam>0-grid and
        lev[i].y*grid+y_cam<win_DY+grid)
     { 
      glBindTexture(GL_TEXTURE_2D,other_texture[1]); 
      glColor4f(dark_volume,dark_volume,dark_volume,1);//0.1
      DrawRubberPoly(lev[i].x*grid+x_cam,
                     lev[i].y*grid+y_cam,
                     grid,
                     grid,
                     0,
                     1);
      if (diff_game_mode==index_game_mode_hard)//при сложном режиме сложности
       {                     		 
        for(int k=0; k<=lmap_max; k++)
        for(int j=lmap[k].light_radius; j>=0; j--)                     		 
         {
          if (lev[i].x==lmap[k].x-j and //и попадает в радиус освещения
              lev[i].y>=lmap[k].y-j and
              lev[i].y<=lmap[k].y+j or     
          
              lev[i].x==lmap[k].x+j and //и попадает в радиус освещения
              lev[i].y>=lmap[k].y-j and
              lev[i].y<=lmap[k].y+j or
          
              lev[i].x>=lmap[k].x-j and //и попадает в радиус освещения
              lev[i].x<=lmap[k].x+j and 
              lev[i].y==lmap[k].y-j or   
              
              lev[i].x>=lmap[k].x-j and //и попадает в радиус освещения
              lev[i].x<=lmap[k].x+j and 
              lev[i].y==lmap[k].y+j)
           {       		
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);     
            glColor4f((float) 0.01*lmap[k].R*(lmap[k].light_radius-j)/lmap[k].light_radius,//0.01*level[i].R,
                      (float) 0.01*lmap[k].G*(lmap[k].light_radius-j)/lmap[k].light_radius,//0.01*level[i].G,
                      (float) 0.01*lmap[k].B*(lmap[k].light_radius-j)/lmap[k].light_radius,//0.01*level[i].B,
                      (float) 0.01*lmap[k].force*(lmap[k].light_radius-j)/lmap[k].light_radius);//0.01*level[i].A);          
                                 
            DrawRubberPoly(lev[i].x*grid+x_cam,
                           lev[i].y*grid+y_cam,
                           grid,
                           grid,
                           0,
                           1);  
         
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1,1,1,1);  
           } 
         }
       }
     }
   }
 }  
  
//Рисуем уровень   
void Draw_Level()
 {
  for(int i=0; i<lev_max; i++) 
   { 
    //Рисуем уровень
    if (game_show==1 and //в игре
        lev[i].unit>index_kal and
        lev[i].x*grid+x_cam>0-grid and
        lev[i].x*grid+x_cam<win_DX+grid and 
        lev[i].y*grid+y_cam>0-grid and
        lev[i].y*grid+y_cam<win_DY+grid or
        edit_show==1 and //в игре
        lev[i].unit>index_kal and
        lev[i].x*grid+x_cam>0-grid and
        lev[i].x*grid+x_cam<win_DX+grid and 
        lev[i].y*grid+y_cam>0-grid and
        lev[i].y*grid+y_cam<win_DY+grid)
     {
      //Соответствующему элементу уровня свои текстуры
      glBindTexture(GL_TEXTURE_2D,lev_texture[lev[i].unit]);
      if (lev[i].unit==index_exit_score and p_score>=lev[i].exit_score or//если можно войти в выход
          lev[i].unit==index_exit_kills and lev[i].exit_kills==0) 
       {glBindTexture(GL_TEXTURE_2D,other_texture[4]);}//дверь с уровня открыта
      if (lev[i].unit==index_button_push and 
          lev_button_push[i]==true) 
       {glBindTexture(GL_TEXTURE_2D,other_texture[5]);}//кнопка нажата

      glColor4f(dark_volume,dark_volume,dark_volume,1);
      DrawRubberPoly(lev[i].x*grid+x_cam,
                     lev[i].y*grid+y_cam,
                     grid,
                     grid,
                     0,
                     1); 
      
      if (diff_game_mode==index_game_mode_hard)//при сложном режиме сложности
       {
        for(int k=0; k<=lmap_max; k++)
        for(int j=lmap[k].light_radius; j>=0; j--)
         {
          if (lev[i].x==lmap[k].x-j and //и попадает в радиус освещения
              lev[i].y>=lmap[k].y-j and
              lev[i].y<=lmap[k].y+j or     
          
              lev[i].x==lmap[k].x+j and //и попадает в радиус освещения
              lev[i].y>=lmap[k].y-j and
              lev[i].y<=lmap[k].y+j or
          
              lev[i].x>=lmap[k].x-j and //и попадает в радиус освещения
              lev[i].x<=lmap[k].x+j and 
              lev[i].y==lmap[k].y-j or   
              
              lev[i].x>=lmap[k].x-j and //и попадает в радиус освещения
              lev[i].x<=lmap[k].x+j and 
              lev[i].y==lmap[k].y+j)
           {             
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);     
            glColor4f((float) 0.01*lmap[k].R*(lmap[k].light_radius-j)/lmap[k].light_radius,//0.01*level[i].R,
                      (float) 0.01*lmap[k].G*(lmap[k].light_radius-j)/lmap[k].light_radius,//0.01*level[i].G,
                      (float) 0.01*lmap[k].B*(lmap[k].light_radius-j)/lmap[k].light_radius,//0.01*level[i].B,
                      (float) 0.01*lmap[k].force*(lmap[k].light_radius-j)/lmap[k].light_radius);//0.01*level[i].A);
             
            DrawRubberPoly(lev[i].x*grid+x_cam,
                           lev[i].y*grid+y_cam,
                           grid,
                           grid,
                           0,
                           1);  
         
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1,1,1,1);  
           }         
		 }
	   }  
              
      //Если дверь на выход, то на ней выводим количество очков для выхода из уровн
      if (lev[i].unit==index_exit_score and //количество очков на выход
          lev[i].exit_score>0 and
          p_score<lev[i].exit_score) 
       {
        glColor4f(0,
                  0,
                  1,
                  0.5+0.01*random(50));
                  
        DrawText((lev[i].x)*grid+x_cam,
	             (lev[i].y-1)*grid+y_cam,
	             grid,//dX
	             grid,//dY
                 1,//font_index
                 IntToStr(lev[i].exit_score),
                 0,//delta
	             1);//center        
       }//конец условия

      if (lev[i].unit==index_exit_kills and//количество убийств на выход
          lev[i].exit_kills>0) 
       {                      
        glColor4f(0,
                  0,
                  1,
                  0.5+0.01*random(50));
                
        DrawText((lev[i].x)*grid+x_cam,
	             (lev[i].y-1)*grid+y_cam,
	             grid,//dX
	             grid,//dY
                 1,//font_index
                 IntToStr(lev[i].exit_kills),
                 0,//delta
	             1);//center                            
       }//конец условия
     }
   }
 }

void Render_ObjColExit()//если враг заходит на спец выход то дохнет, а очки на выход уменьшаються
 {
  for(int i=0; i<lev_max; i++) 
  for(int j=0; j<obj_max; j++)
   {
    if (lev[i].unit==3 and //если враг залетает на 3-ий элемент lev (условие как с телепортом)
        lev[i].exit_kills>0 and
        obj_dead[j]==0 and //если объект жив
        obj[j].unit>=index_enemy1 and //враг   
        obj[j].x==lev[i].x and
        obj[j].y==lev[i].y and
        obj_matrix[obj[j].x][obj[j].y]==obj[j].unit)    
     {
      lev[i].exit_kills=lev[i].exit_kills-1;//уменьшаем очки выхода

      obj_matrix[lev[i].x][lev[i].y]=0;//стираем объект из матрицы объектов
      obj_matrix[obj[j].x][obj[j].y]=0; 

      //если объект двигался
      if (obj_move[j]==true)
       {
        if (obj_move_up[j]==true) {obj_matrix[obj[j].x][obj[j].y-1]=0;}
        if (obj_move_down[j]==true) {obj_matrix[obj[j].x][obj[j].y+1]=0;}
        if (obj_move_left[j]==true) {obj_matrix[obj[j].x][obj[j].y-1]=0;}
        if (obj_move_right[j]==true) {obj_matrix[obj[j].x][obj[j].y+1]=0;}
       }
      obj_dead[j]=index_dead_blood;//объект мертв-там будет кровища
     }//конец условия
   }//конец циклов
 }

void Render_ObjColTeleport()//Взаимодействие объекта с телепортом-попадание в телепорт объекта
 {
  if (t1_true==true and//Если существуют вход и выход из телепорта
      t2_true==true)
   {
    for (int j=0; j<obj_max; j++) 
     {
      if (obj_dead[j]==0 and //если объект жив
          obj_move[j]==false and //если в этот не двигается-чтобы не было движения сразу после телепортации
          obj[j].unit>0 and 
          obj[j].x==t1x and 
          obj[j].y==t1y and 
          obj_matrix[obj[j].x][obj[j].y]>0 and

          obj_matrix[t2x][t2y]==0)//если в месте выхода никого нету-выход чист
       {
        obj_matrix[obj[j].x][obj[j].y]=0;//затираем следы
        obj[j].x=t2x;//телепортация объекта в координаты выхода из телепорта
        obj[j].y=t2y;
        obj_matrix[obj[j].x][obj[j].y]=obj[j].unit;

       } 
     }
   }//конец "Если существуют вход и выход из телепорта"
 }

void Render_ObjCheckMove()//Проверка возможности перемещения объекта
 {
  //Взаимодействие объектов между собой и уровнем
  for(int j=0; j<obj_max; j++)
   {
    if (obj[j].unit>0 and
        obj_dead[j]==0 and //если объект жив
        obj_nuke[j]==false and //если не взрывается
        obj_move[j]==false and //когда движение не происходит
        obj_matrix[obj[j].x][obj[j].y]==obj[j].unit)
     { 
      obj_move_left_v[j]=true;//возможны все направления движения
      obj_move_right_v[j]=true;
      obj_move_up_v[j]=true;
      obj_move_down_v[j]=true;  
     
      obj_move_up_left_v[j]=true;
      obj_move_up_right_v[j]=true;
      obj_move_down_left_v[j]=true;
      obj_move_down_right_v[j]=true;
     }
   }//конец цикла

  for(int j=0; j<obj_max; j++)
   {
    //если объект был в движении то отсчитываем счетчик до его состояния покоя
    if (obj[j].unit>0 and obj[j].unit<=6 and //для всех объектов кроме врагов
        obj_dead[j]==0 and //если объект жив
        obj_move[j]==false and //и больше не двигается
        obj_sleep_count[j]>0) 
     {obj_sleep_count[j]=obj_sleep_count[j]-1;}//уменьшаем таймер до состояния покоя
   }//конец цикла
 
  //проверка объекта-претендента на падение или перемещение на свободную клетку 
  for(int j=0; j<obj_max; j++)
  for(int k=0; k<obj_max; k++)
   {
    //взаимодействие объектов друг с другом-предотвращение слипания объектов при падении
    if (obj[j].unit>0 and obj[j].unit<=6 and//для всех объектов кроме врагов
        obj[k].unit>0 and obj[k].unit<=6 and
        obj_dead[j]==0 and //если объект j жив
        obj_dead[k]==0 and //если объект k жив
        obj_nuke[j]==false and //если не взрывается
        obj_nuke[k]==false) //если не взрывается
     { 
      //1-случай 
      // 1j0k1
      // 11011 
      //чтобы в пропасть падали по 1-правило будет действовать только при дырке в матрице уровня и объектов
      if (obj_move[j]==false and
          obj_move[k]==false and
          obj_move_right_v[j]==true and //если 2 объекта j и k одновременно готовы двигаться к яме между ними
          obj_move_left_v[k]==true and
          obj[j].y==obj[k].y and
          obj[j].x==obj[k].x-2 and 
          obj_matrix[obj[k].x-1][obj[k].y+1]==0 and //если есть куда падать в последствии через ход
          lev_matrix[obj[k].x-1][obj[k].y+1]<=4 and //по матрицам obj и lev  
          obj_matrix[obj[j].x+1][obj[j].y+1]==0 and //если есть куда падать в последствии через ход
          lev_matrix[obj[j].x+1][obj[j].y+1]<=4)//по матрицам obj и lev  
       {
        obj_move_left_v[k]=false;//падает j-ый объект 
       }
      
      //2-случай
      // 1jk11
      // 11011
      //если пропасть занята летающим вниз объектом k,j-может упасть вправо 
      if (obj_move[j]==false and
          obj_move_right_v[j]==true and
          obj[j].y==obj[k].y+1 and
          obj[j].x==obj[k].x-1)
        {
         obj_move_right_v[j]=false;
        }

      //3-случай
      // 11kj1
      // 11011
      //если пропасть занята летающим вниз объектом k,j-может упасть влево 
      if (obj_move[j]==false and
          obj_move_left_v[j]==true and
          obj[j].y==obj[k].y+1 and
          obj[j].x==obj[k].x+1)
        {
         obj_move_left_v[j]=false;
        }

     }//конец условия

    //взаимодействие объектов друг с другом-бомба взрывает врага когда на него падает и сама взрывается
    if (obj[j].unit==index_bomb and//для всех объектов кроме врагов
        obj[k].unit>=index_enemy1 and
        obj_dead[j]==0 and //если объект j жив
        obj_dead[k]==0 and //если объект k жив
        obj_nuke[j]==false and //если не взрывается
        obj_nuke[k]==false and //если не взрывается
        obj[k].x*grid+obj_move_x[k]-0.5*grid<obj[j].x*grid+obj_move_x[j]+0.5*grid and
        obj[k].x*grid+obj_move_x[k]+0.5*grid>obj[j].x*grid+obj_move_x[j]-0.5*grid and
        obj[k].y*grid+obj_move_y[k]-0.5*grid<obj[j].y*grid+obj_move_y[j]+0.5*grid and
        obj[k].y*grid+obj_move_y[k]+0.5*grid>obj[j].y*grid+obj_move_y[j]-0.5*grid)
     {
      obj_move[j]==false;
      obj_move[k]==false;
      obj_nuke[j]=true;
      obj_nuke_count[j]=0;    
      obj_dead[k]=index_dead_blood;//кровища 
     }
   }//конец циклов
 }

void Render_ObjColPlayer()//Взаимодействие объектов типа obj с колобком
 {
  if (p_true==true)//Если колобок существует
   {
    //Тычки объектов колобком 
    p_kick_left=false;
    p_kick_right=false;
 
    for(int j=0; j<obj_max; j++) 
     {
      if (obj_dead[j]==0)//если объект жив
       { 
        if (obj_move_kick_left_mark[j]==true) {obj_move_kick_left_mark[j]=false;}
        if (obj_move_kick_right_mark[j]==true) {obj_move_kick_right_mark[j]=false;} 
       } 
     }

    //ищем объекты для тычка-бомбы
    for(int j=0; j<obj_max; j++) 
     {
      //слева
      if (obj_dead[j]==0 and //если объект жив
          obj_move[j]==false and
          obj[j].unit==index_bomb and 
          obj[j].x==p_x-1 and 
          obj[j].y==p_y and 
          obj_matrix[obj[j].x][obj[j].y]==obj[j].unit and
          obj_sleep_count[j]==0)//если объект в состоянии покоя
       {
        obj_move_kick_left_mark[j]=true;//ставим метку что можем ткнуть объект
       
        if (obj_move_left_v[j]==true)//если объект может двигаться влево 
         {  
          p_kick_left=true;//и позволяем колобку ткнуть объект влево
          p_move_left_v=false;//вовремя тычка сидим на месте
         }
          
        //если объект не может дальше двигатся то и колобок тоже
        if (obj_move_left_v[j]==false) 
         {
          p_move_left_v=false;
          p_kick_left=false;
         }
       }//конец слева

      //справа-аналогично
      if (obj_dead[j]==0 and //если объект жив
          obj_move[j]==false and
          obj[j].unit==index_bomb and
          obj[j].x==p_x+1 and 
          obj[j].y==p_y and 
          obj_matrix[obj[j].x][obj[j].y]==obj[j].unit and
          obj_sleep_count[j]==0)//если объект в состоянии покоя
       {
        obj_move_kick_right_mark[j]=true;//ставим метку

        if (obj_move_right_v[j]==true) 
         {  
          p_kick_right=true;
          p_move_right_v=false;
         }

        if (obj_move_right_v[j]==false) 
         {
          p_move_right_v=false;
          p_kick_right=false;
         }
       }//конец справа
     }//конец цикла про "ищем объекты для тычка-бомбы"

    for(int j=0; j<obj_max; j++)  
     {
      //1. объект на колобке не проваливается
      if (obj_dead[j]==0 and //если объект жив
          obj_move[j]==false and
          obj_move_down_v[j]==true and
          obj_sleep_count[j]==0 and
          obj[j].unit>0 and //бонусы или бомбы
          obj[j].unit<=index_bomb and //**
          obj[j].y==p_y-1 and
          p_x*grid+p_move_x-0.5*grid<obj[j].x*grid+obj_move_x[j]+0.5*grid and
          p_x*grid+p_move_x+0.5*grid>obj[j].x*grid+obj_move_x[j]-0.5*grid)
       {
        obj_move_down_v[j]=false;
       }
 
      //2. объект не катится влево на колобка если под колобком яма 
      if (obj_dead[j]==0 and //если объект жив
          obj_move[j]==false and
          obj_move_left_v[j]==true and
          obj[j].unit>0 and //бонусы или бомбы
          obj[j].unit<=index_bomb and //**
          obj[j].x==p_x+1 and
          obj[j].y==p_y and
          obj_matrix[p_x+1][p_y]>0)//в этой клетке действительно есть объект
       {
        obj_move_left_v[j]=false;
       }

      //3. объект не катится вправо на колобка если под колобком яма 
      if (obj_dead[j]==0 and //если объект жив
          obj_move[j]==false and
          obj_move_right_v[j]==true and
          obj[j].unit>0 and //бонусы или бомбы
          obj[j].unit<=index_bomb and //**
          obj[j].x==p_x-1 and
          obj[j].y==p_y and
          obj_matrix[p_x-1][p_y]>0)//в этой клетке действительно есть объект
       {
        obj_move_right_v[j]=false;
       }

      //4. объект не катится влево на колобка если он в яме 
      if (obj_dead[j]==0 and //если объект жив
          obj_move[j]==false and
          obj_move_left_v[j]==true and
          obj_move_down_left_v[j]==true and
          obj[j].unit>0 and //бонусы или бомбы
          obj[j].unit<=index_bomb and //**
          obj[j].x==p_x+1 and
          obj[j].y==p_y-1 and
          obj_matrix[p_x+1][p_y-1]>0)//в этой клетке действительно есть объект
       {
        obj_move_left_v[j]=false;
        obj_move_down_left_v[j]=false;
       }

      //5. объект не катится вправо на колобка если он в яме
      if (obj_dead[j]==0 and //если объект жив
          obj_move[j]==false and
          obj_move_right_v[j]==true and
          obj_move_down_right_v[j]==true and
          obj[j].unit>0 and //бонусы или бомбы
          obj[j].unit<=index_bomb and //**
          obj[j].x==p_x-1 and
          obj[j].y==p_y-1 and
          obj_matrix[p_x-1][p_y-1]>0)//в этой клетке действительно есть объект
       {
        obj_move_right_v[j]=false;
        obj_move_down_right_v[j]=false;
       }

     }//конец цикла
   }//конец условия "Если колобок существует"
 }

void Render_ObjFilterMoveV(int j)//Фильтрация возможных направлений передвижения объекта по матрицам lev и obj 
 {
  //слева
  if (lev_matrix[obj[j].x-1][obj[j].y]>index_door_open or //по стартовым и выходам ходить может
      obj_matrix[obj[j].x-1][obj[j].y]>0) //по объектам - нет 
     {obj_move_left_v[j]=false;}
  //справа
  if (lev_matrix[obj[j].x+1][obj[j].y]>index_door_open or
      obj_matrix[obj[j].x+1][obj[j].y]>0) {obj_move_right_v[j]=false;}
  //сверху
  if (lev_matrix[obj[j].x][obj[j].y-1]>index_door_open or
      obj_matrix[obj[j].x][obj[j].y-1]>0) {obj_move_up_v[j]=false;}
  //снизу
  if (lev_matrix[obj[j].x][obj[j].y+1]>index_door_open or
      obj_matrix[obj[j].x][obj[j].y+1]>0) {obj_move_down_v[j]=false;}

  //сверху слева
  if (lev_matrix[obj[j].x-1][obj[j].y-1]>index_door_open or
      obj_matrix[obj[j].x-1][obj[j].y-1]>0) {obj_move_up_left_v[j]=false;}
  //сверху справа
  if (lev_matrix[obj[j].x+1][obj[j].y-1]>index_door_open or
      obj_matrix[obj[j].x+1][obj[j].y-1]>0) {obj_move_up_right_v[j]=false;}
  //снизу слева
  if (lev_matrix[obj[j].x-1][obj[j].y+1]>index_door_open or
      obj_matrix[obj[j].x-1][obj[j].y+1]>0) {obj_move_down_left_v[j]=false;}
  //снизу справа
  if (lev_matrix[obj[j].x+1][obj[j].y+1]>index_door_open or
      obj_matrix[obj[j].x+1][obj[j].y+1]>0) {obj_move_down_right_v[j]=false;}

  //если объект бомба а под ней враг то падаем на врага
  if (obj[j].unit==index_bomb and
      obj_move[j]==false and
      obj_dead[j]==0 and
      obj_nuke[j]==false and
      obj_matrix[obj[j].x][obj[j].y+1]>=index_enemy1 and
      obj_sleep_count[j]>0) {obj_move_down_v[j]=true;}
 }

void Render_ObjMove()//Старт движения и псевдо АИ(поведение) объектов
 { 
  for(int j=0; j<obj_max; j++)
   {
    if (obj[j].unit>0) {Render_ObjFilterMoveV(j);}//ограничение по матрицам для всех объектов

    //Реализация тычка при движении объект влево
    if (p_kick_left==true and//если тычок задан
        obj[j].unit==index_bomb and//для бомб
        obj_move_kick_left_mark[j]==true and//если есть помеченый объект для тычка
        obj_dead[j]==0 and//если объект жив
        obj_move_left_v[j]==true and //если объект может двигаться влево
        keys[SDLK_LEFT])//и мы толкаем объект с клавиатуры
     {
      obj_kick_index=j;
      obj_move[j]=true;//объект толкнут
      obj_move_left[j]=true;
      obj_move_left_v[j]=false;
      //на время движения новая клетка заранее занимается
      obj_matrix[obj[j].x-1][obj[j].y]=obj_matrix[obj[j].x][obj[j].y]; 
      obj_rotate[j]=1;//меченый объект крутится при движени
      obj_move_kick_left_mark[j]=false;//снимаем метку
      p_kick_left=false;//тычок проделан   
     }

    //Реализация тычка при движении объект вправо
    if (p_kick_right==true and//если тычок задан
        obj[j].unit==index_bomb and//для бомб
        obj_move_kick_right_mark[j]==true and//если есть помеченый объект для тычка
        obj_dead[j]==0 and//если объект жив
        obj_move_right_v[j]==true and //если объект может двигаться вправо
        keys[SDLK_RIGHT])//и мы толкаем объект с клавиатуры
     {   
      obj_kick_index=j;
      obj_move[j]=true;//объект толкнут
      obj_move_right[j]=true;
      obj_move_right_v[j]=false;
      //на время движения новая клетка заранее занимается
      obj_matrix[obj[j].x+1][obj[j].y]=obj_matrix[obj[j].x][obj[j].y];
      obj_rotate[j]=1;//крутится при движении
      obj_move_kick_right_mark[j]=false;//снимаем метку
      p_kick_right=false;//тычок проделан
     }

    //Не скатываемость объекта
    if (j!=obj_kick_index and
        obj[j].unit>0 and obj[j].unit<=index_bomb and //для всех объектов кроме врагов
        obj_dead[j]==0 and //если объект жив
        obj_move[j]==false) //и больше не двигается
     {
      //Объект не скатывается если стоит
      if (lev_matrix[obj[j].x][obj[j].y+1]>=index_motherboard1 and//на плате
          lev_matrix[obj[j].x][obj[j].y+1]<=index_motherboard7 or 
          obj[j].x==p_x and //или на колобке
          obj[j].y==p_y-1)
       {
        obj_move_left_v[j]=false;
        obj_move_right_v[j]=false;
        obj_move_down_left_v[j]=false;
        obj_move_down_right_v[j]=false;
       }

      //Если есть движущийся объектты то те что между ними не могут двигаться-ваще это бред но работает
      for(int k=0; k<obj_max; k++)
       {
        if (obj[k].unit>0 and//для всех объектов
            obj_dead[k]==0 and //если объект жив
            obj_move[k]==true and //и двигается
            obj[k].x==obj[j].x and//другой объект под ним двигаться не может 
            obj[k].y==obj[j].y-1)
         {
          obj_move_left_v[j]=false;
          obj_move_right_v[j]=false;
          obj_move_down_left_v[j]=false;
          obj_move_down_right_v[j]=false;
         }         
       }
     }//конец условаия о нескатываемости

    //Для obj=1-6-бонусы и бомбы
    if (obj[j].unit<=index_bomb and 
        obj_dead[j]==0 and //если объект жив
        obj_nuke[j]==false and //если объект не на подрыве
        obj_matrix[obj[j].x][obj[j].y]<=obj[j].unit)//бонусы и бомбы
     {
      //1. падение вниз
      if (obj_move[j]==false and
          obj_move_down[j]==false and
          obj_move_down_v[j]==true)
       {
        obj_move[j]=true;
        obj_move_down[j]=true;
        obj_move_down_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x][obj[j].y+1]=obj_matrix[obj[j].x][obj[j].y];  
       }

      //2. влево, затем падение вниз
      if (obj_move[j]==false and
          obj_move_left[j]==false and
          obj_move_left_v[j]==true and
          obj_move_down_left_v[j]==true)
       {
        obj_move[j]=true;
        obj_move_left[j]=true;
        obj_rotate[j]=1;//крутится при движении
        obj_move_left_v[j]=false;
        obj_move_down_left_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x-1][obj[j].y]=obj_matrix[obj[j].x][obj[j].y];  
       }

      //3. вправо, затем падение вниз
      if (obj_move[j]==false and
          obj_move_right[j]==false and
          obj_move_right_v[j]==true and
          obj_move_down_right_v[j]==true)
       {
        obj_move[j]=true;
        obj_move_right[j]=true;
        obj_rotate[j]=1;//крутится при движении 
        obj_move_right_v[j]=false;
        obj_move_down_right_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x+1][obj[j].y]=obj_matrix[obj[j].x][obj[j].y];
       }
     }//конец условия 

    //для obj==7-враг ножницы
    if (obj[j].unit==index_enemy1 and
        obj_dead[j]==0 and //если объект жив
        obj_nuke[j]==false and //если объект не на подрыве
        obj_matrix[obj[j].x][obj[j].y]==obj[j].unit) //**
     {
      //определяем направление движения-псевдо АИ 
      if (obj_move[j]==false) 
       {
        obj_anim[j]=true;//анимированный объект
        obj_AI_param2[j]=obj_AI_param1[j];
        obj_AI_param1[j]=rand()%4+1;//определяем случайно куда двинутся-определяет заторможенность вражжины

        if (p_x>obj[j].x-obj_enemy1_radius and p_x<obj[j].x+obj_enemy1_radius and //если игрок поблиизости
            p_y>obj[j].y-obj_enemy1_radius and p_y<obj[j].y+obj_enemy1_radius)
         {
          if (p_y<obj[j].y and obj_move_up_v[j]==true) {obj_AI_param1[j]=1;}
          if (p_y>obj[j].y and obj_move_down_v[j]==true) {obj_AI_param1[j]=2;}
          if (p_x<obj[j].x and obj_move_left_v[j]==true) {obj_AI_param1[j]=3;}
          if (p_x>obj[j].x and obj_move_right_v[j]==true) {obj_AI_param1[j]=4;}
         }
         
        //чтобы не возвращались тут, преследование колобка
        if (obj_AI_param1[j]==1 and obj_AI_param2[j]==2) 
         {
          if (obj_move_left_v[j]==true) {obj_AI_param1[j]=3;}
          if (obj_move_right_v[j]==true) {obj_AI_param1[j]=4;}
          if (obj_move_left_v[j]==true and p_x<obj[j].x) {obj_AI_param1[j]=3;}
          if (obj_move_right_v[j]==true and p_x>obj[j].x) {obj_AI_param1[j]=4;}
         }
        if (obj_AI_param1[j]==2 and obj_AI_param2[j]==1)
         {
          if (obj_move_left_v[j]==true) {obj_AI_param1[j]=3;}
          if (obj_move_right_v[j]==true) {obj_AI_param1[j]=4;}
          if (obj_move_left_v[j]==true and p_x<obj[j].x) {obj_AI_param1[j]=3;}
          if (obj_move_right_v[j]==true and p_x>obj[j].x) {obj_AI_param1[j]=4;}
         }
        if (obj_AI_param1[j]==3 and obj_AI_param2[j]==4) 
         {
          if (obj_move_up_v[j]==true) {obj_AI_param1[j]=1;}
          if (obj_move_down_v[j]==true) {obj_AI_param1[j]=2;}
          if (obj_move_up_v[j]==true and p_y<obj[j].y) {obj_AI_param1[j]=1;}
          if (obj_move_down_v[j]==true and p_y>obj[j].y) {obj_AI_param1[j]=2;}
         }
        if (obj_AI_param1[j]==4 and obj_AI_param2[j]==3) 
         {
          if (obj_move_up_v[j]==true) {obj_AI_param1[j]=1;}
          if (obj_move_down_v[j]==true) {obj_AI_param1[j]=2;}
          if (obj_move_up_v[j]==true and p_y<obj[j].y) {obj_AI_param1[j]=1;}
          if (obj_move_down_v[j]==true and p_y>obj[j].y) {obj_AI_param1[j]=2;}
         }
       }

      //1. вверх
      if (obj_move[j]==false and
          obj_AI_param1[j]==1 and 
          obj_move_up[j]==false and
          obj_move_up_v[j]==true)
       {
        obj_move[j]=true;
        obj_move_up[j]=true;
        obj_move_up_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x][obj[j].y-1]=obj_matrix[obj[j].x][obj[j].y];  

        obj_angle_finish[j]=90;//финальный угол поворота
       } 
    
      //2. вниз
      if (obj_move[j]==false and
          obj_AI_param1[j]==2 and 
          obj_move_down[j]==false and
          obj_move_down_v[j]==true)
       {
        obj_move[j]=true;
        obj_move_down[j]=true;
        obj_move_down_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x][obj[j].y+1]=obj_matrix[obj[j].x][obj[j].y];

        obj_angle_finish[j]=270;//финальный угол поворота
       }          
       
      //3. влево
      if (obj_move[j]==false and
          obj_AI_param1[j]==3 and 
          obj_move_left[j]==false and
          obj_move_left_v[j]==true)
       {
        obj_move[j]=true;
        obj_move_left[j]=true;
        obj_move_left_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x-1][obj[j].y]=obj_matrix[obj[j].x][obj[j].y];
        obj_angle_finish[j]=180;//финальный угол поворота
       }         

      //4. вправо 
      if (obj_move[j]==false and
          obj_AI_param1[j]==4 and 
          obj_move_right[j]==false and
          obj_move_right_v[j]==true)
       {
        obj_move[j]=true;
        obj_move_right[j]=true;
        obj_move_right_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x+1][obj[j].y]=obj_matrix[obj[j].x][obj[j].y];

        obj_angle_finish[j]=0;//финальный угол поворота
       }        
 
     }//конец условия 

    //для obj==8-враг горизонтальная хрень
    if (obj[j].unit==index_enemy2 and
        obj_dead[j]==0 and //если объект жив
        obj_nuke[j]==false and //если объект не на подрыве
        obj_matrix[obj[j].x][obj[j].y]==obj[j].unit) //**
     {

      //определяем направление движения-псевдо АИ 
      if (obj_move[j]==false)
       {
        obj_move_up_v[j]=false;
        obj_move_down_v[j]=false;
        if (obj_move_left_v[j]==false) {obj_AI_param1[j]=1;}//определяем куда двинутся}
        if (obj_move_right_v[j]==false) {obj_AI_param1[j]=2;}//определяем куда двинутся}
       }
 
      //1. влево
      if (obj_move[j]==false and
          obj_AI_param1[j]==2 and 
          obj_move_left[j]==false and 
          obj_move_left_v[j]==true) 
       {
        obj_move[j]=true;
        obj_move_left[j]=true;
        obj_move_left_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x-1][obj[j].y]=obj_matrix[obj[j].x][obj[j].y];
        obj_rotate[j]=1;//крутится при движении
       }         

      //2. вправо 
      if (obj_move[j]==false and
          obj_AI_param1[j]==1 and 
          obj_move_right[j]==false and 
          obj_move_right_v[j]==true) 
       {
        obj_move[j]=true;
        obj_move_right[j]=true;
        obj_move_right_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x+1][obj[j].y]=obj_matrix[obj[j].x][obj[j].y];
        obj_rotate[j]=1;//крутится при движении
       }       

     }

    //для obj==9-враг вертикальная хрень
    if (obj[j].unit==index_enemy3 and
        obj_dead[j]==0 and //если объект жив
        obj_nuke[j]==false and //если объект не на подрыве
        obj_matrix[obj[j].x][obj[j].y]==obj[j].unit) //**
     {

      //определяем направление движения-псевдо АИ 
      if (obj_move[j]==false)
       {
        obj_move_left_v[j]=false;
        obj_move_right_v[j]=false;
        if (obj_move_up_v[j]==false) {obj_AI_param1[j]=1;}//определяем куда двинутся}
        if (obj_move_down_v[j]==false) {obj_AI_param1[j]=2;}//определяем куда двинутся}
        obj_rotate[j]=2;//все время крутится
       }
 
      //1. вверх
      if (obj_move[j]==false and
          obj_AI_param1[j]==2 and 
          obj_move_up[j]==false and
          obj_move_up_v[j]==true) 
       {
        obj_move[j]=true;
        obj_move_up[j]=true;        
        obj_move_up_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x][obj[j].y-1]=obj_matrix[obj[j].x][obj[j].y];//}
       }         

      //2. вниз 
      if (obj_move[j]==false and
          obj_AI_param1[j]==1 and 
          obj_move_down[j]==false and
          obj_move_down_v[j]==true) 
       {
        obj_move[j]=true;
        obj_move_down[j]=true;
        obj_move_down_v[j]=false;
        //на время движения новая клетка заранее занимается
        obj_matrix[obj[j].x][obj[j].y+1]=obj_matrix[obj[j].x][obj[j].y];
       }
     }//конец условия       
   }//конец цикла

  for(int j=0; j<obj_max; j++)
   {
    //для obj
    if (obj[j].unit>0 and
        obj_dead[j]==0 and //если объект жив
        obj_nuke[j]==0 and//чтобы когда взрывались не падали графически!!
        obj_matrix[obj[j].x][obj[j].y]==obj[j].unit)
     {
      //1. полет вверх
      if (obj_move_up[j]==true and
          obj_move[j]==true)
       {
        obj_move_y[j]=obj_move_y[j]-game_speed*0.055555*grid;//скорость продвижения между клетками
        if (obj_move_y[j]<=-grid) //окончание передвижения
         {
          obj_matrix[obj[j].x][obj[j].y]=0;//затираем старую матрицу
          obj[j].y=obj[j].y-1;//после передвижения передвигаемся на новую клетку физически
          obj_move_y[j]=0;
          obj_move_up[j]=false;
          obj_move[j]=false;
         }
       }

      //2. падение вниз
      if (obj_move_down[j]==true and
          obj_move[j]==true)
       {
        obj_move_y[j]=obj_move_y[j]+game_speed*0.055555*grid;;//скорость продвижения между клетками
        if (obj_move_y[j]>=grid) //окончание передвижения
         {
          obj_sleep_count[j]=5;
          obj_matrix[obj[j].x][obj[j].y]=0;//затираем старую матрицу
          obj[j].y=obj[j].y+1;//после передвижения передвигаемся на новую клетку физически
          obj_move_y[j]=0;
          obj_move_down[j]=false;
          obj_move[j]=false;
          
          //если объект бомба и после падения под ним есть опора-проигрываем звук приземления
          if (obj[j].unit==index_bomb and
              obj_matrix[obj[j].x][obj[j].y+1]>0 or
              obj[j].unit==index_bomb and
              lev_matrix[obj[j].x][obj[j].y+1]>index_door_open) {sound(12);}
         }
       } 

      //3. влево
      if (obj_move_left[j]==true and
          obj_move[j]==true)
       {
        obj_move_x[j]=obj_move_x[j]-game_speed*0.055555*grid;//скорость продвижения между клетками
        if (obj_rotate[j]==1)//крутится при движении
         {
          obj_delta[j]=obj_delta[j]+game_speed*5;//объект катится на 90' за 1 проход grid 0.055555*90
         }       

        if (obj_move_x[j]<=-grid) //окончание передвижения
         {
          if (j==obj_kick_index) {obj_kick_index=0;} //если объект был толкнут то обнуляем индекс
          obj_matrix[obj[j].x][obj[j].y]=0;//затираем старую матрицу
          obj[j].x=obj[j].x-1;//после передвижения передвигаемся на новую клетку физически
          obj_move_x[j]=0;
          obj_move_left[j]=false;
          obj_move[j]=false;
            
          if (obj_rotate[j]==1) //если объект катиться
           {
            obj_delta[j]=obj_delta[j]-game_speed*5;
            obj_rotate[j]=0;//больше не катиться
           }
         }
       }//конец влево

      //4. вправо
      if (obj_move_right[j]==true and
          obj_move[j]==true)
       {
        obj_move_x[j]=obj_move_x[j]+game_speed*0.055555*grid;//скорость продвижения между клетками
        if (obj_rotate[j]==1)//крутится при движении
         {
          obj_delta[j]=obj_delta[j]-game_speed*5;//объект катится на 90' за 1 проход grid 0.055555*90
         }

        if (obj_move_x[j]>=grid)//окончание передвижения
         {
          if (j==obj_kick_index) {obj_kick_index=0;} //если объект был толкнут то обнуляем индекс
          obj_matrix[obj[j].x][obj[j].y]=0;//затираем старую матрицу
          obj[j].x=obj[j].x+1;//после передвижения передвигаемся на новую клетку физически
          obj_move_x[j]=0;
          obj_move_right[j]=false;
          obj_move[j]=false;

          if (obj_rotate[j]==1) //если объект катиться
           {
            obj_delta[j]=obj_delta[j]+game_speed*5;
            obj_rotate[j]=0;//больше не катиться i
           }
         }
       }//конец вправо

      //анимация анимированных объектов
      if (obj_anim[j]==true)
       {
        if (obj[j].unit==index_enemy1)//ножницы
         {
          ControlAnimate(&obj_frame[j],//номер текущего кадра
                         1,//стартовый кадр
                         6,//финальный кадр
                         1,//шаг между кадрами
                         &obj_frame_count[j],//переменная отвечающая за счетчик
                         2,////задержка между кадрами в циклах
                         true);//анимация зациклена

          ControlAngle(&obj_delta[j],//номер текущего угла
                       obj_angle_finish[j],//финальный угол
                       game_speed*10,//game_speed,//game_speed*10,//шаг между углами
                       &obj_angle_count[j],//переменная отвечающая за счетчик задержки при повороте
                       1,//задержка между поворотами в циклах
                       &obj_d1[j],
                       &obj_d2[j], 
                       &obj_napr[j]);

         }// конец условия ножницы
       } //конец условия анимации

     }//конец условия
   }//конец цикла   
   
 }

void Init_OneEff(int x, int y, int k, int eff_num)//Инициация разных эффектов
 {
  //1. Молния
  if (eff_num==index_dead_light)
   {     
    eff_true[k]=true;//параметры эффетка
    eff_type[k]=eff_num;//молния
    eff_W[k]=2*rand()%grid+2*grid;
    eff_H[k]=eff_W[k];
    eff_cR[k]=1;
    eff_cG[k]=1;
    eff_cB[k]=1;
    eff_cAlpha[k]=1;
    eff_frame[k]=1;
   }
      
  //2. Взрыв
  if (eff_num==index_dead_boom)
   { 
    eff_true[k]=true;
    eff_type[k]=eff_num;//взрыв
    eff_W[k]=5*rand()%grid+5*grid;
    eff_H[k]=eff_W[k];
    eff_cR[k]=1;
    eff_cG[k]=1;
    eff_cB[k]=1;
    eff_cAlpha[k]=1;  
    eff_frame[k]=1;
   } 
  
  //3. Кровища
  if (eff_num==index_dead_blood)
   {
    sound(11);
    eff_true[k]=true;
    eff_type[k]=eff_num;//кровища
    eff_W[k]=0.8*grid;/*2*rand()%grid+2*grid;*/
    eff_H[k]=eff_W[k];
    eff_cR[k]=1;
    eff_cG[k]=1;
    eff_cB[k]=1;
    eff_cAlpha[k]=1;  
    eff_frame[k]=1;
   }
  eff_x[k]=x;
  eff_y[k]=y;     	 
 }

void Render_ObjKill()//Смерть объектов
 {
  //Если объект умер(подобран) там эффект
  for (int j=0; j<obj_max; j++)   
  for (int k=0; k<eff_num_max; k++)//начальные условие
   { 
    if (obj[j].unit>0 and 
        obj_dead[j]>0 and//если объект умер
        eff_true[k]==false) 
     {
	  Init_OneEff(obj[j].x*grid+obj_move_x[j],
	              obj[j].y*grid+obj_move_y[j],
	              k,
	              obj_dead[j]);
      obj_dead[j]=0;//объекта больше не существует
      Kill_OneObj(j);
     }//конец условия
   }//конец циклов 
 }

void Render_ObjAfterKill()
 {
  //При взрыве бомбы или врага повреждаем соседей
  for(int j=0; j<obj_max; j++)
   {
    if (obj_dead[j]==0 and //объект до взрыва жив
        obj_nuke[j]==true) //объект на подрыве
     {
      obj_nuke_count[j]=obj_nuke_count[j]+game_speed*1;//таймер до взрыва 
      obj_sigma[j]=obj_sigma[j]+game_speed*0.018*grid;//растягиваем бомбу
      if (obj_nuke_count[j]>25+rand()%10)//если объект взорвался
       {
        sound(1);
        obj_nuke[j]=false;
        obj_nuke_count[j]=0;
        obj_sigma[j]=0;
        obj_dead[j]=index_dead_boom;//взрыв

        //1.Подрываем бомбы и врагов в округе
        for(int k=0; k<obj_max; k++)
         {      
          if (obj_dead[k]==0 and//бомбы
              obj[k].unit==index_bomb and
              obj_sleep_count[k]==0 or//в спокойном состоянии 
              obj[k].unit>=index_enemy1 and//враг //**
              obj_dead[k]==0) 
           { 
			for(int m=0; m<=max_explode_radius; m++)
             {
              //Радиус взрыва на m от эпицентра
              if (obj[k].x>=obj[j].x-m and
                  obj[k].x<=obj[j].x+m and
                  obj[k].y>=obj[j].y-m and
                  obj[k].y<=obj[j].y+m and
                  obj_matrix[obj[k].x][obj[k].y]==obj[k].unit and 
                  obj_nuke[k]==false)
               {               
                obj_nuke[k]=true;
                obj_nuke_count[k]=0;
		       }//конец условия
		     }           
           }//конец условия
         }//конец цикла k
         
        //2.Подрываем процессоры и схемы-элементы уровня
        for(int l=0; l<lev_max; l++) 
         {
          if (lev[l].unit>=index_motherboard1 and//схемы
              lev[l].unit<=index_motherboard7 or
              lev[l].unit>=index_processors1 and//процессоры
              lev[l].unit<=index_processors5)
           {  
			for(int m=0; m<=max_explode_radius; m++)
             {			   
			  //Радиус взрыва на m от эпицентра   
              if (lev[l].x>=obj[j].x-m and
                  lev[l].x<=obj[j].x+m and
                  lev[l].y>=obj[j].y-m and
                  lev[l].y<=obj[j].y+m and
                  lev_matrix[lev[l].x][lev[l].y]==lev[l].unit)
               {   
                if (lev[l].unit>=index_motherboard1 and//схемы
                    lev[l].unit<=index_motherboard7) 
                 {
                  lev[l].unit=0;
                  p_kal[l]=true;
                  lev_matrix[obj[l].x][obj[l].y]=0;
                 }
                 
                if (lev[l].unit>=index_processors1 and//процессоры
                    lev[l].unit<=index_processors5) {Kill_OneLev(l);}
               }//конец условия
		     } 
           }//конец условия  
         }//конец цикла l
        
        //3. Раним колобка при взрыве,если он поблизости		
		for(int m=0; m<=max_explode_radius; m++)
         {
	      if (p_x>=obj[j].x-m and
              p_x<=obj[j].x+m and
              p_y>=obj[j].y-m and
              p_y<=obj[j].y+m)
           {
            p_live=p_live-0.3*p_live_max;
           }
         }//конец цикла  
         
       }//конец условия "если объект взорвался"
     }//конец условия "объект до взрыва жив" 
      
   }//конец цикла j
 }

void Draw_Obj()//Рисуем объекты
 {
  for(int j=0; j<obj_max; j++) 
   {
    //Рисуем уровень
    if (game_show==1 and //в игре
        obj[j].unit>0 and
        obj[j].x*grid+x_cam>0-grid and
        obj[j].x*grid+x_cam<win_DX+grid and 
        obj[j].y*grid+y_cam>0-grid and
        obj[j].y*grid+y_cam<win_DY+grid or
        edit_show==1 and//в редакторе
        obj_dead[j]==0 and 
        obj[j].unit>0 and
        obj[j].x*grid+x_cam>0-grid and
        obj[j].x*grid+x_cam<win_DX+grid and 
        obj[j].y*grid+y_cam>0-grid and
        obj[j].y*grid+y_cam<win_DY+grid)
     {
      if (obj_rotate[j]==index_rotate_loop_A)//если объект крутится все время
       {
        obj_delta[j]=obj_delta[j]-game_speed*0.07*360;//объект катится
        if (obj_delta[j]<0) {obj_delta[j]=360;}
       }
      if (obj_rotate[j]==index_rotate_loop_B)//если объект крутится все время
       {
        obj_delta[j]=obj_delta[j]+game_speed*0.01*360;//объект катится
        if (obj_delta[j]>360) {obj_delta[j]=0;}
       }
      
      if (obj_anim[j]==false) {glBindTexture(GL_TEXTURE_2D,obj_texture[obj[j].unit]);}
      if (obj_anim[j]==true) {glBindTexture(GL_TEXTURE_2D,obj_anim_texture[obj_frame[j]]);}
      
      glColor4f(dark_volume,dark_volume,dark_volume,1);//0.1        
      DrawRubberPoly(obj[j].x*grid+obj_move_x[j]+x_cam,
                     obj[j].y*grid+obj_move_y[j]+y_cam,
                     grid+obj_sigma[j],
                     grid+obj_sigma[j],
                     obj_delta[j],//градусы
                     1); 
                             		 
      if (diff_game_mode==index_game_mode_hard)//при сложном режиме сложности
       {        		 
        for(int k=0; k<=lmap_max; k++)
        for(int l=lmap[k].light_radius; l>=0; l--)
         {
          if (obj[j].x==lmap[k].x-l and //и попадает в радиус освещения
              obj[j].y>=lmap[k].y-l and
              obj[j].y<=lmap[k].y+l or     
          
              obj[j].x==lmap[k].x+l and //и попадает в радиус освещения
              obj[j].y>=lmap[k].y-l and
              obj[j].y<=lmap[k].y+l or
          
              obj[j].x>=lmap[k].x-l and //и попадает в радиус освещения
              obj[j].x<=lmap[k].x+l and 
              obj[j].y==lmap[k].y-l or   
              
              obj[j].x>=lmap[k].x-l and //и попадает в радиус освещения
              obj[j].x<=lmap[k].x+l and 
              obj[j].y==lmap[k].y+l)
           {    
            glBlendFunc(GL_SRC_ALPHA,GL_ONE);     
            glColor4f((float) 0.01*lmap[k].R*(lmap[k].light_radius-l)/lmap[k].light_radius,//0.01*level[i].R,
                      (float) 0.01*lmap[k].G*(lmap[k].light_radius-l)/lmap[k].light_radius,//0.01*level[i].G,
                      (float) 0.01*lmap[k].B*(lmap[k].light_radius-l)/lmap[k].light_radius,//0.01*level[i].B,
                      (float) 0.01*lmap[k].force*(lmap[k].light_radius-l)/lmap[k].light_radius);//0.01*level[i].A);	
      
            DrawRubberPoly(obj[j].x*grid+obj_move_x[j]+x_cam,
                           obj[j].y*grid+obj_move_y[j]+y_cam,
                           grid+obj_sigma[j],
                           grid+obj_sigma[j],
                           obj_delta[j],//градусы
                           1); 
            glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
            glColor4f(1,1,1,1);
           } 
         }   
       }                       
     }//конец условия
   }//конец цикла j
   
  //Сетка для проверки матрицы объектов 
  if (debug_lev_matrix==true)
   {
    for(int j=0;j<grid_X_MAX+1;j++)
    for(int k=0;k<grid_Y_MAX+1;k++)
     {
      if (lev_matrix[j][k]==0)
       {
        glBindTexture(GL_TEXTURE_2D,lev_texture[1]);
        glColor4f(1,1,1,0.6);
        DrawRubberPoly(j*grid+x_cam,
                       k*grid+y_cam,
                       0.6*grid,
                       0.6*grid,
                       0,
                       1); 
        glColor4f(1,1,1,1);
       }
     }
   } 

  //Сетка для проверки матрицы объектов 
  if (debug_obj_matrix==true)
   {
    for(int j=0;j<grid_X_MAX+1;j++)
    for(int k=0;k<grid_Y_MAX+1;k++)
     {
      if (obj_matrix[j][k]==0)
       {
        glBindTexture(GL_TEXTURE_2D,obj_texture[1]);
        glColor4f(1,1,1,0.6);
        DrawRubberPoly(j*grid+x_cam,
                       k*grid+y_cam,
                       0.6*grid,
                       0.6*grid,
                       0,
                       1);
        glColor4f(1,1,1,1);
       }
     }
   }
 }

//Отражение уровня по вертикали
void Mirror()
 {
  if (mirror==true)
   {
    mirror=false;
    mirror_true=not mirror_true;
    
    //Для игрока
    p_y_tmp=grid_Y_MAX-p_y;
    p_y=p_y_tmp;  

    //Для телепортов
    if (t1_true==true){t1y_tmp=grid_Y_MAX-t1y;t1y=t1y_tmp;}
    if (t2_true==true){t2y_tmp=grid_Y_MAX-t2y;t2y=t2y_tmp;}
 
    //Для lev
    bool lev_mirror[obj_max];

    for(int j=0; j<lev_max; j++)//односторонние переходов вверх или вниз
     {
      if (lev[j].unit==index_tube_UP and lev_mirror[j]==false) 
       {
        lev[j].unit=index_tube_DOWN;
        lev_matrix[lev[j].x][lev[j].y]=lev[j].unit;
        lev_mirror[j]=true;
       }
      if (lev[j].unit==index_tube_DOWN and lev_mirror[j]==false) 
       {
        lev[j].unit=index_tube_UP;
        lev_matrix[lev[j].x][lev[j].y]=lev[j].unit;
        lev_mirror[j]=true;
       }
     }//конец о переходах

    for(int j=0; j<lev_max; j++)//остальное 
    for(int k=0; k<=grid_Y_MAX; k++) 
     {
      if (lev[j].y==k) 
       {
        lev_tmp[j].y=grid_Y_MAX-k;
        lev_matrix_tmp[lev[j].x][lev[j].y]=lev_matrix[lev[j].x][lev_tmp[j].y];
        lev_matrix[lev[j].x][lev[j].y]=0;
       }
     }
    for(int j=0; j<lev_max; j++) 
     {
      lev[j].y=lev_tmp[j].y;
      lev_matrix[lev[j].x][lev[j].y]=lev_matrix_tmp[lev[j].x][lev[j].y];
     }

    //Для obj
    for(int j=0; j<obj_max; j++) 
    for(int k=0; k<=grid_Y_MAX; k++) 
     {
      if (obj[j].y==k) 
       {
        obj_tmp[j].y=grid_Y_MAX-k;
        obj_matrix_tmp[obj[j].x][obj[j].y]=obj_matrix[obj[j].x][obj_tmp[j].y];
         
        for(int l=0; l<=grid_X_MAX; l++)//затираем матрицу 
        for(int m=0; m<=grid_Y_MAX; m++) 
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
 
        //Меняем направления вражин-вертикальной хрени
        if (obj[j].unit==index_enemy3)
         {
		  int tmp=1;//для выбора условия
		  if (obj_AI_param1[j]==1 and tmp==1) 
           {
			obj_AI_param1[j]=2; 
            tmp=2;
           }
          if (obj_AI_param1[j]==2 and tmp==1) 
           {
			obj_AI_param1[j]=1; 
			tmp=2;
		   }
         }//конец смены направления
         
       } 
     }//конец цикла

    for(int j=0; j<obj_max; j++) 
     {
      obj[j].y=obj_tmp[j].y;
      obj_matrix[obj[j].x][obj[j].y]=obj_matrix_tmp[obj[j].x][obj[j].y];
     }
    
    //Для лайтмапов
    if (diff_game_mode==index_game_mode_hard) //в сложном ночном режиме игры
     {
      //Для освещения игрока
      Clear_OneLmapPos(0);
      Render_OneLmapPos(0,p_x,p_y);

      //Остальные
      for(int i=0; i<=grid_X_MAX; i++) 
      for(int j=0; j<=grid_Y_MAX; j++)
       {
        lmap_matrix_tmp[i][j]=lmap_matrix[i][grid_Y_MAX-j];
       }

      for(int i=0; i<=grid_X_MAX; i++) 
      for(int j=0; j<=grid_Y_MAX; j++)
       {
        lmap_matrix[i][j]=lmap_matrix_tmp[i][j];
       }
      
      for(int i=0; i<=grid_X_MAX; i++) 
      for(int j=0; j<=grid_Y_MAX; j++) 
      for(int k=1; k<=lmap_max; k++)
       {
		if (lmap[k].undead>0 and
		    lmap_matrix[i][j]==k)
		 {
	      lmap[k].x=i;
          lmap[k].y=j; 
		 }   	   
	   }
      
     }//конец лайтмапов
     
   }//конец условия
 }

void Init_PlayerStartPos()//место старта колобка
 {
  for(int i=0; i<lev_max; i++) 
   {
    if (lev[i].unit==index_start_pos)//1-ый элемент-старт игрока
     {
      p_x=lev[i].x;
      p_y=lev[i].y; 
     }
   }//конец цикла
 }
 
void Start_LevelNum(int level_num)
 {      
  Reset_Game();
  Load_Level(level_num);
  Init_Lev_Doors();//инициализация состояния дверей
  Init_Lev_Lights();//инициализация состояния ламп
  Init_Lmap();
  Render_Lmap();
  Init_PlayerStartPos();
 }
 
void PlayerMoveUp()
 {
  p_move_up_v=false;
  p_move_up=true;

  p_move_stage=1;

  p_move_right=false;
  p_move_left=false;
  p_move_down=false;
 } 

void PlayerMoveDown()
 {
  p_move_down_v=false;
  p_move_down=true;

  p_move_stage=1;

  p_move_right=false;
  p_move_left=false;
  p_move_up=false;
 } 

void PlayerMoveLeft()
 {
  p_move_left_v=false;
  p_move_left=true;

  p_move_stage=1; 
  p_frame_start=3;    

  p_move_right=false;
  p_move_up=false;
  p_move_down=false;
 } 

void PlayerMoveRight()
 {
  p_move_right_v=false;
  p_move_right=true;

  p_move_stage=1;
  p_frame_start=6;
        
  p_move_left=false;
  p_move_up=false;
  p_move_down=false;
 } 

void KeyDown_Player()//Управление при постоянном нажатии клавиши
 {
  //Если колобок жив от им можно управлять 
  if (p_true==true and p_move_stage==0)
   {
    if (keys[SDLK_LEFT] and //влево
        p_move_left_v==true)//если есть возможность пройти влево
     {
      PlayerMoveLeft();
     } 

    if (keys[SDLK_RIGHT] and //вправо 
        p_move_right_v==true)//вправо 
     {
      PlayerMoveRight();
     } 
      
    if (keys[SDLK_UP] and //вверх
        p_move_up_v==true) 
     {
      PlayerMoveUp();
     }
    
    if (keys[SDLK_DOWN] and//вниз
        p_move_down_v==true) 
     {
      PlayerMoveDown();
     }      
   }//конец управления игрока

  //Цправление камерой
  if (keys[SDLK_w]) {y_cam=y_cam+0.15*grid;} 
  if (keys[SDLK_s]) {y_cam=y_cam-0.15*grid;} 
  if (keys[SDLK_a]) {x_cam=x_cam+0.15*grid;} 
  if (keys[SDLK_d]) {x_cam=x_cam-0.15*grid;}
 }

void ControlAnimate_Player(int *frame,//номер текущего кадра
                          int frame_start,//стартовый кадр
                          int frame_finish,//финальный кадр
                          int frame_delta,//шаг между кадрами
                          int *frame_count,//переменная отвечающая за счетчик
                          int delay,//задержка между кадрами в циклах
                          bool frame_cikl)//зациклить ли анимацию
 {
  *frame_count=*frame_count+1;//счетчик задержки 
  if (*frame_count>=delay)
   {
    *frame_count=0;
    *frame=*frame+frame_delta;//следующий кадр анимации
   }

  if (*frame>=frame_finish or
      *frame<frame_start)
   {
    *frame=frame_finish;
    if (frame_cikl==true) {*frame=frame_start;}//возвращаемся на старый кадр
   }
 }

void Render_PlayerMove()//Обработка движений колобка
 {
  if (p_true==true) //Если колобок жив, то
   {
    if (p_move_stage==0)
     {
      p_face_frame_count=p_face_frame_count+1;
      if (p_face_frame_count>50)//смотрим как на гавно
       {  
        p_frame=2;
       }
      else {p_frame=1;}
     }//конец "стоим тупо улыбаемся когда колобок не движется"
  
    if (p_move_stage==1)//стадия передвижения
     { 
      p_face_frame_count=0;
      
      //Вверх
      if (p_move_up==true) 
       {
        p_move_y=p_move_y-game_speed*1.4*0.055555*grid;//скорость продвижения между клетками 
       
        if (p_move_y<-grid) //окончание передвижения
         {
          p_y=p_y-1;//после передвижения передвигаемся на новую клетку физически
          
          //Перещаем видимый радиус
          if (diff_game_mode==index_game_mode_hard)//при сложном режиме сложности
           {
            Clear_OneLmapPos(0);
            Render_OneLmapPos(0,p_x,p_y);
           }
           
          p_move_y=0;
          p_move_up=false;

          p_move_stage=0;
          //p_last_move=index_UP;//сохраняем последнее движение    
         }
       }

      //Вниз
      if (p_move_down==true) 
       {
        p_move_y=p_move_y+game_speed*1.4*0.055555*grid;//скорость продвижения между клетками
 
        if (p_move_y>grid) //окончание передвижения
         {
          p_y=p_y+1;//после передвижения передвигаемся на новую клетку физически
          
          //Перещаем видимый радиус
          if (diff_game_mode==index_game_mode_hard)//при сложном режиме сложности
           {
            Clear_OneLmapPos(0);
            Render_OneLmapPos(0,p_x,p_y);
           }
          
          p_move_y=0;
          p_move_down=false;

          p_move_stage=0;
          //p_last_move=index_DOWN;//сохраняем последнее движение  
         }
       }
 
      //Влево
      if (p_move_left==true) 
       {
        p_move_x=p_move_x-game_speed*1.4*0.055555*grid;//скорость продвижения между клетками  

        if (p_move_x<-grid) //окончание передвижения
         {
          p_x=p_x-1;//после передвижения передвигаемся на новую клетку физически
          
          //Перещаем видимый радиус
          if (diff_game_mode==index_game_mode_hard)//при сложном режиме сложности
           {
            Clear_OneLmapPos(0);
            Render_OneLmapPos(0,p_x,p_y);
           }
          
          p_move_x=0;
          p_move_left=false;

          p_move_stage=0;
          //p_last_move=index_LEFT;//сохраняем последнее движение  
         }
       }

      //Вправо
      if (p_move_right==true) 
       {
        p_move_x=p_move_x+game_speed*1.4*0.055555*grid;//скорость продвижения между клетками
  
        if (p_move_x>grid) //окончание передвижения
         {
          p_x=p_x+1;//после передвижения передвигаемся на новую клетку физически
          
          //Перещаем видимый радиус
          if (diff_game_mode==index_game_mode_hard)//при сложном режиме сложности
           {
            Clear_OneLmapPos(0);
            Render_OneLmapPos(0,p_x,p_y);
           }

          p_move_x=0;
          p_move_right=false;

          p_move_stage=0;
          //p_last_move=index_RIGHT;//сохраняем последнее движение  
         }
       }

      //Анимация движения колобка
      ControlAnimate_Player(&p_frame,//номер текущего кадра
                           p_frame_start,//стартовый кадр
                           p_frame_start+3,//финальный кадр=стратовый + 2 кадра на каждое движение
                           1,//шаг между кадрами
                           &p_frame_count,//переменная отвечающая за счетчик
                           3,//задержка между кадрами в циклах
                           true);//зациклить ли анимацию
     }//конец стадии передвижения 1

    //Перемещение игрового экрана-камеры вслед за игроком
    if ((p_x*grid+x_cam)>(win_DX-0.3333*win_DX)) {x_cam=x_cam-0.15*grid;}//смещение на 0.15 от grid
    if ((p_x*grid+x_cam)<(0+0.3333*win_DX)) {x_cam=x_cam+0.15*grid;}
    if ((p_y*grid+y_cam)>(win_DY-0.3333*win_DY)) {y_cam=y_cam-0.15*grid;}
    if ((p_y*grid+y_cam)<(0+0.3333*win_DY)) {y_cam=y_cam+0.15*grid;}

    //Ограничение передвижения колобка внутри игрового чистого поля
    if (p_x>grid_X_MAX) {p_x=grid_X_MAX;} 
    if (p_x<0) {p_x=0;}  
    if (p_y>grid_Y_MAX) {p_y=grid_Y_MAX;} 
    if (p_y<0) {p_y=0;}    

   }//конец если "Если колобок жив, то"
 } 

void KeyClick_Player()//Управление при однократном нажатии клавиши
 {
  if (keys[SDLK_F1]) {debug_lev_matrix= not debug_lev_matrix;}
  if (keys[SDLK_F2]) {debug_obj_matrix= not debug_obj_matrix;}
  if (keys[SDLK_F3]) //отразить уровень по вертикали
   {
    mirror=true;
    Mirror();
   }

  //Останавливаем трек, чтобы музыка переключилась
  if (keys[SDLK_F4]) {stopmusic();}

  //Если колобок жив от им можно управлять
  if (p_true==true) 
   {
    //Направление для подрыва бомбы
    if (keys[SDLK_LEFT]) {p_vector=index_LEFT;} 
    if (keys[SDLK_RIGHT]) {p_vector=index_RIGHT;} 
    if (keys[SDLK_UP]) {p_vector=index_UP;} 
    if (keys[SDLK_DOWN]) {p_vector=index_DOWN;}

    //За 5 очков можем увеличить радиус видимости
    if (event.key.keysym.sym==SDLK_v and
        diff_game_mode==index_game_mode_hard and
        p_score>=cost_viewradius)
     {
      p_score=p_score-cost_viewradius;
      lmap[0].light_radius=lmap[0].light_radius+2;
     }

    //Открываем/закрываем двери когда игрок не движется, жмем кнопки
    if (event.key.keysym.sym==SDLK_b and
        p_move_left==false and//слева
        p_move_right==false and//справа
        p_move_up==false and//сверху
        p_move_down==false and //снизу
        p_face_frame_count>5)//и прошло какоето время с конца движения
     {
     //Кнопка поворота уровня по вертикали
      for(int j=0; j<lev_max; j++) 
       {
        if (lev[j].unit==index_button_push and//либо синяя //29
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
         {
          if (lev[j].x==p_x and //1 открываем снизу
              lev[j].y==p_y+1 and
              p_vector==index_DOWN)      
           {
            sound(9);
            mirror=true; 
            lev_button_push[j]=not lev_button_push[j];
            break;
           }    

          if (lev[j].x==p_x and //2 открываем сверху
              lev[j].y==p_y-1 and
              p_vector==index_UP)      
           {
            sound(9);
            mirror=true; 
            lev_button_push[j]=not lev_button_push[j];
            break;
           }       

          if (lev[j].x==p_x-1 and //3 открываем влево
              lev[j].y==p_y and
              p_vector==index_LEFT)      
           {
            sound(9);
            mirror=true;
            lev_button_push[j]=not lev_button_push[j];
            break;
           }     

          if (lev[j].x==p_x+1 and //4 открываем враво
              lev[j].y==p_y and
              p_vector==index_RIGHT)      
           {
            sound(9);
            mirror=true;
            lev_button_push[j]=not lev_button_push[j];
            break;
           }        
         }     
       }//конец цикла

      //Обычная дверь 
      for(int j=0; j<lev_max; j++) 
       {
        if (lev[j].unit==index_door_open and//дверь открыта //4
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit or
            lev[j].unit==index_door_close and//дверь закрыта //12
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
         {
          if (lev[j].x==p_x and //1 нажимаем вниз
              lev[j].y==p_y+1 and
              p_vector==index_DOWN)      
           {
            sound(9);
            lev_door_open[j]= not lev_door_open[j];
            break;  
           }
               
          if (lev[j].x==p_x and //2 нажимаем вверх
              lev[j].y==p_y-1 and
              p_vector==index_UP)      
           {
            sound(9);
            lev_door_open[j]= not lev_door_open[j];
            break;  
           }       

          if (lev[j].x==p_x-1 and //3 нажимаем влево
              lev[j].y==p_y and
              p_vector==index_LEFT)      
           {
            sound(9);
            lev_door_open[j]= not lev_door_open[j];
            break;  
           }     

          if (lev[j].x==p_x+1 and //4 нажимаем вправо
              lev[j].y==p_y and
              p_vector==index_RIGHT)      
           {
            sound(9);
            lev_door_open[j]= not lev_door_open[j];
            break;  
           }        
         }     
       }//конец цикла

      //Светильник
      for(int j=0; j<lev_max; j++) 
       {
        if (lev[j].unit==index_lamp_on and//лампочка вкл
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit or
            lev[j].unit==index_lamp_off and//лампочка выкл
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
         {
          if (lev[j].x==p_x and //1 нажимаем вниз
              lev[j].y==p_y+1 and
              p_vector==index_DOWN)      
           {
            sound(9);
            lev_light_push[j]= not lev_light_push[j];
            Level_Control_Lights(j);
            break;  
           }    

          if (lev[j].x==p_x and //2 нажимаем вверх
              lev[j].y==p_y-1 and
              p_vector==index_UP)      
           {
            sound(9);
            lev_light_push[j]= not lev_light_push[j];
            Level_Control_Lights(j);
            break;  
           }       

          if (lev[j].x==p_x-1 and //3 нажимаем влево
              lev[j].y==p_y and
              p_vector==index_LEFT)      
           {
            sound(9);
            lev_light_push[j]= not lev_light_push[j];
            Level_Control_Lights(j);
            break;  
           }     

          if (lev[j].x==p_x+1 and //4 нажимаем враво
              lev[j].y==p_y and
              p_vector==index_RIGHT)      
           {
            sound(9);
            lev_light_push[j]= not lev_light_push[j];
            Level_Control_Lights(j);
            break;  
            
           }        
         }//конец условия     
       }//конец цикла

      //Серебряная дверь с замком
      for(int j=0; j<lev_max; j++) 
       {
        if (p_silver>0 and//если есть серебряный ключ
            lev[j].unit==index_siver_door and//серебряная дверь-дискета
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
         {
          if (lev[j].x==p_x and //1 открываем вниз
              lev[j].y==p_y+1 and
              p_vector==index_DOWN)      
           {
            sound(10);
            Kill_OneLev(j); 
            p_silver=p_silver-1;
            break;  
           }    

          if (lev[j].x==p_x and //2 открываем вверх
              lev[j].y==p_y-1 and
              p_vector==index_UP)      
           {
            sound(10);
            Kill_OneLev(j); 
            p_silver=p_silver-1;
            break; 
           }       

          if (lev[j].x==p_x-1 and //3 открываем влево
              lev[j].y==p_y and
              p_vector==index_LEFT)      
           {
            sound(10);
            Kill_OneLev(j); 
            p_silver=p_silver-1;
            break; 
           }     

          if (lev[j].x==p_x+1 and //4 открываем вправо
              lev[j].y==p_y and
              p_vector==index_RIGHT)      
           {
            sound(10);
            Kill_OneLev(j); 
            p_silver=p_silver-1;
            break;
           }
         }     
       }//конец цикла про серебряную дверь

      //Золотая дверь с замком
      for(int j=0; j<lev_max; j++) 
       {
        if (p_gold>0 and//если есть золотой ключ
            lev[j].unit==index_gold_door and//золотая дверь-дискета
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
         {
          if (lev[j].x==p_x and //1 открываем вниз
              lev[j].y==p_y+1 and
              p_vector==index_DOWN)      
           {
            sound(10);
            Kill_OneLev(j); 
            p_gold=p_gold-1;
            break;  
           }    

          if (lev[j].x==p_x and //2 открываем вверх
              lev[j].y==p_y-1 and
              p_vector==index_UP)      
           {
            sound(10);
            Kill_OneLev(j); 
            p_gold=p_gold-1;
            break; 
           }       

          if (lev[j].x==p_x-1 and //3 открываем влево
              lev[j].y==p_y and
              p_vector==index_LEFT)      
           {
            sound(10);
            Kill_OneLev(j); 
            p_gold=p_gold-1;
            break; 
           }     

          if (lev[j].x==p_x+1 and //4 открываем вправо
              lev[j].y==p_y and
              p_vector==index_RIGHT)      
           {
            sound(10);
            Kill_OneLev(j); 
            p_gold=p_gold-1;
            break;
           }
         }     
       }//конец цикла про золотую дверь
     }//конец "открываем/закрываем двери"
     
    //Подраываем рядом стоящую бомбу 
    if (event.key.keysym.sym==SDLK_b)
     {
      for(int j=0; j<obj_max; j++) 
       {
        if (p_score>=cost_explode and //за 10 очков
            obj[j].unit==index_bomb and
            obj_matrix[obj[j].x][obj[j].y]==obj[j].unit and
            obj_dead[j]==0 and //если объект жив
            obj_move[j]==false and
            obj_sleep_count[j]==0) 
         {
          if (obj[j].x==p_x and //1 нюкаем вниз
              obj[j].y==p_y+1 and
              p_vector==index_DOWN and
              obj_nuke[j]==false)
           {
            obj_nuke[j]=true;
            obj_nuke_count[j]=0;
            p_score=p_score-cost_explode;
            break;  
           }    

          if (obj[j].x==p_x and //2 нюкаем вверх
              obj[j].y==p_y-1 and
              p_vector==index_UP and
              obj_nuke[j]==false)
           {
            obj_nuke[j]=true;
            obj_nuke_count[j]=0;
            p_score=p_score-cost_explode;
            break;  
           }  

          if (obj[j].x==p_x-1 and//3 нюкаем влево
              obj[j].y==p_y and
              p_vector==index_LEFT and
              obj_nuke[j]==false)
           {
            obj_nuke[j]=true;
            obj_nuke_count[j]=0;
            p_score=p_score-cost_explode;
            break;  
           }    

          if (obj[j].x==p_x+1 and//4 нюкаем вправо
              obj[j].y==p_y and
              p_vector==index_RIGHT and
              obj_nuke[j]==false)
           {
            obj_nuke[j]=true;
            obj_nuke_count[j]=0;
            p_score=p_score-cost_explode;
            break;  
           }   
         }     
       }//конец цикла
     }//конец "подрываем рядом стоящую бомбу"

    //Установка входа в телепорт
    if (keys[SDLK_n]) 
     {
      sound(7);
      t1_true=true;
      t1x=p_x;
      t1y=p_y;   
     } 

    //Установка выхода из телепорта
    if (keys[SDLK_m]) 
     {
      sound(7);
      t2_true=true;
      t2x=p_x;
      t2y=p_y;  
     }
   }//конец "Если колобок жив от им можно управлять"
 }
 
void Render_PlayerColLevObj()//Обработка столконвений колобка c lev и obj
 {
  if (p_true==true and p_move_stage==0) //Если колобок жив и не движется
   {
    //Проверка возможности перемещения колобка на новую клетку в разные стороны
    p_move_left_v=true;
    p_move_right_v=true;
    p_move_up_v=true;
    p_move_down_v=true;

    //1) Взаимодействие колобка с LEV
    for(int i=0; i<lev_max; i++) 
     {
	  //Если заходим колобком на схемы	 
      if (lev[i].x==p_x and 
          lev[i].y==p_y and
          lev_matrix[lev[i].x][lev[i].y]>=index_motherboard1 and
          lev_matrix[lev[i].x][lev[i].y]<=index_motherboard7)
       {
        sound(8);	 
        lev_matrix[lev[i].x][lev[i].y]=0;
        lev[i].unit=0;
        p_kal[i]=true;//оставляем кал вместо схемы 
        break;       
       }
      
      //Выход на следущий уровень
      if (lev[i].x==p_x and 
          lev[i].y==p_y and
          lev[i].unit==index_exit_pos1 or
          lev[i].x==p_x and 
          lev[i].y==p_y and
          lev[i].unit==index_exit_pos2)   
       {
        if (lev[i].unit==index_exit_pos1 and //если собрано нужное количество очков
            p_score>=lev[i].exit_score or
            lev[i].unit==index_exit_pos2 and //если скокото врагов поедено
            lev[i].exit_kills<=0) //грузим следущий уровень
         {     
          if (game_timer<best_time[level_num])//если время лучше чем в списке заменяем его и сохраняем результат 
           {
            best_time[level_num]=game_timer; //занесение времени в массив
            SaveBestTime();//сохраняем результат
           }
   
          //Изменение номера текущего уровня
          level_num=level_num+1;
          if (level_num>level_max) {level_num=1;}
          Start_LevelNum(level_num);
          break;
         }
       }//конец цикла
     }//конец условия

    //фильтрация возможностей для матрицы lev (>10)
    if (lev_matrix[p_x-1][p_y]>=index_door_close) {p_move_left_v=false;} //слева
    if (lev_matrix[p_x+1][p_y]>=index_door_close) {p_move_right_v=false;} //справа
    if (lev_matrix[p_x][p_y-1]>=index_door_close) {p_move_up_v=false;} //сверху
    if (lev_matrix[p_x][p_y+1]>=index_door_close) {p_move_down_v=false;} //снизу

    //====ПРОХОДЫ====
    for(int j=0; j<lev_max; j++) 
     {
      //Проход вверх ^ 
      if (lev[j].unit==index_tube_UP and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       {
        if (p_y==lev[j].y+1 and p_x==lev[j].x) {p_move_up_v=true;} //около
        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         {
          p_move_left_v=false;//вылезти из вертикальной трубки нельзя 
          p_move_right_v=false;
          
          //В зависимости от предыдущего направления движения
          if (p_vector==index_UP)
           {
            if (obj_matrix[lev[j].x][lev[j].y-1]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x][lev[j].y-1]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x][lev[j].y-1]==index_tube_UP or// ^
                  lev_matrix[lev[j].x][lev[j].y-1]==index_tube_UPDOWN or// || 
                  lev_matrix[lev[j].x][lev[j].y-1]==index_tube_CROSS)// + 
               {PlayerMoveUp();}
             } 
           }
         }
       }
      //Проход вниз v
      if (lev[j].unit==index_tube_DOWN and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       { 
        if (p_y==lev[j].y-1 and p_x==lev[j].x) {p_move_down_v=true;}//около
        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         { 
          p_move_left_v=false;//вылезти из вертикальной трубки нельзя 
          p_move_right_v=false;
          
          //В зависимости от предыдущего направления движения
          if (p_vector==index_DOWN)
           {
            if (obj_matrix[lev[j].x][lev[j].y+1]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x][lev[j].y+1]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x][lev[j].y+1]==index_tube_DOWN or// v
                  lev_matrix[lev[j].x][lev[j].y+1]==index_tube_UPDOWN or// || 
                  lev_matrix[lev[j].x][lev[j].y+1]==index_tube_CROSS)// + 
               {PlayerMoveDown();}
             }
           } 
         }
       }

      //Проход влево <
      if (lev[j].unit==index_tube_LEFT and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       {
        if (p_y==lev[j].y and p_x==lev[j].x+1) {p_move_left_v=true;}//около
        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         {
          p_move_up_v=false;//вылезти из горизонтальной трубки нельзя 
          p_move_down_v=false;
          
          //В зависимости от предыдущего направления движения
          if (p_vector==index_LEFT)
           {
            if (obj_matrix[lev[j].x-1][lev[j].y]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x-1][lev[j].y]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x-1][lev[j].y]==index_tube_LEFT or// <
                  lev_matrix[lev[j].x-1][lev[j].y]==index_tube_LEFTRIGHT or// = 
                  lev_matrix[lev[j].x-1][lev[j].y]==index_tube_CROSS)// + 
               {PlayerMoveLeft();}
             }   
           } 
         }
       }

      //Проход вправо >
      if (lev[j].unit==index_tube_RIGHT and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       {
        if (p_y==lev[j].y and p_x==lev[j].x-1) {p_move_right_v=true;}//около
        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         {
          p_move_up_v=false;//вылезти из горизонтальной трубки нельзя 
          p_move_down_v=false;
          
          //В зависимости от предыдущего направления движения
          if (p_vector==index_RIGHT)
           {
            if (obj_matrix[lev[j].x+1][lev[j].y]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x+1][lev[j].y]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x+1][lev[j].y]==index_tube_RIGHT or// >
                  lev_matrix[lev[j].x+1][lev[j].y]==index_tube_LEFTRIGHT or// = 
                  lev_matrix[lev[j].x+1][lev[j].y]==index_tube_CROSS)// + 
               {PlayerMoveRight();}
             }  
           }
         }         
       }

      //Проход вверх-низ ||
      if (lev[j].unit==index_tube_UPDOWN and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       {
        //вверх
        if (p_y==lev[j].y+1 and p_x==lev[j].x) {p_move_up_v=true;}//около снизу-наверх
        //вниз
        if (p_y==lev[j].y-1 and p_x==lev[j].x) {p_move_down_v=true;}//около сверху-вниз

        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         {
          p_move_left_v=false;//вылезти из вертикальной трубки нельзя 
          p_move_right_v=false;
          
          //В зависимости от предыдущего направления движения
          if (p_vector==index_UP)//снизу-наверх
           {
            if (obj_matrix[lev[j].x][lev[j].y-1]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x][lev[j].y-1]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x][lev[j].y-1]==index_tube_UP or// ^
                  lev_matrix[lev[j].x][lev[j].y-1]==index_tube_UPDOWN or// || 
                  lev_matrix[lev[j].x][lev[j].y-1]==index_tube_CROSS)// + 
               {PlayerMoveUp();}
             } 
           }

          if (p_vector==index_DOWN)//сверху-вниз
           {
            if (obj_matrix[lev[j].x][lev[j].y+1]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x][lev[j].y+1]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x][lev[j].y+1]==index_tube_DOWN or// v
                  lev_matrix[lev[j].x][lev[j].y+1]==index_tube_UPDOWN or// || 
                  lev_matrix[lev[j].x][lev[j].y+1]==index_tube_CROSS)// + 
               {PlayerMoveDown();}
             } 
           }
         }//конец внутри 
       }//конец ||

      //Проход влево-вправо =
      if (lev[j].unit==index_tube_LEFTRIGHT and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       {
        //влево
        if (p_y==lev[j].y and p_x==lev[j].x+1) {p_move_left_v=true;}//около справа-налево
        //вправо
        if (p_y==lev[j].y and p_x==lev[j].x-1) {p_move_right_v=true;}//около слево-направо

        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         {
          p_move_up_v=false;//вылезти из горизонтальной трубки нельзя 
          p_move_down_v=false;
          //В зависимости от предыдущего направления движения          
          if (p_vector==index_LEFT)//справа-налево
           {
            if (obj_matrix[lev[j].x-1][lev[j].y]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x-1][lev[j].y]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x-1][lev[j].y]==index_tube_LEFT or// <
                  lev_matrix[lev[j].x-1][lev[j].y]==index_tube_LEFTRIGHT or// = 
                  lev_matrix[lev[j].x-1][lev[j].y]==index_tube_CROSS)// + 
               {PlayerMoveLeft();}
             } 
           }

          if (p_vector==index_RIGHT)//слево-направо
           {
            if (obj_matrix[lev[j].x+1][lev[j].y]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x+1][lev[j].y]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x+1][lev[j].y]==index_tube_RIGHT or// >
                  lev_matrix[lev[j].x+1][lev[j].y]==index_tube_LEFTRIGHT or// = 
                  lev_matrix[lev[j].x+1][lev[j].y]==index_tube_CROSS)// + 
               {PlayerMoveRight();}
             }
           }
         }//конец внутри
       }//конец =

      //Проход крестовина вверх-низ-влево-вправо +
      if (lev[j].unit==index_tube_CROSS and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       {
        //вверх
        if (p_y==lev[j].y+1 and p_x==lev[j].x) {p_move_up_v=true;}//около снизу-наверх
        //вниз
        if (p_y==lev[j].y-1 and p_x==lev[j].x) {p_move_down_v=true;}//около сверху-вниз
        //влево
        if (p_y==lev[j].y and p_x==lev[j].x+1) {p_move_left_v=true;}//около справа-налево
        //вправо
        if (p_y==lev[j].y and p_x==lev[j].x-1) {p_move_right_v=true;}//около слева-направо

        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         {
          //В зависимости от предыдущего направления движения
          if (p_vector==index_UP)//снизу-наверх
           {
            if (obj_matrix[lev[j].x][lev[j].y-1]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x][lev[j].y-1]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x][lev[j].y-1]==index_tube_UP or// ^
                  lev_matrix[lev[j].x][lev[j].y-1]==index_tube_UPDOWN or// || 
                  lev_matrix[lev[j].x][lev[j].y-1]==index_tube_CROSS)// + 
               {PlayerMoveUp();}
             } 
           }

          if (p_vector==index_DOWN)//сверху-вниз
           {
            if (obj_matrix[lev[j].x][lev[j].y+1]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x][lev[j].y+1]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x][lev[j].y+1]==index_tube_DOWN or// v
                  lev_matrix[lev[j].x][lev[j].y+1]==index_tube_UPDOWN or// || 
                  lev_matrix[lev[j].x][lev[j].y+1]==index_tube_CROSS)// + 
               {PlayerMoveDown();}
             } 
           }

          if (p_vector==index_LEFT)//справа-налево
           {
            if (obj_matrix[lev[j].x-1][lev[j].y]!=index_bomb)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x-1][lev[j].y]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x-1][lev[j].y]==index_tube_LEFT or// <
                  lev_matrix[lev[j].x-1][lev[j].y]==index_tube_LEFTRIGHT or// = 
                  lev_matrix[lev[j].x-1][lev[j].y]==index_tube_CROSS)// + 
               {PlayerMoveLeft();}
             } 
           }

          if (p_vector==index_RIGHT)//слево-направо
           {
            if (obj_matrix[lev[j].x+1][lev[j].y]!=index_tube_CROSS)//на выходе нет бомбы
             { 
              if (lev_matrix[lev[j].x+1][lev[j].y]<=index_motherboard7 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x+1][lev[j].y]==index_tube_RIGHT or// >
                  lev_matrix[lev[j].x+1][lev[j].y]==index_tube_LEFTRIGHT or// = 
                  lev_matrix[lev[j].x+1][lev[j].y]==index_tube_CROSS)// + 
               {PlayerMoveRight();}
             }
           }
           
         }//конец внутри
       }//конец "крестовина" +
     }//конец цикла про проходы

    //2)Взаимодействие колобка с obj, и при этом определяем границы движения колобка 
    
    //Фильтрация возможностей для матрицы obj=6 (только по бомбам не может ходить)
    if (obj_matrix[p_x-1][p_y]==index_bomb) {p_move_left_v=false;}//слева 
    if (obj_matrix[p_x+1][p_y]==index_bomb) {p_move_right_v=false;}//справа
    if (obj_matrix[p_x][p_y-1]==index_bomb) {p_move_up_v=false;}//сверху
    if (obj_matrix[p_x][p_y+1]==index_bomb) {p_move_down_v=false;}//снизу

    //Сверху-колобок неможет пройти по бомбам
    if (obj_matrix[p_x][p_y-1]==index_bomb) {p_move_up_v=false;}
    
    //Снизу-колобок неможет пройти
    if (obj_matrix[p_x][p_y+1]==index_bomb) {p_move_down_v=false;}
   
   }//конец условия "Если колобок жив, то"
 }
  
void Render_PlayerDead()//Столкновение колобка с бомбой и врагом
 {
  for (int j=0; j<obj_max;j++)   
  for (int k=0; k<eff_num_max;k++)//для начального условие для взрыва
   { 
    if (p_true==true and //столкновение с врагом и бомбами
        obj_dead[j]==0 and
        obj[j].unit>=6 and
        p_x*grid+p_move_x-0.5*grid<obj[j].x*grid+obj_move_x[j]+0.5*grid and
        p_x*grid+p_move_x+0.5*grid>obj[j].x*grid+obj_move_x[j]-0.5*grid and
        p_y*grid+p_move_y-0.5*grid<obj[j].y*grid+obj_move_y[j]+0.5*grid and
        p_y*grid+p_move_y+0.5*grid>obj[j].y*grid+obj_move_y[j]-0.5*grid and
        eff_true[k]==false)
     {
      sound(11);//крик     
      Init_OneEff(p_x*grid+0.8*(rand()%grid+1)-0.8*(rand()%grid+1),
                  p_y*grid+0.8*(rand()%grid+1)-0.8*(rand()%grid+1),
                  k,
                  index_dead_blood);
      p_live=p_live-game_speed*1;//отнимаем у колобка жизнь 
      break; 
     }//выход из условия "столкновения"
   }//конец циклов

  if (p_live<=0)//если жизнь кончилась, то колобок сдох
   {
    p_live=0; 
    p_true=false;//колобок больше не существует и не жилец:(
   }
 }  

void Render_PlayerEatObj()//Колобок ест бонусы (obj=1-6) и собирает при этом очки
 {
  for(int j=0; j<obj_max; j++) 
   {
    if (p_true==true and//если колобок и объект существуют
        obj_dead[j]==0 and
        obj[j].unit>0 and
        obj[j].unit<index_bomb and//жрем бонусы до 6-го объекта-бомбы не жрем
        p_x*grid+p_move_x-0.44*grid<obj[j].x*grid+obj_move_x[j]+0.44*grid and
        p_x*grid+p_move_x+0.44*grid>obj[j].x*grid+obj_move_x[j]-0.44*grid and
        p_y*grid+p_move_y-0.44*grid<obj[j].y*grid+obj_move_y[j]+0.44*grid and
        p_y*grid+p_move_y+0.44*grid>obj[j].y*grid+obj_move_y[j]-0.44*grid)
     { 
      obj_matrix[p_x][p_y]=0;//стираем объект из матрицы объектов
      obj_dead[j]=index_dead_light;//молния на месте съедания
      
      //Жрем колобком разные объекты 
      if (obj[j].unit==index_penguin)//мелкий пингвин
       {
        sound(2); 
        p_score=p_score+1;
       }
      if (obj[j].unit==index_big_penguin)//жирный пингвин
       {
        sound(3);
        p_score=p_score+5;//начисляем очки за поедание жирного пингвина
       }
      if (obj[j].unit==index_health)//лечилка
       {
        sound(4);
        p_live=p_live+0.3*p_live_max;//поедаем лечилку и лечимся
        if (p_live>=p_live_max)//если жизни много, то небольше максимального значения
         {p_live=p_live_max;}
       }
      if (obj[j].unit==index_silver_key)//серебряная звезда - ключ
       {
        sound(5);
        p_silver=p_silver+1;
       }
      if (obj[j].unit==index_gold_key)//золотая звезда - ключ 
       {
        sound(6);
        p_gold=p_gold+1;
       }
     }//конец условия
   }//конец цикла
 }

void Draw_Player()//Рисуем колобка
 {
  if (p_true==true)//если колобок толкал снимаем метку//если колобок жив
   {      
    
    if (lev_matrix[p_x][p_y]>=index_tube_UP and 
        lev_matrix[p_x][p_y]<=index_tube_CROSS)//колобок прозрачен в трубах
     {glColor4f(1,1,1,0.55);}
    else {glColor4f(1,1,1,1);}

    glBindTexture(GL_TEXTURE_2D,player_texture[p_frame]);
    DrawRubberPoly(p_x*grid+p_move_x+x_cam,
                   p_y*grid+p_move_y+y_cam,
                   grid,
                   grid,
                   0,
                   1); 
    glColor4f(1,1,1,1);
   }  
 }
 
void Draw_Portals()//Рисуем телепорты
 { 
  delta=delta+game_speed*5;//кручение 
  if (delta>360) {delta=0;}  

  //Вход в портал
  if (t1_true==true and
      t1x*grid+x_cam>0-grid and
      t1x*grid+x_cam<win_DX+grid and 
      t1y*grid+y_cam>0-grid and
      t1y*grid+y_cam<win_DY+grid)      
   {
    glBindTexture(GL_TEXTURE_2D,t_texture);
    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    DrawRubberPoly(t1x*grid+x_cam,
                   t1y*grid+y_cam,
                   1.5*grid-3*sin(delta*3.142/180),
                   1.5*grid-3*sin(delta*3.142/180),
                   delta+240,
                   1); 
    glColor4f(0.5,0.5,0,1);
    DrawRubberPoly(t1x*grid+x_cam,
                   t1y*grid+y_cam,
                   1.5*grid-6*sin(delta*3.142/180),
                   1.5*grid-6*sin(delta*3.142/180),
                   2*delta+120,
                   1);
    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    DrawRubberPoly(t1x*grid+x_cam,
                   t1y*grid+y_cam,
                   1.5*grid+9*sin(delta*3.142/180),
                   1.5*grid+9*sin(delta*3.142/180),
                   delta,
                   1);  
    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);  
   }

  //Выход тз портала
  if (t2_true==true and
      t2x*grid+x_cam>0-grid and
      t2x*grid+x_cam<win_DX+grid and 
      t2y*grid+y_cam>0-grid and
      t2y*grid+y_cam<win_DY+grid) 
   { 
    glBindTexture(GL_TEXTURE_2D,t_texture);
    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    DrawRubberPoly(t2x*grid+x_cam,
                   t2y*grid+y_cam,
                   1.5*grid-3*sin(delta*3.142/180),
                   1.5*grid-3*sin(delta*3.142/180),
                   -delta-240,
                   1); 
    glColor4f(0,0,1,1);
    DrawRubberPoly(t2x*grid+x_cam,
                   t2y*grid+y_cam,
                   1.5*grid-6*sin(delta*3.142/180),
                   1.5*grid-6*sin(delta*3.142/180),
                   -2*delta-120,
                   1);
    glColor4f(0,1,0,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    DrawRubberPoly(t2x*grid+x_cam,
                   t2y*grid+y_cam,
                   1.5*grid+9*sin(delta*3.142/180),
                   1.5*grid+9*sin(delta*3.142/180),
                   -delta,
                   1);  
    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
   }
 } 

 void Draw_HUD()//Рисуем ХУД
 {
  //ХУД
  glBindTexture(GL_TEXTURE_2D,other_texture[3]);
  DrawRubberPoly(0,
                 0,
                 win_DX/2,//512
                 win_DY/12,//64
                 0,
                 0); 

  //Значок молекулы
  glBindTexture(GL_TEXTURE_2D,obj_texture[1]);
  DrawRubberPoly(win_DX/37.93,//27,
                 win_DY/30.72,//25
                 win_DX/51.2,//20,
                 win_DY/38.4,//20,
                 0,
                 1);

  //лайв бар
  if (p_live>0)
   {     
    glBindTexture(GL_TEXTURE_2D,other_texture[2]);
    DrawRubberPoly(win_DX/22.756,//45,
                   win_DY/27.429,//28,
                   p_live*(win_DX/102.4/*10*/),
                   win_DY/38.4,
                   0,
                   0); 
   } 

  glColor4f(1,1,1,1);
  //Выводим число очков             
  DrawText(win_DX/64,//16,
           win_DY/21.33,//36,
           win_DX/20.48,//50,//dX
           win_DY/48,//16,//dY
           1,//font_index
           IntToStr(p_score),
           0,//delta
           0);//center                   

  //Выводим номер уровня
  DrawText(win_DX/22.756,//45
           win_DY/76.8,//10
           win_DX/2,//140
           win_DY/48,//16,
           1,//font_index
           "lev="+IntToStr(level_num)+", s="+IntToStr(p_silver)+", g="+IntToStr(p_gold)+
           ", time="+IntToStr(game_timer)+" is (best="+IntToStr(best_time[level_num])+")",
           0,//delta
           0);//center
 }   
