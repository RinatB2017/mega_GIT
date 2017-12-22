//Разработано aureli0,NeonLight_Child,Ph0Eni}{,Zinoviev54,imanavhod-это все я:)
//http://sourceforge.net/projects/swibplex/
//последний билд от 23.12.2009
   
void Player_Start()//место старта колобка
 {
  for(i=0;i< lev_num_max;i++) 
   {
    if (lev[i].unit==1)//1-ый элемент-старт игрока
     {
      p_x=lev[i].x;
      p_y=lev[i].y; 
     }
   }//конец цикла
 }
 
 void Player_Controls_KeyClick()//Управление при однократном нажатии клавиши
 {
  if (keys[SDLK_F1]) {debug_lev_matrix= not debug_lev_matrix;}
  if (keys[SDLK_F2]) {debug_obj_matrix= not debug_obj_matrix;}

  if (keys[SDLK_F3]) //отразить уровень по вертикали
   {
    mirror=true;
    Mirror();
   }

  //останавливаем трек, чтобы музыка переключилась
  if (keys[SDLK_F4]) {stopmusic();}

  if (p_true==true) //Если колобок жив от им можно управлять
   {
    //Направление для подрыва бомбы
    if (keys[SDLK_LEFT]) {p_vector=4;} 
    if (keys[SDLK_RIGHT]) {p_vector=2;} 
    if (keys[SDLK_UP]) {p_vector=1;} 
    if (keys[SDLK_DOWN]) {p_vector=3;}

    if (event.key.keysym.sym==SDLK_v and
        diff_game_mode==2)
     {
      if (p_score>=5) //за 5 очков можем увеличить радиус видимости
       {
        p_score=p_score-5;
        dark_radius=dark_radius+2;
       }
     }
   

    //открываем/закрываем двери когда игрок не движется, жмем кнопки
    if (event.key.keysym.sym==SDLK_b and
        p_move_left==false and//слева
        p_move_right==false and//справа
        p_move_up==false and//сверху
        p_move_down==false and //снизу
        p_face_frame_count>5)//и прошло какоето время с конца движения
     {

     //кнопка поворота уровня по вертикали

      for(j=0; j < lev_num_max; j++) 
       {
        if (lev[j].unit==29 and//либо синяя
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
         {

          if (lev[j].x==p_x and //1 открываем сверху
              lev[j].y==p_y+1 and
              p_vector==3)      
           {
            sound(9);
            mirror=true; 
            lev_button_push[j]=not lev_button_push[j];
            break;
           }    

          if (lev[j].x==p_x and //2 открываем снизу
              lev[j].y==p_y-1 and
              p_vector==1)      
           {
            sound(9);
            mirror=true; 
            lev_button_push[j]=not lev_button_push[j];
            break;
           }       

          if (lev[j].x==p_x-1 and //3 открываем слева
              lev[j].y==p_y and
              p_vector==4)      
           {
            sound(9);
            mirror=true;
            lev_button_push[j]=not lev_button_push[j];
            break;
           }     

          if (lev[j].x==p_x+1 and //4 открываем справа
              lev[j].y==p_y and
              p_vector==2)      
           {
            sound(9);
            mirror=true;
            lev_button_push[j]=not lev_button_push[j];
            break;
           }        
   
         }     
       }//конец цикла

      //обычная дверь 
      for(j=0; j < lev_num_max; j++) 
       {
        if (lev[j].unit==4 and//дверь открыта
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit or
            lev[j].unit==12 and//дверь закрыта
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
         {

          if (lev[j].x==p_x and //1 нажимаем сверху
              lev[j].y==p_y+1 and
              p_vector==3)      
           {
            sound(9);
            lev_door_open[j]= not lev_door_open[j];
            break;  
           }    

          if (lev[j].x==p_x and //2 нажимаем снизу
              lev[j].y==p_y-1 and
              p_vector==1)      
           {
            sound(9);
            lev_door_open[j]= not lev_door_open[j];
            break;  
           }       

          if (lev[j].x==p_x-1 and //3 нажимаем слева
              lev[j].y==p_y and
              p_vector==4)      
           {
            sound(9);
            lev_door_open[j]= not lev_door_open[j];
            break;  
           }     

          if (lev[j].x==p_x+1 and //4 нажимаем справа
              lev[j].y==p_y and
              p_vector==2)      
           {
            sound(9);
            lev_door_open[j]= not lev_door_open[j];
            break;  
           }        
   
         }     
       }//конец цикла

      //светильник
      for(j=0; j < lev_num_max; j++) 
       {
        if (lev[j].unit==31 and//лампочка вкл
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit or
            lev[j].unit==30 and//лампочка выкл
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
         {

          if (lev[j].x==p_x and //1 нажимаем сверху
              lev[j].y==p_y+1 and
              p_vector==3)      
           {
            sound(9);
            lev_light_push[j]= not lev_light_push[j];
            break;  
           }    

          if (lev[j].x==p_x and //2 нажимаем снизу
              lev[j].y==p_y-1 and
              p_vector==1)      
           {
            sound(9);
            lev_light_push[j]= not lev_light_push[j];
            break;  
           }       

          if (lev[j].x==p_x-1 and //3 нажимаем слева
              lev[j].y==p_y and
              p_vector==4)      
           {
            sound(9);
            lev_light_push[j]= not lev_light_push[j];
            break;  
           }     

          if (lev[j].x==p_x+1 and //4 нажимаем справа
              lev[j].y==p_y and
              p_vector==2)      
           {
            sound(9);
            lev_light_push[j]= not lev_light_push[j];
            break;  
           }        
         }//конец условия     
       }//конец цикла

      //если светильник включается или выключается то вокруг него изменяем матрицу тьмы
      Level_Control_Lights();//текстура lev для разного состояния ламп
      if (diff_game_mode==2) //если сложный ночной режим
       {
        Dark_Refresh();
       }

      //серебряная дверь с замком
      for(j=0; j < lev_num_max; j++) 
       {
        if (p_silver>0 and//если есть серебро
            lev[j].unit==20 and//серебряная дверь-дискета
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
         {
          if (lev[j].x==p_x and //1 открываем сверху
              lev[j].y==p_y+1 and
              p_vector==3)      
           {
            sound(10);
            kill_lev(j); 
            p_silver=p_silver-1;
            break;  
           }    

          if (lev[j].x==p_x and //2 открываем снизу
              lev[j].y==p_y-1 and
              p_vector==1)      
           {
            sound(10);
            kill_lev(j); 
            p_silver=p_silver-1;
            break; 
           }       

          if (lev[j].x==p_x-1 and //3 открываем слева
              lev[j].y==p_y and
              p_vector==4)      
           {
            sound(10);
            kill_lev(j); 
            p_silver=p_silver-1;
            break; 
           }     

          if (lev[j].x==p_x+1 and //4 открываем справа
              lev[j].y==p_y and
              p_vector==2)      
           {
            sound(10);
            kill_lev(j); 
            p_silver=p_silver-1;
            break;
           }        

         }     
       }//конец цикла про серебряную дверь

      //золотая дверь с замком
      for(j=0; j < lev_num_max; j++) 
       {
        if (p_gold>0 and//если есть золото
            lev[j].unit==21 and//золотая дверь-дискета
            lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
         {
          if (lev[j].x==p_x and //1 открываем сверху
              lev[j].y==p_y+1 and
              p_vector==3)      
           {
            sound(10);
            kill_lev(j); 
            p_gold=p_gold-1;
            break;  
           }    

          if (lev[j].x==p_x and //2 открываем снизу
              lev[j].y==p_y-1 and
              p_vector==1)      
           {
            sound(10);
            kill_lev(j); 
            p_gold=p_gold-1;
            break; 
           }       

          if (lev[j].x==p_x-1 and //3 открываем слева
              lev[j].y==p_y and
              p_vector==4)      
           {
            sound(10);
            kill_lev(j); 
            p_gold=p_gold-1;
            break; 
           }     

          if (lev[j].x==p_x+1 and //4 открываем справа
              lev[j].y==p_y and
              p_vector==2)      
           {
            sound(10);
            kill_lev(j); 
            p_gold=p_gold-1;
            break;
           }        

         }     
       }//конец цикла про золотую дверь
     }//конец "открываем/закрываем двери"

    //подраываем рядом стоящую бомбу 
    if (event.key.keysym.sym==SDLK_b)
     {
      for(j=0; j < obj_num_max; j++) 
       {
        if (p_score>=10 and //за 10 очков
            obj[j].unit==6 and  //**
            obj_matrix[obj[j].x][obj[j].y]==obj[j].unit and
            obj_dead[j]==0 and //если объект жив
            obj_move[j]==false and
            obj_sleep_count[j]==0) 
         {
          if (obj[j].x==p_x and //1 нюкаем сверху
              obj[j].y==p_y+1 and
              p_vector==3 and
              obj_nuke[j]==false)
           {
            obj_nuke[j]=true;
            obj_nuke_count[j]=0;
            p_score=p_score-10;
            break;  
           }    

          if (obj[j].x==p_x and //2 нюкаем снизу
              obj[j].y==p_y-1 and
              p_vector==1 and
              obj_nuke[j]==false)
           {
            obj_nuke[j]=true;
            obj_nuke_count[j]=0;
            p_score=p_score-10;
            break;  
           }  

          if (obj[j].x==p_x-1 and//3 нюкаем слева
              obj[j].y==p_y and
              p_vector==4 and
              obj_nuke[j]==false)
           {
            obj_nuke[j]=true;
            obj_nuke_count[j]=0;
            p_score=p_score-10;
            break;  
           }    

          if (obj[j].x==p_x+1 and//4 нюкаем справа
              obj[j].y==p_y and
              p_vector==2 and
              obj_nuke[j]==false)
           {
            obj_nuke[j]=true;
            obj_nuke_count[j]=0;
            p_score=p_score-10;
            break;  
           }   

         }     
       }//конец цикла
      
     }//конец "подраываем рядом стоящую бомбу"

   //Установка входа в телепорт
   if ( keys[SDLK_n] ) 
    {
     sound(7);
     t1_true=true;
     t1x=p_x;
     t1y=p_y;   
    } 

   //установка выхода из телепорта
   if ( keys[SDLK_m] ) 
    {
     sound(7);
     t2_true=true;
     t2x=p_x;
     t2y=p_y;  
    } 

  }//конец "Если колобок жив от им можно управлять"
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
    
void Player_Controls_KeyDown()//Управление при постоянном нажатии клавиши
 {
  if (p_true==true) //Если колобок жив от им можно управлять
   {
   //Управляем движением колобка
    if (keys[SDLK_LEFT] and p_move_stage==0)//влево
     {
      if (p_move_left_v==true)//если есть возможность пройти влево
       {
        PlayerMoveLeft();
       } 
     }

    if (keys[SDLK_RIGHT] and p_move_stage==0)//вправо 
     {
      if (p_move_right_v==true)//вправо 
       {
        PlayerMoveRight();
       } 
     } 

    if (keys[SDLK_UP] and p_move_stage==0)//вверх
     {
      if (p_move_up_v==true) 
       {
        PlayerMoveUp();
       }
     }
    
    if (keys[SDLK_DOWN]and p_move_stage==0)//вниз
     {
      if (p_move_down_v==true) 
       {
        PlayerMoveDown();
       } 
     } 

   }//конец "Если колобок жив от им можно управлять"

  //управление камерой
  if ( keys[SDLK_w] ) {y_cam=y_cam+0.15*grid;} 
  if ( keys[SDLK_s] ) {y_cam=y_cam-0.15*grid;} 
  if ( keys[SDLK_a] ) {x_cam=x_cam+0.15*grid;} 
  if ( keys[SDLK_d] ) {x_cam=x_cam-0.15*grid;}

 }

void Player_Render_Move()//Обработка движений колобка
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
      //вверх
      if (p_move_up==true) 
       {
        p_move_y=p_move_y-game_speed*1.4*0.055555*grid;//скорость продвижения между клетками 
       
        if (p_move_y<-grid) //окончание передвижения
         {
          p_y=p_y-1;//после передвижения передвигаемся на новую клетку физически
          p_move_y=0;
          p_move_up=false;

          p_move_stage=0;
         }
       }

      //вниз
      if (p_move_down==true) 
       {
        p_move_y=p_move_y+game_speed*1.4*0.055555*grid;//скорость продвижения между клетками
 
        if (p_move_y>grid) //окончание передвижения
         {
          p_y=p_y+1;//после передвижения передвигаемся на новую клетку физически
          p_move_y=0;
          p_move_down=false;

          p_move_stage=0;
         }
       }
 
      //влево
      if (p_move_left==true) 
       {
        p_move_x=p_move_x-game_speed*1.4*0.055555*grid;//скорость продвижения между клетками  

        if (p_move_x<-grid) //окончание передвижения
         {
          p_x=p_x-1;//после передвижения передвигаемся на новую клетку физически
          p_move_x=0;
          p_move_left=false;

          p_move_stage=0;
         }
       }

      //вправо
      if (p_move_right==true) 
       {
        p_move_x=p_move_x+game_speed*1.4*0.055555*grid;//скорость продвижения между клетками
  
        if (p_move_x>grid) //окончание передвижения
         {
          p_x=p_x+1;//после передвижения передвигаемся на новую клетку физически
          p_move_x=0;
          p_move_right=false;

          p_move_stage=0;
         }
       }

      //анимация движения колобка
      ControlAnimate(&p_frame,//номер текущего кадра
                     p_frame_start,//стартовый кадр
                     p_frame_start+3,//финальный кадр=стратовый + 2 кадра на каждое движение
                     1,//шаг между кадрами
                     &p_frame_count,//переменная отвечающая за счетчик
                     3,//задержка между кадрами в циклах
                     true);//зациклить ли анимацию

     }//конец стадии передвижения 1

    //перемещение игрового экрана-камеры
    if ((p_x*grid+x_cam)>(win_DX-0.3333*win_DX)) {x_cam=x_cam-0.15*grid;}//смещение на 0.15 от grid
    if ((p_x*grid+x_cam)<(0+0.3333*win_DX)) {x_cam=x_cam+0.15*grid;}
    if ((p_y*grid+y_cam)>(win_DY-0.3333*win_DY)) {y_cam=y_cam-0.15*grid;}
    if ((p_y*grid+y_cam)<(0+0.3333*win_DY)) {y_cam=y_cam+0.15*grid;}

  //Ограничение передвижения колобка внутри игрового чистого поля
    if (p_x>lev_width) {p_x=lev_width;} 
    if (p_x<0) {p_x=0;}  
    if (p_y>lev_height) {p_y=lev_height;} 
    if (p_y<0) {p_y=0;}    

   }//конец если "Если колобок жив, то"
 }

void Player_Render_Collision_Lev_Obj()//Обработка столконвений колобка c lev и obj
 {
  if (p_true==true and p_move_stage==0) //Если колобок жив и не движется
   {
    //Проверка возможности перемещения колобка на новую клетку в разные стороны
    p_move_left_v=true;
    p_move_right_v=true;
    p_move_up_v=true;
    p_move_down_v=true;

    //1) Взаимодействие колобка с LEV
    //Если заходим колобком на зеленый элемент lev (=5-11), то стираем его и срем там
    if (lev_matrix[p_x][p_y]>=5 and lev_matrix[p_x][p_y]<=11)
     {
      sound(8);
      for(i=0; i < lev_num_max; i++) 
       {
        if (lev[i].x==p_x and 
            lev[i].y==p_y and
            lev_matrix[lev[i].x][lev[i].y]>=5 and 
            lev_matrix[lev[i].x][lev[i].y]<=11)
         {
          lev_matrix[lev[i].x][lev[i].y]=0;
          lev[i].unit=0;
          p_kal[i]=true;//оставляем кал         
         }
       }  
     }

    //Если заходим колобком на элемент lev (выходы=2,3-по молекулам,по врагам), то выход из уровня
    if (lev_matrix[p_x][p_y]==2 or 
        lev_matrix[p_x][p_y]==3)
     {
      for(i=0; i < lev_num_max; i++) 
       {
        if (lev[i].x==p_x and 
            lev[i].y==p_y and
            lev[i].unit==2 or
            lev[i].x==p_x and 
            lev[i].y==p_y and
            lev[i].unit==3)   
         {
          //выход на следущий уровень
          if (lev[i].unit==2 and //если собрано нужное количество очков
              p_score>=lev[i].exit_score
              or
              lev[i].unit==3 and //если скокото врагов сдохло
              lev[i].exit_kills<=0) //грузим следущий уровень
           {     
            if (game_timer<best_time[level_num])//если время лучше чем в списке заменяем его и сохраняем результат 
             {
              best_time[level_num]=game_timer; //занесение времени в массив
              SaveBestTime();//сохраняем результат
             }
   
            Init_Game();
            //игровой таймер 
            level_num++;
            //если больше максимального значени уровней-потом доделать
            if (level_num>level_max) {level_num=1;}

            Load_Level(level_num);
            Init_Lev_Doors();//инициализация состояния дверей

            Init_Lev_Lights();//инициализация состояния ламп
            Dark_Init();

            Player_Start();
           }
         }
       }//конец цикла
     }//конец условия


    //фильтрация возможностей для матрицы lev (>10)
    if (lev_matrix[p_x-1][p_y]>11) {p_move_left_v=false;} //слева
    if (lev_matrix[p_x+1][p_y]>11) {p_move_right_v=false;} //справа
    if (lev_matrix[p_x][p_y-1]>11) {p_move_up_v=false;} //сверху
    if (lev_matrix[p_x][p_y+1]>11) {p_move_down_v=false;} //снизу

//====ПРОХОДЫ====
    for(j=0; j < lev_num_max; j++) 
     {

//проход вверх ^ 
      if (lev[j].unit==22 and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       {
        if (p_y==lev[j].y+1 and p_x==lev[j].x) {p_move_up_v=true;} //около
        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         {
          p_move_left_v=false;//вылезти из вертикальной трубки нельзя 
          p_move_right_v=false;
          if (obj_matrix[lev[j].x][lev[j].y-1]!=6)
           { 
            if (lev_matrix[lev[j].x][lev[j].y-1]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                lev_matrix[lev[j].x][lev[j].y-1]==22 or// ^
                lev_matrix[lev[j].x][lev[j].y-1]==26 or// || 
                lev_matrix[lev[j].x][lev[j].y-1]==28)  // + 
             {PlayerMoveUp();}
           } 
         }
       }

//проход вниз v
      if (lev[j].unit==23 and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       { 
        if (p_y==lev[j].y-1 and p_x==lev[j].x) {p_move_down_v=true;}//около
        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         { 
          p_move_left_v=false;//вылезти из вертикальной трубки нельзя 
          p_move_right_v=false;
          if (obj_matrix[lev[j].x][lev[j].y+1]!=6)//на выходе
           { 
            if (lev_matrix[lev[j].x][lev[j].y+1]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                lev_matrix[lev[j].x][lev[j].y+1]==23 or// v
                lev_matrix[lev[j].x][lev[j].y+1]==26 or// || 
                lev_matrix[lev[j].x][lev[j].y+1]==28)  // + 
             {PlayerMoveDown();}
           } 
         }
       }

//проход влево <
      if (lev[j].unit==24 and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       {
        if (p_y==lev[j].y and p_x==lev[j].x+1) {p_move_left_v=true;}//около
        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         {
          p_move_up_v=false;//вылезти из горизонтальной трубки нельзя 
          p_move_down_v=false;
          if (obj_matrix[lev[j].x-1][lev[j].y]!=6)//на выходе
           { 
            if (lev_matrix[lev[j].x-1][lev[j].y]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                lev_matrix[lev[j].x-1][lev[j].y]==24 or// <
                lev_matrix[lev[j].x-1][lev[j].y]==27 or// = 
                lev_matrix[lev[j].x-1][lev[j].y]==28)  // + 
             {PlayerMoveLeft();}
           } 
         }
       }

//проход вправо >
      if (lev[j].unit==25 and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       {
        if (p_y==lev[j].y and p_x==lev[j].x-1) {p_move_right_v=true;}//около
        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         {
          p_move_up_v=false;//вылезти из горизонтальной трубки нельзя 
          p_move_down_v=false;
          if (obj_matrix[lev[j].x+1][lev[j].y]!=6)//на выходе
           { 
            if (lev_matrix[lev[j].x+1][lev[j].y]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                lev_matrix[lev[j].x+1][lev[j].y]==25 or// >
                lev_matrix[lev[j].x+1][lev[j].y]==27 or// = 
                lev_matrix[lev[j].x+1][lev[j].y]==28)  // + 
             {PlayerMoveRight();}
           }
         }         
       }

//проход вверх-низ ||
      if (lev[j].unit==26 and
          lev_matrix[lev[j].x][lev[j].y]==lev[j].unit)
       {
        //вверх
        if (p_y==lev[j].y+1 and p_x==lev[j].x) {p_move_up_v=true;}//около снизу-наверх
        //вниз
        if (p_y==lev[j].y-1 and p_x==lev[j].x) {p_move_down_v=true;} //около сверху-вниз

        if (p_y==lev[j].y and p_x==lev[j].x)//внутри 
         {
          p_move_left_v=false;//вылезти из вертикальной трубки нельзя 
          p_move_right_v=false;
          //в зависимости от предыдущего направления движения
          if (p_vector==1) //снизу-наверх
           {
            if (obj_matrix[lev[j].x][lev[j].y-1]!=6)
             { 
              if (lev_matrix[lev[j].x][lev[j].y-1]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x][lev[j].y-1]==22 or// ^
                  lev_matrix[lev[j].x][lev[j].y-1]==26 or// || 
                  lev_matrix[lev[j].x][lev[j].y-1]==28)  // + 
               {PlayerMoveUp();}
             } 
           }

          if (p_vector==3)//сверху-вниз
           {
            if (obj_matrix[lev[j].x][lev[j].y+1]!=6)//на выходе
             { 
              if (lev_matrix[lev[j].x][lev[j].y+1]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x][lev[j].y+1]==23 or// v
                  lev_matrix[lev[j].x][lev[j].y+1]==26 or// || 
                  lev_matrix[lev[j].x][lev[j].y+1]==28)  // + 
               {PlayerMoveDown();}
             } 
           }
         }//конец внутри 
       }//конец ||


//проход влево-вправо =
      if (lev[j].unit==27 and
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
          //в зависимости от предыдущего направления движения          
          if (p_vector==4) //справа-налево
           {
            if (obj_matrix[lev[j].x-1][lev[j].y]!=6)//на выходе
             { 
              if (lev_matrix[lev[j].x-1][lev[j].y]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x-1][lev[j].y]==24 or// <
                  lev_matrix[lev[j].x-1][lev[j].y]==27 or// = 
                  lev_matrix[lev[j].x-1][lev[j].y]==28)  // + 
               {PlayerMoveLeft();}
             } 
           }

          if (p_vector==2) //слево-направо
           {
            if (obj_matrix[lev[j].x+1][lev[j].y]!=6)//на выходе
             { 
              if (lev_matrix[lev[j].x+1][lev[j].y]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x+1][lev[j].y]==25 or// >
                  lev_matrix[lev[j].x+1][lev[j].y]==27 or// = 
                  lev_matrix[lev[j].x+1][lev[j].y]==28)  // + 
               {PlayerMoveRight();}
             }
           }
         }//конец внутри
       }//конец =


//проход крестовина вверх-низ-влево-вправо +
      if (lev[j].unit==28 and
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
          //в зависимости от предыдущего направления движения
          if (p_vector==1) //снизу-наверх
           {
            if (obj_matrix[lev[j].x][lev[j].y-1]!=6)
             { 
              if (lev_matrix[lev[j].x][lev[j].y-1]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x][lev[j].y-1]==22 or// ^
                  lev_matrix[lev[j].x][lev[j].y-1]==26 or// || 
                  lev_matrix[lev[j].x][lev[j].y-1]==28)  // + 
               {PlayerMoveUp();}
             } 
           }

          if (p_vector==3)//сверху-вниз
           {
            if (obj_matrix[lev[j].x][lev[j].y+1]!=6)//на выходе
             { 
              if (lev_matrix[lev[j].x][lev[j].y+1]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x][lev[j].y+1]==23 or// v
                  lev_matrix[lev[j].x][lev[j].y+1]==26 or// || 
                  lev_matrix[lev[j].x][lev[j].y+1]==28)  // + 
               {PlayerMoveDown();}
             } 
           }

          if (p_vector==4) //справа-налево
           {
            if (obj_matrix[lev[j].x-1][lev[j].y]!=6)//на выходе
             { 
              if (lev_matrix[lev[j].x-1][lev[j].y]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x-1][lev[j].y]==24 or// <
                  lev_matrix[lev[j].x-1][lev[j].y]==27 or// = 
                  lev_matrix[lev[j].x-1][lev[j].y]==28)  // + 
               {PlayerMoveLeft();}
             } 
           }

          if (p_vector==2) //слево-направо
           {
            if (obj_matrix[lev[j].x+1][lev[j].y]!=6)//на выходе
             { 
              if (lev_matrix[lev[j].x+1][lev[j].y]<12 or//если есть возможность вылезти на местоназначение - двигаем колобка 
                  lev_matrix[lev[j].x+1][lev[j].y]==25 or// >
                  lev_matrix[lev[j].x+1][lev[j].y]==27 or// = 
                  lev_matrix[lev[j].x+1][lev[j].y]==28)  // + 
               {PlayerMoveRight();}
             }
           }

         }//конец внутри
       }//конец "крестовина" +

     }//конец цикла про проходы


    //2)Взаимодействие колобка с obj, и при этом определяем границы движения колобка 
    //фильтрация возможностей для матрицы obj=6 (только по бомбам не может ходить)
    if (obj_matrix[p_x-1][p_y]==6) {p_move_left_v=false;} //слева //**
    if (obj_matrix[p_x+1][p_y]==6) {p_move_right_v=false;} //справа //**
    if (obj_matrix[p_x][p_y-1]==6) {p_move_up_v=false;} //сверху //**
    if (obj_matrix[p_x][p_y+1]==6) {p_move_down_v=false;} //снизу //**

    //сверху-колобок неможет пройти по бомбам //**
    if (obj_matrix[p_x][p_y-1]==6) {p_move_up_v=false;}
    //снизу-колобок неможет пройти
    if (obj_matrix[p_x][p_y+1]==6) {p_move_down_v=false;}


   }//конец "Если колобок жив, то"

 }
 
void Player_Render_Dead()//Столкновение колобка с бомбой и врагом
 {
  for (j=0;j<obj_num_max;j++)   
  for (k=0;k<eff_num_max;k++)//для начального условие для взрыва
   { 
    if ( p_true==true and //столкновение с врагом и бомбами
        obj_dead[j]==0 and
        obj[j].unit>=6 and
        p_x*grid+p_move_x-0.5*grid<obj[j].x*grid+obj_move_x[j]+0.5*grid and
        p_x*grid+p_move_x+0.5*grid>obj[j].x*grid+obj_move_x[j]-0.5*grid and
        p_y*grid+p_move_y-0.5*grid<obj[j].y*grid+obj_move_y[j]+0.5*grid and
        p_y*grid+p_move_y+0.5*grid>obj[j].y*grid+obj_move_y[j]-0.5*grid and
        eff_true[k]==false)
     {
      sound(11);//крик
      //начальные условия для эффектов
      eff_true[k]=true;
      eff_type[k]=3;//тип фэфекта-кровь
      eff_W[k]=0.8*grid;//размеры эффекта
      eff_H[k]=eff_W[k];
      eff_cR[k]=1;//оттенки цвета эффекта R,G,B
      eff_cG[k]=0;
      eff_cB[k]=0;
      eff_cAlpha[k]=1;//прозрачность эффетка
   
      eff_frame[k]=1;//кадр
      eff_x[k]=p_x*grid+0.8*(rand()%grid+1)-0.8*(rand()%grid+1);//местоположение эффекта
      eff_y[k]=p_y*grid+0.8*(rand()%grid+1)-0.8*(rand()%grid+1);

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

void Player_Eat_Obj()//Колобок ест бонусы (obj=1-6) и собирает при этом очки
 {
  for(j=0; j < obj_num_max; j++) 
   {
    if (p_true==true and//если колобок и объект существуют
        obj_dead[j]==0 and
        obj[j].unit>0 and
        obj[j].unit<6 and//жрем бонусы до 6-го объекта-бомбы не жрем
        p_x*grid+p_move_x-0.44*grid<obj[j].x*grid+obj_move_x[j]+0.44*grid and
        p_x*grid+p_move_x+0.44*grid>obj[j].x*grid+obj_move_x[j]-0.44*grid and
        p_y*grid+p_move_y-0.44*grid<obj[j].y*grid+obj_move_y[j]+0.44*grid and
        p_y*grid+p_move_y+0.44*grid>obj[j].y*grid+obj_move_y[j]-0.44*grid)
     {
      obj_matrix[p_x][p_y]=0;//стираем объект из матрицы объектов
      obj_dead[j]=1;//молния
      if (obj[j].unit==1)//жрем колобком разные объекты 
       {
        sound(2); 
        p_score=p_score+1;//начисляем очки за поедание тонкого пингвина
       }
      if (obj[j].unit==2) 
       {
        sound(3);
        p_score=p_score+5;//начисляем очки за поедание жирного пингвина
       }
      if (obj[j].unit==3) 
       {
        sound(4);
        p_live=p_live+0.3*p_live_max;//поедаем лечилку и лечимся
        if (p_live>=p_live_max)//если жизни много, то небольше максимального значения
         {p_live=p_live_max;}
       }
      if (obj[j].unit==4) 
       {
        sound(5);
        p_silver=p_silver+1;//поедаем серебро
       }
      if (obj[j].unit==5) 
       {
        sound(6);
        p_gold=p_gold+1;//поедаем золото
       }
     }//конец условия
   }//конец цикла
 }
 
void Player_Draw()//Рисуем колобка
 {
  if (p_true==true)//если колобок толкал снимаем метку//если колобок жив
   {      
    
    if (lev_matrix[p_x][p_y]>=22 and lev_matrix[p_x][p_y]<=28)//прозрачен в ПРОХОДАХ
     {glColor4f(1,1,1,0.55);}
    else {glColor4f(1,1,1,1);}

    glBindTexture(GL_TEXTURE_2D,player_texture[p_frame]);
    DrawTXT(p_x*grid+p_move_x+x_cam,
            p_y*grid+p_move_y+y_cam,
            grid,grid,0,1); 
    glColor4f(1,1,1,1);
   }  
 }

void Portals_Draw()//Рисуем телепорты
 { 
  delta=delta+game_speed*5;//кручение 
  if (delta>360) {delta=0;}  

  //вход 
  if (diff_game_mode==1 and //при обычном режиме сложности
      t1_true==true and
      t1x*grid+x_cam>0-grid and
      t1x*grid+x_cam<win_DX+grid and 
      t1y*grid+y_cam>0-grid and
      t1y*grid+y_cam<win_DY+grid or

      diff_game_mode==2 and ////при повышенном режиме сложности
      dark_matrix[t1x][t1y]==0 and
      t1_true==true and
      t1x*grid+x_cam>0-grid and
      t1x*grid+x_cam<win_DX+grid and 
      t1y*grid+y_cam>0-grid and
      t1y*grid+y_cam<win_DY+grid or

      
      diff_game_mode==2 and //при повышенном режиме сложности
      t1_true==true and
      t1x>p_x-dark_radius and
      t1x<p_x+dark_radius and 
      t1y>p_y-dark_radius and
      t1y<p_y+dark_radius)      
   {
    glBindTexture(GL_TEXTURE_2D,t_texture);
    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    DrawTXT(t1x*grid+x_cam,
            t1y*grid+y_cam,
            1.5*grid-3*sin(delta*3.142/180),
            1.5*grid-3*sin(delta*3.142/180),delta+240,1); 
    glColor4f(0.5,0.5,0,1);
    DrawTXT(t1x*grid+x_cam,
            t1y*grid+y_cam,
            1.5*grid-6*sin(delta*3.142/180),
            1.5*grid-6*sin(delta*3.142/180),2*delta+120,1);
    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    DrawTXT(t1x*grid+x_cam,
            t1y*grid+y_cam,
            1.5*grid+9*sin(delta*3.142/180),
            1.5*grid+9*sin(delta*3.142/180),delta,1);  
    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);  
   }

  //выход
  if (diff_game_mode==1 and //при обычном режиме сложности
      t2_true==true and
      t2x*grid+x_cam>0-grid and
      t2x*grid+x_cam<win_DX+grid and 
      t2y*grid+y_cam>0-grid and
      t2y*grid+y_cam<win_DY+grid or

      diff_game_mode==2 and //при повышенном режиме сложности
      dark_matrix[t2x][t2y]==0 and
      t2_true==true and
      t2x*grid+x_cam>0-grid and
      t2x*grid+x_cam<win_DX+grid and 
      t2y*grid+y_cam>0-grid and
      t2y*grid+y_cam<win_DY+grid or

      diff_game_mode==2 and //при повышенном режиме сложности
      t2_true==true and
      t2x>p_x-dark_radius and
      t2x<p_x+dark_radius and 
      t2y>p_y-dark_radius and
      t2y<p_y+dark_radius) 
   { 
    glBindTexture(GL_TEXTURE_2D,t_texture);
    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    DrawTXT(t2x*grid+x_cam,
            t2y*grid+y_cam,
            1.5*grid-3*sin(delta*3.142/180),
            1.5*grid-3*sin(delta*3.142/180),-delta-240,1); 
    glColor4f(0,0,1,1);
    DrawTXT(t2x*grid+x_cam,
            t2y*grid+y_cam,
            1.5*grid-6*sin(delta*3.142/180),
            1.5*grid-6*sin(delta*3.142/180),-2*delta-120,1);
    glColor4f(0,1,0,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE);
    DrawTXT(t2x*grid+x_cam,
            t2y*grid+y_cam,
            1.5*grid+9*sin(delta*3.142/180),
            1.5*grid+9*sin(delta*3.142/180),-delta,1);  
    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA); 
   }
 }
