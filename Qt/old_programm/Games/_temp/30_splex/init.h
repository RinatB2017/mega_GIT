//http://swibplex.sourceforge.com

//Главные. Константы
const int //456
          //3*7
          //218	  
          
          index_DOWN=3,
          //index_DOWN_LEFT=2,
          index_LEFT=2,
          //index_UP_LEFT=4,
          index_UP=1,
          //index_UP_RIGHT=6,
          index_RIGHT=4,
          //index_DOWN_RIGHT=8,
          
          index_game_mode_easy=1,
          index_game_mode_hard=2,
          
          index_kal=0,
          index_exit_score=2,
          index_exit_kills=3,
          
          index_start_pos=1,
          index_exit_pos1=2,
          index_exit_pos2=3,
          index_door_open=4,
          index_motherboard1=5,
          index_motherboard2=6,
          index_motherboard3=7,
          index_motherboard4=8,
          index_motherboard5=9,
          index_motherboard6=10,
          index_motherboard7=11,
          index_door_close=12,
          
          index_processors1=15,
          index_processors2=16,
          index_processors3=17,
          index_processors4=18,
          index_processors5=19,
          
          index_siver_door=20,
          index_gold_door=21,
          
          index_tube_UP=22,
          index_tube_DOWN=23,
          index_tube_LEFT=24,
          index_tube_RIGHT=25,
          index_tube_UPDOWN=26,
          index_tube_LEFTRIGHT=27,
          index_tube_CROSS=28,
          
          
          index_button_push=29,
          index_lamp_off=30,
          index_lamp_on=31,
          
          index_rotate_loop_A=2,
          index_rotate_loop_B=3,
          
          
          index_penguin=1,
          index_big_penguin=2,
          index_health=3,
          index_silver_key=4,
          index_gold_key=5,
          
          
          index_bomb=6,
          index_enemy1=7,//ножницы
          index_enemy2=8,//рожа
          index_enemy3=9,//загагулина
         
          index_dead_light=1,
          index_dead_boom=2,
          index_dead_blood=3;//смерть с кровищей
              
float dark_volume=0.01;          

string name="txt";
bool input_true=true;

//Главные. Переменные
int done=0,//переменная отвечающая за окончание Великого рендера
    pre_done=0,//перед выходом что сделать
    
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

    tmp_win_DX,//тоже самое что и раньше, только для опций
    tmp_win_DY,
    tmp_pal,
    tmp_full_screen,
    show_fps,

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

    info_init,//для инфы
    info_show,
    info_return,
    info_exit,

    level_num,//текущий номер уровня  
    diff_game_mode,//игровая сложность
    
    game_now,//таймер для игры
    game_then,
    game_timer;

float x_cam=0,//камера
      y_cam=0;  

//Раздел констант
const
 int grid_X_MAX=59,//размеры уровня по X//
     grid_Y_MAX=23,//размеры уровня по Y//
     grid_DX=32,
     grid_DY=32,

     lev_max=grid_X_MAX*grid_Y_MAX,//максимальное количество элементов уровня
     obj_max=grid_X_MAX*grid_Y_MAX,//максимальное количество объектов
     
     level_num_max=999,//максимальное количество уровней
     eff_num_max=99,//максимальное количество эффектов
     start_time=2;//через это время появится меню, в секундах
     
//Переменные колобка
int p_x,//координаты колобка
    p_y, 
    p_x_tmp,
    p_y_tmp,
    p_move_stage,//стадия управления движением колобка
    p_vector,//вектор направления взгляда при действии-взпыв или открытие двери
    p_frame,//кадр колобка
    p_frame_start,//стартовый кадр анимации
    p_frame_count,//временной счетчик между кадрами анимации
    p_face_frame_count,//временной счетчик для лица
    p_score,//количесвто очков
    p_silver,//ключи-серебряные и золотые
    p_gold,
    p_kal[lev_max];//остатки от печатных плат (1-го типа lev[]) 

float p_move_x,//смещение при движении колобка по OX
      p_move_y,//смещение при движении колобка по OY
      p_live,
      p_live_max;//максимальное количесвто жизней

bool p_move_left,//движется влево
     p_move_right,
     p_move_up,   
     p_move_down,
  
     p_move_left_v,//может ли двигаьтся влево 
     p_move_right_v,     
     p_move_up_v,     
     p_move_down_v, 
     p_kick_left, 
     p_kick_right,
     p_true;//определяет жив ли колобок

//Телепорт как в portal
int t1x,t1y,//координаты входа в телепорт
    t2x,t2y,//координаты выхода из телепорта
    t1y_tmp,
    t2y_tmp; 

bool t1_true,t2_true,//определяет есть ли телепорты
     mirror,//перевернут ли уровень
     mirror_true,//состояние перевернутости уровня
     lev_door_open[lev_max],//открыта ли дверь
     lev_button_push[lev_max],//нажат ли объект-кнопка
     lev_light_push[lev_max];//включен ли светильник

//Переменные динамических (которые могут перемещаться) объектов
int obj_enemy1_radius=2,//радиус чуйки врагов - должно зависеть от сложности игры
    max_explode_radius=1,//максимальный радиус взрыва
    cost_explode=10,//стоимость взрыва бомбы
    cost_viewradius=5;
    
float obj_sigma[obj_max],//графическое увеличение объекта
      obj_move_x[obj_max],//смещение
      obj_move_y[obj_max],
      obj_delta[obj_max];//угол поворота относительно центра
float obj_d1[obj_max],
      obj_d2[obj_max];

int obj_index[obj_max],//индекс объекта      
    obj_rotate[obj_max],//определяет тип кручения объекта 1 - при движении, 2- все время
    obj_AI_param1[obj_max],//для перемещения объекта - псевдо АИ
    obj_AI_param2[obj_max],
    obj_sleep_count[obj_max],//таймер покоя, после падения = 5,затем вычитается до 0
    obj_nuke_index,//индекс взрывающегося объекта
    obj_nuke_count[obj_max],//счетчик до взрыва объекта
    obj_dead[obj_max],//мертв ли объект и с каким эффектом он сдохнет
    obj_frame[obj_max],//текущий кадр анимации
    obj_frame_count[obj_max],//счетчик задержки между кадрами
    obj_angle_finish[obj_max],//финальный угол поворота
    obj_angle_count[obj_max],
    obj_napr[obj_max],
    obj_kick_index;//индекс пихнутого объекта  

bool obj_move[obj_max],//объект движется
     obj_move_up[obj_max],
     obj_move_down[obj_max],
     obj_move_left[obj_max],
     obj_move_right[obj_max],
     obj_move_up_v[obj_max],//объект может двинутся вверх
     obj_move_down_v[obj_max],
     obj_move_left_v[obj_max],
     obj_move_right_v[obj_max],
     
     //для проверок на сваливаемость для элементов OBJ 1,2
     obj_move_up_left_v[obj_max],//вверх-влево
     obj_move_up_right_v[obj_max],//вверх-вправо
     obj_move_down_left_v[obj_max],//вниз-влево
     obj_move_down_right_v[obj_max],//вниз-вправо
     
     obj_move_kick_left_mark[obj_max],//отмечен для тычка слева
     obj_move_kick_right_mark[obj_max],//отмечен для тычка справа
     obj_nuke[obj_max],//объект на подрыве
     obj_anim[obj_max];//есть ли анимация в объекте

//Освещение
int dark_radius,//радиус света вокруг Свиборга
    lev_dark_radius;//радиус света вокгру фонарей уровня

//Сетка уровня
struct lev_type 
 { 
  int x,
      y,
      unit,
      exit_score,//очки на выход от бонусов
      exit_kills;//очки на выход от врагов
 }
lev[lev_max],
lev_tmp[lev_max];//промежуточная сетка для зеркала

struct obj_type //сетка объектов
 { 
  int x,
      y,
      unit;
 }
obj[obj_max],
obj_tmp[obj_max];//промежуточная сетка для зеркала

int grid,//размер ячейки сетки уровня
    old_grid,
    delta;//для телепортов и курсоров

bool debug_lev_matrix=false,
     debug_obj_matrix=false;

//Лайтмап. Константы
const int lmap_max=99;

//Лайтмап. Переменные
struct light_type
 {       
  int x,//координаты расположения в сетке
      y,
      
      light_radius,//радиус светимости
      force;//сила источника, 100% - это максимум
     
  int R,//цветовой оттенок,%
      G,
      B,
      A;//прозрачность,%
  
  int undead,//если больше 0, то существует
      num;
 }
lmap[lmap_max];
//lmap_tmp[lmap_max]   

//Переменные для проверки некоторых коллизий
int lev_matrix[grid_X_MAX+1][grid_Y_MAX+1],//матрица уровня
    obj_matrix[grid_X_MAX+1][grid_Y_MAX+1],//матрица объектов
    lmap_matrix[grid_X_MAX+1][grid_Y_MAX+1],//матрица освещения

//Временные переменные для отзеркаливания
    lev_matrix_tmp[grid_X_MAX+1][grid_Y_MAX+1],//матрица уровня
    obj_matrix_tmp[grid_X_MAX+1][grid_Y_MAX+1],//матрица объектов
    lmap_matrix_tmp[grid_X_MAX+1][grid_Y_MAX+1];//матрица освещения

//Лайтмап. Процедуры
void Kill_LmapOne(int i)
 {
  lmap_matrix[lmap[i].x][lmap[i].y]=0;
  lmap[i].x=0;//координаты расположения в сетке
  lmap[i].y=0;
  lmap[i].light_radius=0;//радиус светимости
  lmap[i].force=0;//сила
             
  lmap[i].R=0;//цветовой оттенок,%
  lmap[i].G=0;
  lmap[i].B=0;
  lmap[i].A=0;//прозрачность,%
  
  lmap[i].undead=0;
  lmap[i].num=0;
 }
 
void Init_Lmap()
 {
  for(int i=0;i<=lmap_max;i++)
   {
	Kill_LmapOne(i);
   }
 } 

void Summon_LmapOne(int i, int x, int y)
 {
  lmap[i].x=x;
  lmap[i].y=y;
  lmap[i].light_radius=6;//4+random(4);//радиус светимости
  lmap[i].force=100;//50+random(50);//сила
             
  lmap[i].R=30+random(70);//цветовой оттенок,%
  lmap[i].G=30+random(70);
  lmap[i].B=30+random(70);
  lmap[i].A=100;//прозрачность,%
    
  lmap[i].undead=1;//???
  lmap[i].num=i;
  
  lmap_matrix[lmap[i].x][lmap[i].y]=lmap[i].num;
 }

void Clear_OneLmapPos(int i)
 {
  lmap_matrix[lmap[i].x][lmap[i].y]=0;
  lmap[i].x=0;
  lmap[i].y=0;
 } 
  
void Render_OneLmapPos(int i, int x, int y)
 {
  lmap[i].x=x;
  lmap[i].y=y;
  lmap_matrix[lmap[i].x][lmap[i].y]=lmap[i].num;
 } 	 

//Сброс параметроа игрока
void Reset_Player()
 {
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
  
  for(int i=0;i<lev_max;i++) 
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

  //Начальные параметры телепорта
  t1x=0;
  t1y=0;
  t1y_tmp=t1y;
  t2x=0;
  t2y=0;//координаты входа и выхода из телепорта
  t2y_tmp=t2y;
  t1_true=false;
  t2_true=false;//определяет есть ли телепорты	  
 }

void Kill_OneLev(int i)//уничтожить элемент уровня
 {
  lev_matrix[lev[i].x][lev[i].y]=0;
  lev[i].x=0;
  lev[i].y=0;
  lev[i].unit=0;

  lev[i].exit_score=0;
  lev[i].exit_kills=0;
 }

void Kill_OneObj(int j)//уничтожаем элемент объект
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
 }

//Сброс переменных элементов уровня
void Reset_Lev()
 {
  for(int j=0; j<lev_max; j++) 
   {
	Kill_OneLev(j);
   }	 
 }
 
//Сброс параметров переменных динамических объектов
void Reset_Obj()
 {
  for(int j=0; j<obj_max; j++) 
   {	
	Kill_OneObj(j);
   } 
 }

//Массив лучшего времени для каждого из пройденных этапов
int best_time[level_num_max];//+1
string level_name[level_num_max];//массив имен уровней +1

//Переменные видео эффектов
int eff_x[eff_num_max],//кординаты
    eff_y[eff_num_max],
    eff_type[eff_num_max],//тип эфеектов взрыв,молния, и т.д.
    eff_frame_count[eff_num_max],//счетчик задержки между кадрами анимации
    eff_frame[eff_num_max];//номер кадра

float eff_W[eff_num_max],
      eff_H[eff_num_max],
      eff_cR[eff_num_max],//цвета RGB, прозрачность альфа 
      eff_cG[eff_num_max],
      eff_cB[eff_num_max],
      eff_cAlpha[eff_num_max];

bool eff_true[eff_num_max];//определяет существеут ли эффект

FILE *fp;//для записи и загрузки уровня

//Текстуры
GLuint player_texture[20],//хэндл текстур колобка
       obj_texture[32],//текстуры объектов
       obj_anim_texture[32],//для объектов с анимацией 
       t_texture,//текстура портала
       lev_texture[32],//текстуры уровня
       other_texture[10],//остальные текстуры
       eff_texture[99][999],//текстуры эффектов
       cursor_TX,//текстура курсора для редактора и кнопок меню
       logo_back_TX,//задний фон логотипа
       game_back_TX;//задний фон игры
       

//============
//=[РЕДАКТОР]=
//============
int edit_mode,//режим редактирования
    score_for_exit,//очки выхода
    num_texture,//порядковый номер текстуры
    x_cursor,
    y_cursor;     

const int index_button_level_down=1,//кнопки основного меню
          index_button_level_up=2,
          index_button_new_game=3,
          index_button_edit=4,
          index_button_exit=5,
          index_button_resume=6,//кнопки внутри игрового меню
          index_button_return=7,
          index_rbutton_delay=8,
          index_rbutton_music_vol=9,
          index_rbutton_sound_vol=10;


void Init_Buttons()
 {
  InitButton(index_button_level_down,//номер кнопки - уровень+
             "Level DOWN",//имя кнопки
             "data/pic/ui.png",//адрес картинки

             2048,//W
             1024,//H
             610,//t_x
             0,//t_y
             72,//t_dx
             75,//t_dy
             -100,//X
             -100,//Y
             0.5*win_DY/(10.67),//72 - размеры
             0.5*win_DY/(10.24),//75
             0,//anim
             0,//anim_horiz
             0,//anim_vert
             0,//start
             0,//finish
             0,//delta
             0,//delay
             0,//loop

             true);//on screen

  InitButton(index_button_level_up,//номер кнопки - уровень+
             "Level UP",//имя кнопки
             "data/pic/ui.png",//адрес картинки

             2048,//W
             1024,//H
             682,//t_x
             0,//t_y
             72,//t_dx
             75,//t_dy
             -100,//X
             -100,//Y
             0.5*win_DY/(10.67),//72 - размеры
             0.5*win_DY/(10.24),//75
             0,//anim
             0,//anim_horiz
             0,//anim_vert
             0,//start
             0,//finish
             0,//delta
             0,//delay
             0,//loop

             true);//on screen

  InitButton(index_button_new_game,//номер кнопки - новая игра
             "New Game",//имя кнопки
             "data/pic/ui.png",//адрес картинки

             2048,//W
             1024,//H
             0,//t_x
             0,//t_y
             600,//t_dx
             112,//t_dy
             -100,//X
             -100,//Y
             0.5*win_DY/(1.28),//600 - размеры
             0.5*win_DY/(6.86),//112
             0,//anim
             0,//anim_horiz
             0,//anim_vert
             0,//start
             0,//finish
             0,//delta
             0,//delay
             0,//loop

             true);//on screen

  InitButton(index_button_edit,//номер кнопки - редактор
             "Edit",//имя кнопки
             "data/pic/ui.png",//адрес картинки

             2048,//W
             1024,//H
             0,//t_x
             112,//t_y
             600,//t_dx
             112,//t_dy
             -100,//X
             -100,//Y
             0.5*win_DY/(1.28),//600 - размеры
             0.5*win_DY/(6.86),//112
             0,//anim
             0,//anim_horiz
             0,//anim_vert
             0,//start
             0,//finish
             0,//delta
             0,//delay
             0,//loop

             true);//on screen

  InitRadioButton(index_rbutton_delay,//номер кнопки поля,
                  delay,
                  "Delay",
                  "data/pic/ui.png",//текстура
                  10,//количество сегментов выбора
                  2,//горизонтальная кнопка

                  2048,//W
                  1024,//H
                  0,//t_x
                  784,//t_y
                  600,//t_dx
                  112,//t_dy
                  -100,//X
                  -100,//Y
                  0.5*win_DY/(1.28),//600 - размеры
                  0.5*win_DY/(6.86),//112
                  true);

  InitRadioButton(index_rbutton_music_vol,//номер кнопки поля,
                  music_volume,
                  "Music_volume",
                  "data/pic/ui.png",//текстура
                  10,//количество сегментов выбора
                  2,//горизонтальная кнопка

                  2048,//W
                  1024,//H
                  0,//t_x
                  672,//t_y
                  600,//t_dx
                  112,//t_dy
                  -100,//X
                  -100,//Y
                  0.5*win_DY/(1.28),//600 - размеры
                  0.5*win_DY/(6.86),//112
                  true);

  InitRadioButton(index_rbutton_sound_vol,//номер кнопки поля,
                  sound_volume,
                  "Sound_volume",
                  "data/pic/ui.png",//текстура
                  10,//количество сегментов выбора
                  2,//горизонтальная кнопка

                  2048,//W
                  1024,//H
                  0,//t_x
                  560,//t_y
                  600,//t_dx
                  112,//t_dy
                  -100,//X
                  -100,//Y
                  0.5*win_DY/(1.28),//600 - размеры
                  0.5*win_DY/(6.86),//112
                  true);

  InitButton(index_button_exit,//номер кнопки - выход
             "Exit To System",//имя кнопки
             "data/pic/ui.png",//адрес картинки

             2048,//W
             1024,//H
             0,//t_x
             224,//t_y
             600,//t_dx
             112,//t_dy
             -100,//X
             -100,//Y
             0.5*win_DY/(1.28),//600 - размеры
             0.5*win_DY/(6.86),//112
             0,//anim
             0,//anim_horiz
             0,//anim_vert
             0,//start
             0,//finish
             0,//delta
             0,//delay
             0,//loop

             true);//on screen

//кнопки в меню игры
  InitButton(index_button_resume,//номер кнопки - вернутся
             "Resume",//имя кнопки
             "data/pic/ui.png",//адрес картинки

             2048,//W
             1024,//H
             0,//t_x
             448,//t_y
             600,//t_dx
             112,//t_dy
             -100,//X
             -100,//Y
             0.5*win_DY/(1.28),//600 - размеры
             0.5*win_DY/(6.86),//112
             0,//anim
             0,//anim_horiz
             0,//anim_vert
             0,//start
             0,//finish
             0,//delta
             0,//delay
             0,//loop

             true);//on screen

  InitButton(index_button_return,//номер кнопки - вернутся
             "Return to Main Menu",//имя кнопки
             "data/pic/ui.png",//адрес картинки

             2048,//W
             1024,//H
             0,//t_x
             336,//t_y
             600,//t_dx
             112,//t_dy
             -100,//X
             -100,//Y
             0.5*win_DY/(1.28),//600 - размеры
             0.5*win_DY/(6.86),//112
             0,//anim
             0,//anim_horiz
             0,//anim_vert
             0,//start
             0,//finish
             0,//delta
             0,//delay
             0,//loop

             true);//on screen
 }

void Init_ButtonsVar()
 {
  //Задержка
  RButton[index_rbutton_delay].case_value[1]=0;
  RButton[index_rbutton_delay].case_value[2]=10;
  RButton[index_rbutton_delay].case_value[3]=20;
  RButton[index_rbutton_delay].case_value[4]=30;
  RButton[index_rbutton_delay].case_value[5]=40;
  RButton[index_rbutton_delay].case_value[6]=50;
  RButton[index_rbutton_delay].case_value[7]=60;
  RButton[index_rbutton_delay].case_value[8]=70;
  RButton[index_rbutton_delay].case_value[9]=80;
  RButton[index_rbutton_delay].case_value[10]=90;

  //Громкость музыки
  RButton[index_rbutton_music_vol].case_value[1]=0;
  RButton[index_rbutton_music_vol].case_value[2]=4;
  RButton[index_rbutton_music_vol].case_value[3]=16;
  RButton[index_rbutton_music_vol].case_value[4]=32;
  RButton[index_rbutton_music_vol].case_value[5]=48;
  RButton[index_rbutton_music_vol].case_value[6]=64;
  RButton[index_rbutton_music_vol].case_value[7]=80;
  RButton[index_rbutton_music_vol].case_value[8]=96;
  RButton[index_rbutton_music_vol].case_value[9]=112;
  RButton[index_rbutton_music_vol].case_value[10]=128;

  //Громкость звуков
  RButton[index_rbutton_sound_vol].case_value[1]=0;
  RButton[index_rbutton_sound_vol].case_value[2]=4;
  RButton[index_rbutton_sound_vol].case_value[3]=16;
  RButton[index_rbutton_sound_vol].case_value[4]=32;
  RButton[index_rbutton_sound_vol].case_value[5]=48;
  RButton[index_rbutton_sound_vol].case_value[6]=64;
  RButton[index_rbutton_sound_vol].case_value[7]=80;
  RButton[index_rbutton_sound_vol].case_value[8]=96;
  RButton[index_rbutton_sound_vol].case_value[9]=112;
  RButton[index_rbutton_sound_vol].case_value[10]=128;	 
 }
