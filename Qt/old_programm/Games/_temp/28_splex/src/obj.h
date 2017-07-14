//Разработано aureli0,NeonLight_Child,Ph0Eni}{,Zinoviev54,imanavhod-это все я:)
//http://sourceforge.net/projects/swibplex/
//последний билд от 23.12.2009
     
void kill_obj(int j)//уничтожаем элемент объект
 {
  obj_matrix[obj[j].x][obj[j].y]=0;//стираем объект из матрицы объектов
  if (obj_move[j]==true)
   {
    if (obj_move_up[j]==true) {obj_matrix[obj[j].x][obj[j].y-1]=0;}//стираем объект из матрицы объектов
    if (obj_move_down[j]==true) {obj_matrix[obj[j].x][obj[j].y+1]=0;}//стираем объект из матрицы объектов
    if (obj_move_left[j]==true) {obj_matrix[obj[j].x-1][obj[j].y]=0;}//стираем объект из матрицы объектов
    if (obj_move_right[j]==true) {obj_matrix[obj[j].x+1][obj[j].y]=0;}//стираем объект из матрицы объектов
    obj_move[j]=false;
   } 
  obj[j].x=0;//объекта больше нет
  obj[j].y=0;//координаты обнуляем
  obj[j].unit=0;
  
  obj_rotate[j]=0;
  obj_delta[j]=0;
  obj_sigma[j]=0;
 }
 
void Obj_Render_Exit()//если враг заходит на спец выход то дохнет, а очки на выход уменьшаються
 {
  for(i=0; i < lev_num_max; i++) 
  for(j=0; j < obj_num_max; j++)
   {
    if (lev[i].unit==3 and //если враг залетает на 3-ий элемент lev (условие как с телепортом)
        lev[i].exit_kills>0 and
        obj_dead[j]==0 and //если объект жив
        obj[j].unit>=7 and //враг   
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
      obj_dead[j]=3;//объект мертв-там будет кровища
     }//конец условия
   }//конец циклов
 }

void Obj_Render_Teleport()//Взаимодействие объекта с телепортом-попадание в телепорт объекта
 {
  if (t1_true==true and//Если существуют вход и выход из телепорта
      t2_true==true)
   {
    for (j=0; j < obj_num_max; j++) 
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

void Obj_Render_Stage1()//Проверка возможности перемещения объекта
 {
  //Взаимодействие объектов между собой и уровнем
  for(j=0; j < obj_num_max; j++)
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

  for(j=0; j < obj_num_max; j++)
   {
    //если объект был в движении то отсчитываем счетчик до его состояния покоя
    if (obj[j].unit>0 and obj[j].unit<=6 and //для всех объектов кроме врагов
        obj_dead[j]==0 and //если объект жив
        obj_move[j]==false and //и больше не двигается
        obj_sleep_count[j]>0) 
     {obj_sleep_count[j]=obj_sleep_count[j]-1;}//уменьшаем таймер до состояния покоя
   }//конец цикла
 
  //проверка объекта-претендента на падение или перемещение на свободную клетку 
  for(j=0; j < obj_num_max; j++)
  for(k=0; k < obj_num_max; k++)
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
    if (obj[j].unit==6 and//для всех объектов кроме врагов
        obj[k].unit>=7 and
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
      obj_dead[k]=3;//кровища 
     }
   }//конец циклов
 }

void Obj_Render_Stage2()//Взаимодействие объектов типа obj с колобком
 {
  if (p_true==true)//Если колобок существует
   {
    //Тычки объектов колобком 
    p_kick_left=false;
    p_kick_right=false;
 
    for(j=0;j<obj_num_max;j++) 
     {
      if (obj_dead[j]==0)//если объект жив
       { 
        if (obj_move_kick_left_mark[j]==true) {obj_move_kick_left_mark[j]=false;}
        if (obj_move_kick_right_mark[j]==true) {obj_move_kick_right_mark[j]=false;} 
       } 
     }

    //ищем объекты для тычка-бомбы
    for(j=0; j < obj_num_max; j++) 
     {
      //слева
      if (obj_dead[j]==0 and //если объект жив
          obj_move[j]==false and
          obj[j].unit==6 and 
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
          obj[j].unit==6 and
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


    for(j=0; j < obj_num_max; j++)  
     {
      //1. объект на колобке не проваливается
      if (obj_dead[j]==0 and //если объект жив
          obj_move[j]==false and
          obj_move_down_v[j]==true and
          obj_sleep_count[j]==0 and
          obj[j].unit>0 and //бонусы или бомбы
          obj[j].unit<=6 and //**
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
          obj[j].unit<=6 and //**
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
          obj[j].unit<=6 and //**
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
          obj[j].unit<=6 and //**
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
          obj[j].unit<=6 and //**
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

void Obj_collisions(int j) //фильтрация возможных направлений передвижения объекта по матрицам lev и obj 
 {
  //слева
  if (lev_matrix[obj[j].x-1][obj[j].y]>4 or //по стартовым и выходам ходить может
      obj_matrix[obj[j].x-1][obj[j].y]>0) //по объектам - нет 
     {obj_move_left_v[j]=false;}
  //справа
  if (lev_matrix[obj[j].x+1][obj[j].y]>4 or
      obj_matrix[obj[j].x+1][obj[j].y]>0) {obj_move_right_v[j]=false;}
  //сверху
  if (lev_matrix[obj[j].x][obj[j].y-1]>4 or
      obj_matrix[obj[j].x][obj[j].y-1]>0) {obj_move_up_v[j]=false;}
  //снизу
  if (lev_matrix[obj[j].x][obj[j].y+1]>4 or
      obj_matrix[obj[j].x][obj[j].y+1]>0) {obj_move_down_v[j]=false;}

  //сверху слева
  if (lev_matrix[obj[j].x-1][obj[j].y-1]>4 or
      obj_matrix[obj[j].x-1][obj[j].y-1]>0) {obj_move_up_left_v[j]=false;}
  //сверху справа
  if (lev_matrix[obj[j].x+1][obj[j].y-1]>4 or
      obj_matrix[obj[j].x+1][obj[j].y-1]>0) {obj_move_up_right_v[j]=false;}
  //снизу слева
  if (lev_matrix[obj[j].x-1][obj[j].y+1]>4 or
      obj_matrix[obj[j].x-1][obj[j].y+1]>0) {obj_move_down_left_v[j]=false;}
  //снизу справа
  if (lev_matrix[obj[j].x+1][obj[j].y+1]>4 or
      obj_matrix[obj[j].x+1][obj[j].y+1]>0) {obj_move_down_right_v[j]=false;}

  //если объект бомба а под ней враг то падаем на врага
  if (obj[j].unit==6 and
      obj_move[j]==false and
      obj_dead[j]==0 and
      obj_nuke[j]==false and
      obj_matrix[obj[j].x][obj[j].y+1]>=7 and
      obj_sleep_count[j]>0) {obj_move_down_v[j]=true;}
 }

void Obj_Render_Stage3()//Старт движения и псевдо АИ(поведение) объектов
 { 
  for(j=0; j < obj_num_max; j++)
   {
    if (obj[j].unit>0) {Obj_collisions(j);}//ограничение по матрицам для всех объектов

    //Реализация тычка при движении объект влево
    if (p_kick_left==true and//если тычок задан
        obj[j].unit==6 and//для бомб
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
        obj[j].unit==6 and//для бомб
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

      obj_rotate[j]=1;//крутится при движени

      obj_move_kick_right_mark[j]=false;//снимаем метку
      p_kick_right=false;//тычок проделан
     }

//Не скатываемость объекта
    if (j!=obj_kick_index and
        obj[j].unit>0 and obj[j].unit<=6 and //для всех объектов кроме врагов
        obj_dead[j]==0 and //если объект жив
        obj_move[j]==false) //и больше не двигается
     {
      //Объект не скатывается если стоит
      if (lev_matrix[obj[j].x][obj[j].y+1]>=5 and//на плате
          lev_matrix[obj[j].x][obj[j].y+1]<=11 
          or 
          obj[j].x==p_x and //или на колобке
          obj[j].y==p_y-1)
       {
        obj_move_left_v[j]=false;
        obj_move_right_v[j]=false;
        obj_move_down_left_v[j]=false;
        obj_move_down_right_v[j]=false;
       }

      //если есть движущийся объектты то те что между ними не могут двигаться-ваще это бред но работает
      for(k=0; k < obj_num_max; k++)
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
       
     }

    //для obj=1-6-бонусы и бомбы
    if (obj[j].unit<=6 and 
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
    if (obj[j].unit==7 and
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

        if (p_x>obj[j].x-6 and p_x<obj[j].x+6 and //если игрок поблиизости
            p_y>obj[j].y-6 and p_y<obj[j].y+6)
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
    if (obj[j].unit==8 and
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
    if (obj[j].unit==9 and
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
 }

void Obj_Render_Stage4()//Анимация и движение объектов
 {
  for(j=0; j < obj_num_max; j++)
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
          if (obj[j].unit==6 and
              obj_matrix[obj[j].x][obj[j].y+1]>0 or
              obj[j].unit==6 and
              lev_matrix[obj[j].x][obj[j].y+1]>4) 
           {
            sound(12);
           }

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
        if (obj[j].unit==7)//ножницы
         {

          ControlAnimate(&obj_frame[j],//номер текущего кадра
                         1,//стартовый кадр
                         7,//финальный кадр
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
 
void Obj_Render_Dead()//Смерть объектов
 {
  //если объект умер(подобран) там эффект
  for (j=0;j<obj_num_max;j++)   
  for (k=0;k<eff_num_max;k++)//начальные условие
   { 
    if (obj[j].unit>0 and 
        obj_dead[j]>0 and//если объект умер
        eff_true[k]==false) 
     {
//молния
      if (obj_dead[j]==1)
       {     
        eff_true[k]=true;//параметры эффетка
        eff_type[k]=obj_dead[j];//молния
        eff_W[k]=2*rand()%grid+2*grid;
        eff_H[k]=eff_W[k];
        eff_cR[k]=1;
        eff_cG[k]=1;
        eff_cB[k]=1;
        eff_cAlpha[k]=1;  

        eff_frame[k]=1;
        eff_x[k]=obj[j].x*grid+obj_move_x[j];
        eff_y[k]=obj[j].y*grid+obj_move_y[j];
       }
//взрыв
      if (obj_dead[j]==2)
       { 
        eff_true[k]=true;
        eff_type[k]=obj_dead[j];//взрыв
        eff_W[k]=5*rand()%grid+5*grid;
        eff_H[k]=eff_W[k];
        eff_cR[k]=1;
        eff_cG[k]=1;
        eff_cB[k]=1;
        eff_cAlpha[k]=1;  
   
        eff_frame[k]=1;
        eff_x[k]=obj[j].x*grid+obj_move_x[j];
        eff_y[k]=obj[j].y*grid+obj_move_y[j];
       } 
//кровища
      if (obj_dead[j]==3)
       {
        sound(11);
        eff_true[k]=true;
        eff_type[k]=3;//кровища
        eff_W[k]=2*grid;/*2*rand()%grid+2*grid;*/
        eff_H[k]=eff_W[k];
        eff_cR[k]=1;
        eff_cG[k]=1;
        eff_cB[k]=1;
        eff_cAlpha[k]=1;  
   
        eff_frame[k]=1;
        eff_x[k]=obj[j].x*grid+obj_move_x[j];
        eff_y[k]=obj[j].y*grid+obj_move_y[j];
       }

      obj_dead[j]=0;//объекта больше не существует
      kill_obj(j);
     }//конец условия

   }//конец циклов 

  //при взрыве бомбы или врага повреждаем соседей
  for(j=0; j < obj_num_max; j++)
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
 
        obj_dead[j]=2;//взрыв

        //1.подрываем бомбы и врагов в округе
        for( k=0; k < obj_num_max; k++)
         {      
          if (obj_dead[k]==0 and//бомбы
              obj[k].unit==6 and
              obj_sleep_count[k]==0//в спокойном состоянии 
              or
              obj[k].unit>=7 and//враг //**
              obj_dead[k]==0) 
           {   
            //радиус взрыва по 8 направлениям по часовой
            //812
            //703
            //654

            //1
            if (obj[k].x==obj[j].x and
                obj[k].y==obj[j].y-1 and
                obj_matrix[obj[k].x][obj[k].y]==obj[k].unit and 
                obj_nuke[k]==false)
             {
              obj_nuke[k]=true;
              obj_nuke_count[k]=0;
             } 

            //2
            if (obj[k].x==obj[j].x+1 and
                obj[k].y==obj[j].y-1 and
                obj_matrix[obj[k].x][obj[k].y]==obj[k].unit and 
                obj_nuke[k]==false)
             {
              obj_nuke[k]=true;
              obj_nuke_count[k]=0;
             } 

            //3
            if (obj[k].x==obj[j].x+1 and
                obj[k].y==obj[j].y and
                obj_matrix[obj[k].x][obj[k].y]==obj[k].unit and 
                obj_nuke[k]==false)
             {
              obj_nuke[k]=true;
              obj_nuke_count[k]=0;
             } 

            //4
            if (obj[k].x==obj[j].x+1 and
                  obj[k].y==obj[j].y+1 and
                  obj_matrix[obj[k].x][obj[k].y]==obj[k].unit and 
                  obj_nuke[k]==false)
               {
                obj_nuke[k]=true;
                obj_nuke_count[k]=0;
               } 

            //5
            if (obj[k].x==obj[j].x and
                obj[k].y==obj[j].y+1 and
                obj_matrix[obj[k].x][obj[k].y]==obj[k].unit and 
                obj_nuke[k]==false)
             {
              obj_nuke[k]=true;
              obj_nuke_count[k]=0;
             } 

            //6
            if (obj[k].x==obj[j].x-1 and
                obj[k].y==obj[j].y+1 and
                obj_matrix[obj[k].x][obj[k].y]==obj[k].unit and 
                obj_nuke[k]==false)
             {
              obj_nuke[k]=true;
              obj_nuke_count[k]=0;
             } 

            //7
            if (obj[k].x==obj[j].x-1 and
                obj[k].y==obj[j].y and
                obj_matrix[obj[k].x][obj[k].y]==obj[k].unit and 
                obj_nuke[k]==false)
             {
              obj_nuke[k]=true;
              obj_nuke_count[k]=0;
             } 

            //8
            if (obj[k].x==obj[j].x-1 and
                obj[k].y==obj[j].y-1 and
                obj_matrix[obj[k].x][obj[k].y]==obj[k].unit and 
                obj_nuke[k]==false)
             {
              obj_nuke[k]=true;
              obj_nuke_count[k]=0;
             }
           
           }//конец условия
         }//конец цикла k 

        //2.подрываем процессоры и схемы-элементы уровня
        for(l=0;l<lev_num_max;l++) 
         {
          if (lev[l].unit>=5 and//схемы
              lev[l].unit<=11 or
              lev[l].unit>=15 and//процессоры
              lev[l].unit<=19)
           {  
            //радиус взрыва по 8 направлениям по часовой
            //812
            //703
            //654   

            //1
            if (lev[l].x==obj[j].x and 
                lev[l].y==obj[j].y-1 and
                lev_matrix[lev[l].x][lev[l].y]==lev[l].unit or
            //2
                lev[l].x==obj[j].x+1 and 
                lev[l].y==obj[j].y-1 and
                lev_matrix[lev[l].x][lev[l].y]==lev[l].unit or
            //3 
                lev[l].x==obj[j].x+1 and 
                lev[l].y==obj[j].y and
                lev_matrix[lev[l].x][lev[l].y]==lev[l].unit or
            //4  
                lev[l].x==obj[j].x+1 and 
                lev[l].y==obj[j].y+1 and
                lev_matrix[lev[l].x][lev[l].y]==lev[l].unit or
            //5 
                lev[l].x==obj[j].x and 
                lev[l].y==obj[j].y+1 and
                lev_matrix[lev[l].x][lev[l].y]==lev[l].unit or
            //6
                lev[l].x==obj[j].x-1 and 
                lev[l].y==obj[j].y+1 and
                lev_matrix[lev[l].x][lev[l].y]==lev[l].unit or
            //7
                lev[l].x==obj[j].x-1 and 
                lev[l].y==obj[j].y and
                lev_matrix[lev[l].x][lev[l].y]==lev[l].unit or
            //8
                lev[l].x==obj[j].x-1 and 
                lev[l].y==obj[j].y-1 and
                lev_matrix[lev[l].x][lev[l].y]==lev[l].unit)
             {
              if (lev[l].unit>=5 and//схемы
                  lev[l].unit<=11) 
               {
                lev[l].unit=0;
                p_kal[l]=true;
                lev_matrix[obj[l].x][obj[l].y]=0;
               }
              if (lev[l].unit>=15 and//процессоры
                  lev[l].unit<=19)
               {kill_lev(l);}
             }

           }//конец условия  
         }//конец цикла l
      
//раним колобка при взрыве,если он поблизости
            if (p_x==obj[j].x and 
                p_y==obj[j].y-1 or
            //2
                p_x==obj[j].x+1 and 
                p_y==obj[j].y-1 or
            //3 
                p_x==obj[j].x+1 and 
                p_y==obj[j].y or
            //4  
                p_x==obj[j].x+1 and 
                p_y==obj[j].y+1 or
            //5 
                p_x==obj[j].x and 
                p_y==obj[j].y+1 or             
            //6
                p_x==obj[j].x-1 and 
                p_y==obj[j].y+1 or
            //7
                p_x==obj[j].x-1 and 
                p_y==obj[j].y or
            //8
                p_x==obj[j].x-1 and 
                p_y==obj[j].y-1)
             {
              p_live=p_live-0.3*p_live_max;
             }

       }//конец условия "если объект взорвался"
 
     }//конец условия "объект до взрыва жив"  
   }//коне цикла j
 } 
 
void Obj_Draw()//Рисуем объекты
 {
  for(j=0; j < obj_num_max; j++) 
   {
    //если элемент уровня существует(жив) и попадает в окно вывода-выводим его
    if (game_show==1 and//в игре
        diff_game_mode==1 and //обычный режим сложности
        obj_dead[j]==0 and 
        obj[j].unit>0 and
        obj[j].x*grid+x_cam>0-grid and
        obj[j].x*grid+x_cam<win_DX+grid and 
        obj[j].y*grid+y_cam>0-grid and
        obj[j].y*grid+y_cam<win_DY+grid or

        game_show==1 and//в игре
        diff_game_mode==2 and //повышеный режим сложности
        dark_matrix[obj[j].x][obj[j].y]==0 and
        obj_dead[j]==0 and 
        obj[j].unit>0 and
        obj[j].x*grid+x_cam>0-grid and
        obj[j].x*grid+x_cam<win_DX+grid and 
        obj[j].y*grid+y_cam>0-grid and
        obj[j].y*grid+y_cam<win_DY+grid or

        game_show==1 and//в игре
        diff_game_mode==2 and //повышеный режим сложности
        obj_dead[j]==0 and 
        obj[j].unit>0 and
        obj[j].x>p_x-dark_radius and
        obj[j].x<p_x+dark_radius and 
        obj[j].y>p_y-dark_radius and
        obj[j].y<p_y+dark_radius or

        edit_show==1 and//в редакторе
        obj_dead[j]==0 and 
        obj[j].unit>0 and
        obj[j].x*grid+x_cam>0-grid and
        obj[j].x*grid+x_cam<win_DX+grid and 
        obj[j].y*grid+y_cam>0-grid and
        obj[j].y*grid+y_cam<win_DY+grid) 
     {
      if (obj_rotate[j]==2)//если объект крутится все время
       {
        obj_delta[j]=obj_delta[j]-game_speed*0.07*360;//объект катится
        if (obj_delta[j]<0) {obj_delta[j]=360;}
       }

      if (obj_rotate[j]==3)//если объект крутится все время
       {
        obj_delta[j]=obj_delta[j]+game_speed*0.01*360;//объект катится
        if (obj_delta[j]>360) {obj_delta[j]=0;}
       }

      if (obj_anim[j]==false) {glBindTexture(GL_TEXTURE_2D,obj_texture[obj[j].unit]);}
      if (obj_anim[j]==true) {glBindTexture(GL_TEXTURE_2D,obj_anim_texture[obj_frame[j]]);}
      DrawTXT(obj[j].x*grid+obj_move_x[j]+x_cam,
              obj[j].y*grid+obj_move_y[j]+y_cam,
              grid+obj_sigma[j],
              grid+obj_sigma[j],
              obj_delta[j],1); //градусы
     }//конец условия
   }//конец цикла j

  //сетка для проверки матрицы объектов 
  if (debug_lev_matrix==true)
   {
    for(j=0;j<lev_width+1;j++)
    for(k=0;k<lev_height+1;k++)
     {
      if (lev_matrix[j][k]==0)
       {
        glBindTexture(GL_TEXTURE_2D,lev_texture[1]);
        glColor4f(1,1,1,0.6);
        DrawTXT(j*grid+x_cam,
                k*grid+y_cam,0.6*grid,0.6*grid,0,1); 
        glColor4f(1,1,1,1);
       }
     }
   } 

  //сетка для проверки матрицы объектов 
  if (debug_obj_matrix==true)
   {
    for(j=0;j<lev_width+1;j++)
    for(k=0;k<lev_height+1;k++)
     {
      if (obj_matrix[j][k]==0)
       {
        glBindTexture(GL_TEXTURE_2D,obj_texture[1]);
        glColor4f(1,1,1,0.6);
        DrawTXT(j*grid+x_cam,
                k*grid+y_cam,0.6*grid,0.6*grid,0,1); 
        glColor4f(1,1,1,1);
       }
     }
   } 
 
 }
