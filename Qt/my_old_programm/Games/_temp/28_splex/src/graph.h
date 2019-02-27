//Разработано aureli0,NeonLight_Child,Ph0Eni}{,Zinoviev54,imanavhod-это все я:)
//http://sourceforge.net/projects/swibplex/
//последний билд от 23.12.2009

//Модуль вывода и обработки графики 
using namespace std;//ХЗ что это но с ним заработал строковый тип string (string.h)

//Раздел констант
//const 
// int button_max=9999;//максимальное количество кнопок

//раздел переменных
TTF_Font *fonts[9] = {NULL,NULL,NULL};//всего 3 шрифта
//Цвет текста 
  SDL_Color txtColorRed = {255,1,1};
  SDL_Color txtColorGreen = {1,255,1};
  SDL_Color txtColorBlue = {1,1,255};
  SDL_Color txtColorWhite = {255,255,255};
  SDL_Color txtColorBlack = {1,1,1};
GLuint t_txt;//временная тектовая текстура

//для счетчика ФПС
int fps,fps_max,fps_timer,timer,now,then;  
float k_fps;//коэффициент ФПС для анимации и счетчиков

void InitFps()//инициализация ФПС
 {
  timer=0;
  fps=999;
  fps_max=fps;
 
  k_fps=1;
 }

void SolveFps()//подсчет ФПС, таймер
 {
  now = SDL_GetTicks();
  fps_timer=fps_timer+1;
  if ((now-then)>1000 or now<then)
   {
    then = SDL_GetTicks();
    fps=fps_timer;
    if (timer>1 and fps>fps_max) {fps_max=fps;}
    fps_timer=0;
    timer=timer+1;//просто секундный таймер
    k_fps=(fps/fps_max);
   } 
 }

/*
//задержка по ФПС
float delay_temp;
void Delay_by_Fps(int fps_limit)//По ограничению ФПС даем задержку
 {
  if (fps>fps_limit)
     {delay_temp=delay_temp+0.001;}
  if (fps<fps_limit and fps>0.85*fps_limit)
     {delay_temp=delay_temp-0.00025;}
  if (fps<0.85*fps_limit)
     {delay_temp=delay_temp-0.001;}
  if (delay_temp<0) {delay_temp=0.0001;}

  SDL_Delay(delay_temp);
 }
*/

//Перерисовать окно-сперто из sdlball
void resizeWindow( int width, int height )
{
    /* Height / width ration */
    GLfloat ratio;

    /* Protect against a divide by zero */
    if ( height == 0 )
      height = 1;

    ratio = ( GLfloat )width / ( GLfloat )height;

    /* Setup our viewport. */
    glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );

    /* change to the projection matrix and set our viewing volume. */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );

    /* Set our perspective */
    gluPerspective( 45.0f, ratio, 0.1f, 10.0f );

    /* Make sure we're chaning the model view and not the projection */
    glMatrixMode( GL_MODELVIEW );

    /* Reset The View */
    glLoadIdentity();
}


//инициализация 2д 
void gl2dMode (int dW, int dH)
 {
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0, dW, dH, 0, -1, 1 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
 }

//Функция загрузки текстур из файла в переменную
bool load(string file, GLuint &tex)
 {
  SDL_Surface *temp = NULL;
  GLint maxTexSize;
  GLuint glFormat = GL_RGBA;
      
  if(file.substr(file.length()-3,3).compare("jpg") == 0)
   {
    glFormat = GL_RGB;
   }
      
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize); 

  temp = IMG_Load(file.data());

  if(temp == NULL)
   {
    cout << "Texture manager ERROR: " << file << " : "<< SDL_GetError() << endl;
    SDL_FreeSurface( temp );
    return 0;
   }
      
  if(temp->w > maxTexSize)
   {
    cout << "Texture manager ERROR: " << file << " texturesize too large." << endl;
   }
 
  glGenTextures(1, &tex);
  glBindTexture(GL_TEXTURE_2D, tex);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );

  glTexImage2D(GL_TEXTURE_2D, 0, glFormat, temp->w, temp->h, 0, glFormat, GL_UNSIGNED_BYTE, temp->pixels);
  SDL_FreeSurface( temp );

  return 1;
}

//рисуем затекстурированый полигон
void DrawTXT (float x,
              float y,
              float dX,
              float dY,
              float delta,
              int center)
 {
    glLoadIdentity();
    glTranslatef(x,y,0);   
    glRotatef(delta,0,0,-1); 
    if (center==1) {glTranslatef(-dX/2,-dY/2,0);}//смещаем по центре

    glBegin( GL_QUADS );
        // Верхний левый угол
        glTexCoord2i(0,0);      glVertex2f( 0, 0 );
        //Нижний левый угол
        glTexCoord2i( 0, 1 );   glVertex2f( 0, dY );
        //Нижний правый угол
        glTexCoord2i( 1, 1 );   glVertex2f( dX, dY );
        //Верхний правый угол
        glTexCoord2i( 1, 0 );   glVertex2f( dX, 0 );
    glEnd();
    glLoadIdentity();
 } 

void TTF_LoadFont(int grid) //инициализация шрифтов
 {
//Массив фонтов, задаем шрифт и болд
  fonts[1] = TTF_OpenFont( "data/ttf/font1.ttf", win_DY/64/*12*/ );//открываем фонт
  fonts[2] = TTF_OpenFont( "data/ttf/font1.ttf", win_DY/48/*16*/ );//открываем фонт
  fonts[3] = TTF_OpenFont( "data/ttf/font2.ttf", win_DY/24/*32*/ );//открываем фонт 
  fonts[4] = TTF_OpenFont( "data/ttf/font2.ttf", win_DY/12/*64*/ );//открываем фонт
  fonts[5] = TTF_OpenFont( "data/ttf/font2.ttf", grid/*64*/ );//открываем фонт
 }

//Создаем текстовую текстуру - использовать только при инициализации
void GlTxt(int sdl_dx,int sdl_dy,TTF_Font *font, SDL_Color textColor,const char text[2], GLuint texture)
 {
  SDL_Surface *temp,*tempb;
  int w,h;
  Uint32 rmask, gmask, bmask, amask;
  #if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
  #else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
  #endif

  if(font == NULL)
   { 
    cout << SDL_GetError() << endl;;
    SDL_FreeSurface(temp);
    SDL_FreeSurface(tempb);
   }

  temp = TTF_RenderText_Blended(font, text, textColor);
  SDL_SetAlpha(temp, 0, 0);

  tempb = SDL_CreateRGBSurface(0, sdl_dx, sdl_dy, 32, rmask,gmask,bmask,amask);

  TTF_SizeUTF8(font,text, &w,&h);
  
  SDL_Rect src,dst;
  src.x=0;
  src.y=0;
  src.w=sdl_dx;//w;//криво но работает, иначе тока до 10 символов в строке
  src.h=sdl_dy;//h;

  dst.x=0;
  dst.y=0;
  dst.w=sdl_dx;//w;
  dst.h=sdl_dy;//h;

  SDL_BlitSurface(temp, &src, tempb, &dst);
  glBindTexture(GL_TEXTURE_2D, texture);
  gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, tempb->w, tempb->h, GL_RGBA, GL_UNSIGNED_BYTE, tempb->pixels);

  SDL_FreeSurface(temp);
  SDL_FreeSurface(tempb);

 }

//процедура смены кадра, и контроля
void ControlAnimate(int *frame,//номер текущего кадра
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


//процедура смены угла, и контроля
void ControlAngle(float *angle,//номер текущего угла
//                int angle_start,//стартовый угол
                  float angle_finish,//финальный угол
                  float angle_delta,//шаг между углами
                  int *angle_count,//переменная отвечающая за счетчик задержки при повороте
                  int delay,//задержка между поворотами
                  float *d1,
                  float *d2, 
                  int *napr)
 {
  float angle_start;

  if (*napr==0 and *angle!=angle_finish)//инициализация-выбор кратчайшего направления поворота
   {
    angle_start=*angle;
    if (angle_finish>=angle_start) {*d1=angle_finish-angle_start;}  
    if (angle_finish<angle_start) {*d1=angle_start-angle_finish;}  
    *d2=360-*d1;
    //Ищем тот d который минимален для выбора направления поворота
    if (angle_finish>angle_start)
     {
      if (*d1>=*d2) {*napr=2;}//по часовой*
      if (*d1<*d2) {*napr=1;}//против часовой
     }
    if (angle_finish<angle_start)
     {
      if (*d1>=*d2) {*napr=1;}//против часовой
      if (*d1<*d2) {*napr=2;}//по часовой*
     }
   }
  *angle_count=*angle_count+1;//счетчик задержки
  if (*angle_count>=delay)
   {
    *angle_count=0;

    if (*napr==1)//движение против часовой стрелки
     {
      *angle=*angle+angle_delta;
      if (*angle>360) {*angle=0;}
     }
    if (*napr==2)//движение по часовой стрелке
     {
      *angle=*angle-angle_delta;
      if (*angle<0) {*angle=360;}
     }

   //cout << "angle=" <<*angle <<" a_fin=" <<angle_finish<<" napr="<<*napr << " d1="<< *d1 <<" d2="<< *d2 <<endl;

    if (*angle>angle_finish-angle_delta and *angle<angle_finish+angle_delta and angle_finish>0 or
        *angle<angle_finish+angle_delta and angle_finish==0 or
        *angle>angle_finish+360-angle_delta and angle_finish==0)
     {
      *angle=angle_finish;
      *napr=0;
      *d1=0;
      *d2=0;
     }
   }
 }

//==[Кнопки]==
//Раздел констант
const 
 int button_max=999;//максимальное количество кнопок

struct Button_type//кнопки
 { 
  float x,y,dx,dy;//координаты и размеры кнопки
  bool cursor,//курсор находится на кнопке
       click,//была ли нажата клавиша
       on_screen;//видна ли кнопка на экране

  char *name;//имя кнопки
  GLuint texture;//текстура кнопки
 }
Button[button_max];

//Иництализация 1-й кнопки
void InitButton(int button_index,
                char button_text[],
                char file_name[],  
                float x,
                float y,
                float dx,
                float dy, 
                bool on_screen)
 {
  Button[button_index].name=button_text;
  load(file_name,Button[button_index].texture);
  Button[button_index].x=x;
  Button[button_index].y=y;
  Button[button_index].dx=dx;
  Button[button_index].dy=dy;
  Button[button_index].on_screen=on_screen;
 }

//Обработка нажатий на кнопки
//для целых значений
void KeyButton_int(int button_index, 
                     SDL_Event event,
                     float mouse_X,
                     float mouse_Y,
                     int *click,
                     int click_delta, 
                     int click_limit,//до какого значения - граница
                     int click_val)//чему приравнивать значение после click_limit
 { 
  if (Button[button_index].on_screen==true)
   {
    if (mouse_X>Button[button_index].x-0.5*Button[button_index].dx and 
        mouse_X<Button[button_index].x+0.5*Button[button_index].dx and 
        mouse_Y>Button[button_index].y-0.5*Button[button_index].dy and
        mouse_Y<Button[button_index].y+0.5*Button[button_index].dy)
     { 
      Button[button_index].cursor=true;
      //нажатие на кнопки
      if (event.type == SDL_MOUSEBUTTONDOWN and
          event.button.state == SDL_PRESSED and
          event.button.button == SDL_BUTTON_LEFT) 
       {
        Button[button_index].click=not Button[button_index].click;//клик по кнопке был
        
        *click=*click+click_delta;

        //если дельта положительная
        if (click_delta>0 and
            *click>click_limit) {*click=click_val;}
        //если дельта отрицательная
        if (click_delta<0 and
           *click<click_limit) {*click=click_val;} 
       }
     }
    else Button[button_index].cursor=false;
  
    //отжимаем клавишу если на ней нету курсора а она нажималась
    if (Button[button_index].cursor==false and
        Button[button_index].click==true) 
     {Button[button_index].click=false;}
   }
 }  

//для значений чисел с точкой
void KeyButton_float(int button_index, 
                     SDL_Event event,
                     float mouse_X,
                     float mouse_Y,
                     float *click,
                     float click_delta, 
                     float click_max)
 { 
  if (Button[button_index].on_screen==true)
   {
    if (mouse_X>Button[button_index].x-0.5*Button[button_index].dx and 
        mouse_X<Button[button_index].x+0.5*Button[button_index].dx and 
        mouse_Y>Button[button_index].y-0.5*Button[button_index].dy and
        mouse_Y<Button[button_index].y+0.5*Button[button_index].dy)
     { 
      Button[button_index].cursor=true;
      //нажатие на кнопки
      if (event.type == SDL_MOUSEBUTTONDOWN and
          event.button.state == SDL_PRESSED and //однократное нажатие!!
          event.button.button == SDL_BUTTON_LEFT) 
       {
        Button[button_index].click=not Button[button_index].click;//клик по кнопке был

        *click=*click+click_delta;
        //если дельта положительная
        if (click_delta>0 and
            *click>click_max) {*click=0;}
        //если дельта отрицательная
        if (click_delta<0 and
            *click<0) {*click=click_max;}
       }  
     }
    else Button[button_index].cursor=false;
  
    //отжимаем клавишу если на ней нету курсора а она нажималась
    if (Button[button_index].cursor==false and
        Button[button_index].click==true) 
     {Button[button_index].click=false;}
   }
 } 

//Рисуем кнопку
void DrawButton(int button_index,
                float R, float G, float B, float A)
 {
  if (Button[button_index].on_screen==true)
   {
    glColor4d(R,G,B,A);
    glEnable(GL_TEXTURE_2D);   
    glBindTexture(GL_TEXTURE_2D, Button[button_index].texture);
    DrawTXT(Button[button_index].x,
            Button[button_index].y,
            Button[button_index].dx-5*Button[button_index].click,
            Button[button_index].dy-5*Button[button_index].click,0,1);
    glDisable(GL_TEXTURE_2D); 
    glColor4d(1,1,1,1);
   }
 }

//подцветка кнопок при наведении мыши
void LightButton(int button_index, GLuint cursor_texture,
                 float R, float G, float B, float A)
 { 
  if (Button[button_index].on_screen==true and
      Button[button_index].cursor==true) 
   {
    //рисуем курсор панели на кнопке
    glColor4d(R,G,B,A);
    glEnable(GL_TEXTURE_2D);   
    glBindTexture( GL_TEXTURE_2D, cursor_texture);
    DrawTXT(Button[button_index].x,
            Button[button_index].y,
            Button[button_index].dx+5,
            Button[button_index].dy+5,0,1);
    glDisable(GL_TEXTURE_2D); 
    glColor4d(1,1,1,1);
   }
 }

void Kill_Button(int button_index)
 {
  Button[button_index].on_screen=false;
  Button[button_index].cursor=false;
  Button[button_index].click=false;
 }

void Reset_Button(int button_index)//чтобы при последущем входе не было глюков с отжатием кнопки
 {
  Button[button_index].cursor=false;
  Button[button_index].click=false;
 }

//==[Радиокнопки]==
struct RButton_type//кнопки
 { 
  float x,y,dx,dy;//координаты и размеры кнопки
  int orient,//тип кнопки-вертикальный или горизонтальный
      case_max,//сколько всего случаев начиная с 1 (1,2,3,4,...)
      case_index_cursor,//номер сегмента под курсорм
      case_index,//номер выбранного сегмента 
      case_dx,//размер сегмента по горизонтали
      case_dy,//размер сегмента по вертикали

      case_value[999];//значение сегмента
  
  bool cursor,//курсор находится на кнопке
       on_screen;//видна ли кнопка на экране

  char *name;//имя кнопки
  GLuint texture;//текстура кнопки
 }
RButton[button_max];

//Иництализация радио кнопки-вертикальной
void InitRadioButton(int button_index,//номер кнопки-поля
                     int click,//к какой величине привязана
                     char button_text[],//текс кнопки
                     char file_name[],//имя текстуры
                     int case_max,//количество сегментов
                     int button_type,//тип кнопки-вертикальный или горизонтальный
                     float x,//координаты поля
                     float y,
                     float dx,//размеры поля
                     float dy, 
                     bool on_screen)//на экране ли
 {
  RButton[button_index].name=button_text;
  load(file_name,RButton[button_index].texture);
  RButton[button_index].case_max=case_max;
  RButton[button_index].orient=button_type;
  if (button_type==1) {RButton[button_index].case_dy=dy/case_max;}//высота сегмента
  if (button_type==2) {RButton[button_index].case_dx=dx/case_max;}//длина сегмента
  RButton[button_index].x=x;
  RButton[button_index].y=y;
  RButton[button_index].dx=dx;
  RButton[button_index].dy=dy;
  RButton[button_index].on_screen=on_screen;

  int i;
  //значение click по умолчанию
  for(i=1;i<=RButton[button_index].case_max;i++) 
   {
    if (RButton[button_index].case_value[i]==click)
     {     
      RButton[button_index].case_index=i;//выбранный сегмент
      break;
     }
   } //конец цикла
 }

//Обработка нажатий на кнопки
//для целых значений
void KeyRadioButton(int button_index, 
                    int *click,
                    SDL_Event event,
                    float mouse_X,
                    float mouse_Y)
 { 
  int i;
  if (RButton[button_index].on_screen==true)//если радио-кнопка на экране
   {
    if (mouse_X>RButton[button_index].x-0.5*RButton[button_index].dx and//курсор мыши над кнопкой 
        mouse_X<RButton[button_index].x+0.5*RButton[button_index].dx and 
        mouse_Y>RButton[button_index].y-0.5*RButton[button_index].dy and
        mouse_Y<RButton[button_index].y+0.5*RButton[button_index].dy)
     { 
      RButton[button_index].cursor=true;//курсор над кнопкой-полем

      //в зависимости от области радио-кнопки значение click ,проверка ПО ВЕРТИКАЛИ    
      for(i=1;i<=RButton[button_index].case_max;i++) 
       {
    
        if (RButton[button_index].orient==1 and//по вертикали
            mouse_Y>RButton[button_index].y-0.5*RButton[button_index].dy+(i-1)*RButton[button_index].case_dy and
            mouse_Y<RButton[button_index].y-0.5*RButton[button_index].dy+i*RButton[button_index].case_dy or
            RButton[button_index].orient==2 and//по горизонтали
            mouse_X>RButton[button_index].x-0.5*RButton[button_index].dx+(i-1)*RButton[button_index].case_dx and
            mouse_X<RButton[button_index].x-0.5*RButton[button_index].dx+i*RButton[button_index].case_dx)
         {
          RButton[button_index].case_index_cursor=i;//текущий номер сегмента под курсором
          break;
         }
       } //конец цикла

       //обработка нажатия на кнопку мыши 
      if (event.type == SDL_MOUSEBUTTONDOWN and 
          event.button.state == SDL_PRESSED and//однократное нажатие
          event.button.button == SDL_BUTTON_LEFT)//левой кнопкой
       {
        //клик равен величине присвоенной сегменту кнопки
        RButton[button_index].case_index=RButton[button_index].case_index_cursor;
        *click=RButton[button_index].case_value[RButton[button_index].case_index];
       }//конец условия
 
     }//конец условия
    else RButton[button_index].cursor=false;
  
   }//конец условия "если радио-кнопка на экране"
 } 

void DrawRadioButton(int button_index,GLuint cursor_texture,
                     float R, float G, float B, float A)
 {
  if (RButton[button_index].on_screen==true)
   {
    glColor4d(R,G,B,A);
    glEnable(GL_TEXTURE_2D);   
    glBindTexture(GL_TEXTURE_2D, RButton[button_index].texture);
    DrawTXT(RButton[button_index].x,
            RButton[button_index].y,
            RButton[button_index].dx,
            RButton[button_index].dy,0,1);
    //рисуем курсор на выбранный ранее сегмент
    if (RButton[button_index].case_index>0)
     {
      glBindTexture( GL_TEXTURE_2D, cursor_texture);

      if (RButton[button_index].orient==1)//вертикальный курсор
       {
        DrawTXT(RButton[button_index].x,
                RButton[button_index].y-0.5*RButton[button_index].dy
                                       +(RButton[button_index].case_index-1)*RButton[button_index].case_dy
                                       +0.5*RButton[button_index].case_dy,
                RButton[button_index].dx+5,
                RButton[button_index].case_dy+5,0,1);
       }

      if (RButton[button_index].orient==2)//горизонтальный курсор
       {
        DrawTXT(RButton[button_index].x-0.5*RButton[button_index].dx
                                       +(RButton[button_index].case_index-1)*RButton[button_index].case_dx
                                       +0.5*RButton[button_index].case_dx,
                RButton[button_index].y,
                RButton[button_index].case_dx+5,
                RButton[button_index].dy+5,0,1);
       }


     }
    glDisable(GL_TEXTURE_2D); 
    glColor4d(1,1,1,1);
   }
 }

//подцветка кнопок при наведении мыши
void LightRadioButton(int button_index, GLuint cursor_texture,
                      float R, float G, float B, float A)
 { 
  if (RButton[button_index].on_screen==true and
      RButton[button_index].cursor==true) 
   {
    //рисуем курсор панели на сегменте кнопки-поля
    glColor4d(R,G,B,A);
    glEnable(GL_TEXTURE_2D);   
    glBindTexture( GL_TEXTURE_2D, cursor_texture);

    if (RButton[button_index].orient==1)//вертикальный курсор
     {
      DrawTXT(RButton[button_index].x,
              RButton[button_index].y-0.5*RButton[button_index].dy
                                     +(RButton[button_index].case_index_cursor-1)*RButton[button_index].case_dy
                                     +0.5*RButton[button_index].case_dy,
              RButton[button_index].dx+5,
              RButton[button_index].case_dy+5,0,1);
     }

    if (RButton[button_index].orient==2)//горизонтальный курсор
     {
      DrawTXT(RButton[button_index].x-0.5*RButton[button_index].dx
                                     +(RButton[button_index].case_index_cursor-1)*RButton[button_index].case_dx
                                     +0.5*RButton[button_index].case_dx,
              RButton[button_index].y,
              RButton[button_index].case_dx+5,
              RButton[button_index].dy+5,0,1);
     }

    glDisable(GL_TEXTURE_2D); 
    glColor4d(1,1,1,1);
   }

 }

