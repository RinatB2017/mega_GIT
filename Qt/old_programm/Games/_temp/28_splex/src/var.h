//Разработано aureli0,NeonLight_Child,Ph0Eni}{,Zinoviev54,imanavhod-это все я:)
//http://sourceforge.net/projects/swibplex/
//последний билд от 23.12.2009

//Раздел констант
const
 int lev_width=59,//размеры уровня по X//
     lev_height=23,//размеры уровня по Y//

     lev_num_max=lev_width*lev_height,//максимальное количество элементов уровня
     obj_num_max=lev_width*lev_height,//максимальное количество объектов

     eff_num_max=99;//максимальное количество эффектов

//Record
struct lev_type //сетка уровня
 { 
  int x,
      y,
      unit,
      exit_score,//очки на выход от бонусов
      exit_kills;//очки на выход от врагов
 }
lev[lev_num_max],
lev_tmp[lev_num_max];//промежуточная сетка для зеркала

struct obj_type //сетка объектов
 { 
  int x,
      y,
      unit;
 }
obj[obj_num_max],
obj_tmp[obj_num_max];//промежуточная сетка для зеркала

char buff[200],//для загрузки индексированных текстур
     txt_tmp[999];//для цифр текста [колво символов]

float x_cam,//координаты камеры
      y_cam;

int grid,//размер ячейки сетки уровня
    old_grid,
    delta;//для телепортов и курсоров

bool debug_lev_matrix=false,
     debug_obj_matrix=false;

//таймер для игры
int game_now,
    game_then,
    game_timer;
//массив лучшего времени для каждого из пройденных этапов
int best_time[999];//+1

string level_name[999];//массив имен уровней +1

//переменные колобка
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
    p_kal[lev_num_max];//остатки от печатных плат (1-го типа lev[]) 


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

//телепорт как в portal
int t1x,t1y,t2x,t2y,//координаты входа и выхода из телепорта
    t1y_tmp,
    t2y_tmp; 

bool t1_true,t2_true,//определяет есть ли телепорты
     mirror,//перевернут ли уровень
     mirror_true,//состояние перевернутости уровня
     lev_door_open[lev_num_max],//открыта ли дверь
     lev_button_push[lev_num_max],//нажат ли объект-кнопка
     lev_light_push[lev_num_max];//включен ли светильник

//переменные динамических(которые могут перемещаться) объектов
float obj_sigma[obj_num_max],//графическое увеличение объекта
      obj_move_x[obj_num_max],//смещение
      obj_move_y[obj_num_max],
      obj_delta[obj_num_max];//угол поворота относительно центра
float obj_d1[obj_num_max],
      obj_d2[obj_num_max];

    
int obj_index[obj_num_max],//индекс объекта      
    obj_rotate[obj_num_max],//определяет тип кручения объекта 1 - при движении, 2- все время
    obj_AI_param1[obj_num_max],//для перемещения объекта - псевдо АИ
    obj_AI_param2[obj_num_max],
    obj_sleep_count[obj_num_max],//таймер покоя, после падения = 5,затем вычитается до 0
    obj_nuke_index,//индекс взрывающегося объекта
    obj_nuke_count[obj_num_max],//счетчик до взрыва объекта
    obj_dead[obj_num_max],//мертв ли объект и с каким эффектом он сдохнет
    obj_frame[obj_num_max],//текущий кадр анимации
    obj_frame_count[obj_num_max],//счетчик задержки между кадрами
    obj_angle_finish[obj_num_max],//финальный угол поворота
    obj_angle_count[obj_num_max],
    obj_napr[obj_num_max],
    obj_kick_index;//индекс пихнутого объекта  

bool obj_move[obj_num_max],//объект движется
     obj_move_up[obj_num_max],
     obj_move_down[obj_num_max],
     obj_move_left[obj_num_max],
     obj_move_right[obj_num_max],
     obj_move_up_v[obj_num_max],//объект может двинутся вверх
     obj_move_down_v[obj_num_max],
     obj_move_left_v[obj_num_max],
     obj_move_right_v[obj_num_max],
     
     //для проверок на сваливаемость для элементов OBJ 1,2
     obj_move_up_left_v[obj_num_max],//вверх-влево
     obj_move_up_right_v[obj_num_max],//вверх-вправо
     obj_move_down_left_v[obj_num_max],//вниз-влево
     obj_move_down_right_v[obj_num_max],//вниз-вправо
     
     obj_move_kick_left_mark[obj_num_max],//отмечен для тычка слева
     obj_move_kick_right_mark[obj_num_max],//отмечен для тычка справа
     obj_nuke[obj_num_max],//объект на подрыве
     obj_anim[obj_num_max];//есть ли анимация в объекте

//переменные для проверки некоторых коллизий
int lev_matrix[lev_width+1][lev_height+1],//матрица уровня
    obj_matrix[lev_width+1][lev_height+1],//матрица объектов
//временные переменные для отзеркаливания
    lev_matrix_tmp[lev_width+1][lev_height+1],//матрица уровня
    obj_matrix_tmp[lev_width+1][lev_height+1],//матрица объектов

    dark_matrix[lev_width+1][lev_height+1],
    dark_matrix_tmp[lev_width+1][lev_height+1];

//переменные видео эффектов
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

int dark_radius,//радиус света вокруг Свиборга
    lev_dark_radius;//радиус света вокгру фонарей уровня

FILE *fp;//для записи и загрузки уровня

//Текстуры
GLuint player_texture[20], //хэндл текстур колобка
       obj_texture[32],//текстуры объектов
       obj_anim_texture[32],//для объектов с анимацией 
       t_texture,//текстура портала
       lev_texture[32],//текстуры уровня
       other_texture[10],//остальные текстуры
       eff_texture[99][999],//текстуры эффектов
       dark_texture,//текстура тьмы 
       cursor_texture,//текстура курсора для редактора и кнопок меню
       background_texture;//задний фон

//============
//=[РЕДАКТОР]=
//============
int edit_mode,//режим редактирования
    score_for_exit,//очки выхода
    num_texture,//порядковый номер текстуры
    x_cursor,
    y_cursor;    
