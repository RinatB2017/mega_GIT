//http://swibplex.sourceforge.com

#include <iostream>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <string.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>

//#include <FTGL/ftgl.h>//для русского шрифта

//Для использования xml парсера
//#include "pugixml.hpp"
//using namespace pugi;
//если использовать pugi тогда makefile будет таким
//all:
//	gcc -o ssge ssge.cpp pugixml.cpp -lm -I/usr/include/GL -lGL -lGLU -lSDL_ttf -lSDL_mixer -lSDL_image

//раздел переменных
TTF_Font *fonts[3];// = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};//всего 3 шрифта
//Цвет текста
SDL_Color txtColorRed = {255,1,1};
SDL_Color txtColorGreen = {1,255,1};
SDL_Color txtColorBlue = {1,1,255};
SDL_Color txtColorWhite = {255,255,255};
SDL_Color txtColorBlack = {1,1,1};

GLuint t_txt;//временная тектовая текстура

char buff[999];//переменная буффера

int win_DX=1024,//размеры окна по X
    win_DY=768,//размеры окна по Y
    old_win_DX,
    old_win_DY,
    pal,//палитра
    full_screen,//на полный экран или нет
    shadows_ON,
    alpha_advance_ON;

float mouse_X,//координаты мышки
      mouse_Y;

bool mouse_LB,mouse_RB;//о нажиме на левую и правую кнопки мыши

SDL_Surface *screen,//Экран
            *surface;//SDL поверхность

SDL_Event event;//SDL событие

Uint8* keys;//для управления клавиатурой

using namespace std;//ХЗ что это но с ним заработал строковый тип string

struct TVector2
 {
  float  x,// красный
         y;// зелёный
 };

TVector2 normaliz2(TVector2 n)
 {
  float l;
  TVector2 tm;
  l=sqrt(n.x*n.x+n.y*n.y);

  if (l!=0)
   {
    tm.x=n.x/l;
    tm.y=n.y/l;
   }
  return tm;
 }

void Randomize()
 {
  srand(time(NULL));//randomize;-включить счетчик случайных чисел <iostream.h>
}

int random(int i)
 {
  return rand()%i+1;
 }

int ZorO(int i)//либо 0, либо 1 (Zero Or One)
 {
  if (i>0) {i=1;}
  if (i<=0) {i=0;}
  return i;
 }

//аналогично противоположная функция
int OorZ(int i)//либо 1, либо 0 (One Or Zero)
 {
  if (i>0) {i=0;}
  if (i<=0) {i=1;}
  return i;
 }


		/*
		SDL_EnableUNICODE(SDL_ENABLE);   
		if (name.length() < 5)
         {
          if (event.key.keysym.unicode >= (Uint16)'a' and event.key.keysym.unicode <= (Uint16)'z')
           {
            name += (char)event.key.keysym.unicode;
           }
           
          if (event.key.keysym.unicode >= (Uint16)'0' and event.key.keysym.unicode <= (Uint16)'9')
           {
            name += (char)event.key.keysym.unicode;
           }
           
          else if (event.key.keysym.unicode >= (Uint16)'A' and event.key.keysym.unicode <= (Uint16)'Z')
           {
            name += (char)event.key.keysym.unicode;
           }
         }
		 
		if (event.key.keysym.sym == SDLK_BACKSPACE and name.length() > 0)
         {
          name.erase(name.length() - 1);
         } 
         
         
		 */ 

//Ввод текста с клавиатуры ненарушая основного хода цикла
void Input(SDL_Event event, string *name, int limit, bool *confirm)
 {
  string tmp=*name;	 
  SDL_EnableUNICODE(SDL_ENABLE);
   
  if (event.type==SDL_KEYDOWN)//еcли нажата кнопка на клавиатуре-однократное нажатие
   {
	if (*confirm==true)   
	 {  
 	  if (tmp.length() < limit)
       {
        if (event.key.keysym.unicode >= (Uint16)'a' and event.key.keysym.unicode <= (Uint16)'z')
         {
          tmp += (char)event.key.keysym.unicode;
         }
           
        if (event.key.keysym.unicode >= (Uint16)'0' and event.key.keysym.unicode <= (Uint16)'9')
         {
          tmp += (char)event.key.keysym.unicode;
         }
           
        else if (event.key.keysym.unicode >= (Uint16)'A' and event.key.keysym.unicode <= (Uint16)'Z')
         {
          tmp += (char)event.key.keysym.unicode;
         }
       }

      if (event.key.keysym.sym == SDLK_BACKSPACE and tmp.length() > 0)
       {
        tmp.erase(tmp.length() - 1);
       }
           
       *name=tmp; 
     }
     
    if (event.key.keysym.sym==SDLK_RETURN) {*confirm=false;}
     
   }
 }  

/*
void findMaxi(int arr[], int n, int* pToMax)
 {
  if (n <= 0)
  return;
  int max = arr[0];
  pToMax = &arr[0];
  for (int i = 1; i < n; i++)
   {
    if (arr[i] > max)
     {
      max = arr[i];
      pToMax = (arr+i);
     }
   }
 }

void findMini(int arr[], int n, int* pToMin)
 {
  if (n <= 0)
  return;
  int min = arr[0];
  pToMin = &arr[0];
  for (int i = 1; i < n; i++)
   {
    if (arr[i] < min)
     {
      min = arr[i];
      pToMin = (arr+i);
     }
   }
 }

void findMinfval(float arr[], int n, float* pToMin)
 {
  if (n <= 0) {return;}
  float min = arr[0];
  pToMin = &arr[0];
  for (int i = 1; i < n; i++)
   {
    if (arr[i] < min)
     {
      min = arr[i];
      pToMin = (arr+i);
     }
   }
 }
*/
void findMinfindex(float arr[], int n, int* n_Min)
 {
  if (n <= 0) {return;}
  float min = arr[0];
  for (int i = 1; i < n; i++)
   {
    if (arr[i] < min)
     {
      min = arr[i];
      *n_Min=i;
     }
   }
 }

//============================================= max
// From algorithms/arrayfuncs.cpp
// Returns the maximum value in an array.
float maxf(float a[], int size)
 {
  float maxVal = a[0];     // Note 2.
  for (int i=1; i<size; i++)
   {
    if (a[i] > maxVal)
     {
      maxVal = a[i];
     }
   }
  return maxVal;
 }//end max

float minf(float a[], int size)
 {
  float minVal = a[0];     // Note 2.
  for (int i=1; i<size; i++)
   {
    if (a[i] < minVal)
     {
      minVal = a[i];
     }
   }
  return minVal;
 }//end max


int maxi(int a[], int size)
 {
  int maxVal = a[0];     // Note 2.
  for (int i=1; i<size; i++)
   {
    if (a[i] > maxVal)
     {
      maxVal = a[i];
     }
   }
  return maxVal;
 }//end max

int mini(int a[], int size)
 {
  int minVal = a[0];     // Note 2.
  for (int i=1; i<size; i++)
   {
    if (a[i] < minVal)
     {
      minVal = a[i];
     }
   }
  return minVal;
 }//end max

//============================================= maxIndex
// From algorithms/arrayfuncs.cpp
// Returns the index of the maximum value in an array.
int maxIndex(float a[], int size) {
    int maxIndex = 0;
    for (int i=1; i<size; i++) {
        if (a[i] > a[maxIndex]) {
            maxIndex = i;
        }
    }
    return maxIndex;
}//end maxIndex


/*
char *StrToChar(string s1)
 {
  char *ch=new char[sizeof(char)];
  ch=(char *)s1.c_str();
  return ch;
 }
*/

/*
char StringToChar(string text)
 {
  return text.c_str();
 }
*/


/*
int StrToInt (string s1)//строка в число
 {
  const char *ch=new char[sizeof(char)];
  ch = s1.c_str();//перевод string в char
  return atoi(ch);
 }
*/

/*
char StringToChar(string text)
 {
  return text.c_str();
 }
*/

int StrToInt(const string& s1)
 {
  return atoi(s1.c_str());
 }

/*
string IntToStr (int i)//перевод числа в строку-поправить
 {
  //или return itoa() или
  char *ch=new char[sizeof(char)];
  sprintf(ch,"%i",i);//перевод числа в символ
  string str;
  str=ch;//перевод символа в строку
  return str;
 }
*/

string IntToStr (int i)//перевод числа в строку-поправить
 {
    char buf[16];
    snprintf(buf, sizeof(buf), "%i", i);
    return string(buf);
 }

string FloatToStr (float i,int k)//перевод числа в строку-поправить
 {
    char buf[16];
    snprintf(buf, sizeof(buf), "%0.*f",k, i);
    return string(buf);
 }

/*
char *IntToChar(int i)
 {
  char *ch=new char[sizeof(char)];
  ch=StrToChar(IntToStr(i));
  return ch;
 }
*/




#include <algorithm>
string StringWordGet(string str, char separ,int token_index)//1,2,3-ые токены
 {
  string::size_type pos;//= int-текущий номер позиции разделителя
  int pos_index=1;//текущий индекс массива позиций разделителя
  int *pos_massive = new int[std::count( str.begin(), str.end(), separ )];//массив местоположения разделителей (от 0 до хрена ())
                    //выделяем память!!!
  pos=str.find_first_of(separ);//позиция,где попадается первый разделитель
  //1) заполняем массив позиций разделителя
  while (pos != string::npos)//пока не пройдемся по всей строке
   {
    pos_massive[pos_index]=pos;
    pos = str.find(separ,pos+1);
    pos_index=pos_index+1;
   }
  //2) вырезаем кусок текста-токен
  if (token_index>pos_index) {token_index=pos_index;}//выравниваем запрос
  token_index=token_index-1;//для получения нужного местоположения в массиве
  string token;//кусок текста
  if (token_index==0) {token=str.substr(0,pos_massive[1]);}//для первого токена из строки
  else
   {
    token=str.substr(pos_massive[token_index]+1,pos_massive[token_index+1]-pos_massive[token_index]-1);
   }
  return token;
 }

//для счетчика ФПС
int fps,fps_max,fps_timer,timer,timer_ep,now,then;
float k_fps;//коэффициент ФПС для анимации и счетчиков

void InitFps()//инициализация ФПС
 {
  timer=0;
  timer_ep=0;
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
    timer_ep=timer_ep+1;//таймер эпизода
    k_fps=(fps/fps_max);
   }
 }
 
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

//Функция для инициализации OpenGL средствами SDL
bool init_sdl_opengl (int win_DX,//размеры экрана
                      int win_DY,
                      int pal,//палитра 16 или 32 разрадная
                      int full_screen)//на полный =1 экран или в окне =0
 {
  //Проверка на возможность инициализация видео при помощи SDL
  //Если чтото не получилось (например не поддерживается заданный видео режим),
  //то выводим причину сбоя и заканчиваем работу программы
  if (SDL_Init(SDL_INIT_VIDEO)!=0)
   {
    printf("Unable to initialize SDL: %s\n",SDL_GetError());
    return 0;
   }
  //Иначе
  if (SDL_Init(SDL_INIT_VIDEO)==0)
   {
    //Включаем двойной буфер с OpenGL
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
    SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );//для теней и буфера трафарета под карту нвидиа
    //И инициализируем флаг SDL_OPENGL (который говорит о том что SDL использует OpenGL).
    if (full_screen==0)//если оконный режим
       {screen=SDL_SetVideoMode(win_DX,win_DY,pal,SDL_OPENGL|SDL_RESIZABLE);}//экран OpenGl на полный экран
    if (full_screen==1)//если полноэкранный режим
       {screen=SDL_SetVideoMode(win_DX,win_DY,pal,SDL_OPENGL|SDL_FULLSCREEN|SDL_RESIZABLE);}//экран OpenGl в окне
    return 1;
   }
  //Если не был установлен заданный видеорежим win_DX, win_DY, то выводим причину
  if (!screen)
   {
    printf("Unable to set video mode: %s\n",SDL_GetError());
    return 0;
   }
 }


//Инициализация 2д режима при помощи glOrtho
//glOrtho()-устанавливает режим ортогонального (прямоугольного) проецирования.
//Это значит, что изображение будет рисоваться как в изометрии.
//6 параметров типа GLdouble (или просто double): left, right, bottom, top,
//near, far определяют координаты соответственно левой, правой, нижней,
//верхней, ближней и дальней плоскостей отсечения, т.е. всё, что окажется
//за этими пределами, рисоваться не будет. На самом деле эта процедура
//просто устанавливает масштабы координатных осей.
//(http://www.gamedev.ru/articles/?id=20114)
void gl2dMode (int win_DX,
               int win_DY)
 {
  //изменяем проекционные матрицы и устнавливаем видимый объем
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, win_DX, win_DY, 0, -1, 1 );
  //Убеждаемся в том, что мы меняем вид модели, а не проекции (Make sure we're chaning the model view and not the projection)
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();//перезагрузка вида
 }

//Следующая процедура не обязательна, но полезна: она необходима
//для перерисовки окна при изменении его габаритов win_DX и win_DY (например в опциях игры).
//Для перерисовки экрана в новый размер воспользуемся изменением
//glViewport - задает область просмотра (http://opengl.gamedev.ru/doc/?func=glViewport)
//(мышкой растягивать окно можно, но видимая область в окне при этом будет без изменеия)
//Процедура позимствована из исходников sdl-ball
void resize_window (int w,
                    int h)
 {
  //win_DX=w;
  //win_DY=h;
  //Устанавливаем наш новый вьюпорт
  glViewport(0,0,(GLsizei) w,(GLsizei) h);
  gl2dMode(w, h);
 }

void ReshapeGL(int width, int height) // reshape the window when it's moved or resized
{

   win_DX=width;
   win_DY=height;

   glViewport(0,0,(GLsizei)(width),(GLsizei)(height));
     // reset the current viewport
   glMatrixMode(GL_PROJECTION);
     // select the projection matrix
   glLoadIdentity();

   glOrtho(0, (GLsizei)(width), (GLfloat)(height), 0, -1, 1 );


     // reset the projection matrix
   //gluPerspective(45.0f,(GLfloat)(width)/(GLfloat)(height),1.0f,100.0f);
     // calculate the aspect ratio of the window
   glMatrixMode(GL_MODELVIEW);
     // select the modelview matrix
   glLoadIdentity();
     // reset the modelview matrix
   return;
 }

//Функция для загрузки текстуры из графического (jpg,png,bmp) файла в переменную типа GLuint.
//Загрузка текстуры происходит с линейной фильтрацией GL_LINEAR (пиксели текстуры "размываются").
//Эту функцию лучше применять в тех случаях, когда текстура исользуется в качестве тайлов уровня
//или спрайтов с "малым содержанием дырок" - при использовании линейной фильтрации.
//Возможно появление артефактов в свободных участках текстуры. Эту функцию желательно использовать
//при инициализации.
bool load_LINEAR(string file, GLuint &tex)
 {
  SDL_Surface *temp = NULL;//временная SDL текстура
  GLint maxTexSize;
  GLuint glFormat = GL_RGBA;//по умолчанию, подразумеваем использование альфа канала в текстуре

  if(file.substr(file.length()-3,3).compare("jpg") == 0)//если изображение содержит расширению jpg - альфы нет
   {
    glFormat = GL_RGB;
   }

  //загружаем изображение во временную SDL текстуру temp
  temp = IMG_Load(file.data());

  //проверка 1
  if(temp == NULL)//если изображение не удалось загрузить, то выводим причину
   {
    cout << "Error: " << file << " : "<< SDL_GetError() << endl;
    SDL_FreeSurface(temp);//освобождаем память от временной SDL текстуры
    return 0;//и функция возвращает 0 (ложь), т.е функцию свою задачу выполнить не может
   }

  //проверка 2 - напрвышает максимально допустимого размера SDL текстуры для использования ее в OpenGL
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
  if(temp->w > maxTexSize)//если есть превышение, то функция не срабатывает и пишем сообщение об ошибке
   {
    cout << "Error: " << file << " texturka velika i shiroka." << endl;
    SDL_FreeSurface(temp);//освобождаем память от временной SDL текстуры
    return 0;//и функция возвращает 0 (ложь), т.е функцию свою задачу выполнить не может
   }

  //Если все проверки пройдены без проблем, то генерим текстуру с изображением из SDL текстуры temp
  glGenTextures(1, &tex);//создаем 1 OpenGL текстуру с именем tex
  glBindTexture(GL_TEXTURE_2D, tex);

  //передаем изображение из SDL текстуре temp текстуре tex
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//GL_LINEAR задает линейную фильтрацию
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
  glTexImage2D(GL_TEXTURE_2D, 0, glFormat, temp->w, temp->h, 0, glFormat, GL_UNSIGNED_BYTE, temp->pixels);

  SDL_FreeSurface(temp);//освобождаем память от временной SDL текстуры
  return 1;//и функция возвращает 1 (истина), т.е функцию свою задачу выполнла
}

//Следущая функция полностью аналогична предыдущей, за иключением того, что изображение передается в текстуру
//без линейной фильтрации. Как следствие, - в текстуре артефакты отсутствуют, зато присутствует
//пикселизация при малой детализацмм исходного изображения.
//В "своих" 2д играх я исользую эту функцию для спрайтов и тайлов.
bool load_NEAREST(string file, GLuint &tex)
 {
  SDL_Surface *temp = NULL;//временная SDL текстура
  GLint maxTexSize;
  GLuint glFormat = GL_RGBA;//по умолчанию, подразумеваем использование альфа канала в текстуре

  if(file.substr(file.length()-3,3).compare("jpg") == 0)//если изображение содержит расширению jpg - альфы нет
   {
    glFormat = GL_RGB;
   }

  //загружаем изображение во временную SDL текстуру temp
  temp = IMG_Load(file.data());

  //проверка 1
  if(temp == NULL)//если изображение не удалось загрузить, то выводим причину
   {
    cout << "Error: " << file << " : "<< SDL_GetError() << endl;
    SDL_FreeSurface(temp);//освобождаем память от временной SDL текстуры
    return 0;//и функция возвращает 0 (ложь), т.е функцию свою задачу выполнить не может
   }

  //проверка 2 - напрвышает максимально допустимого размера SDL текстуры для использования ее в OpenGL
  glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTexSize);
  if(temp->w > maxTexSize)//если есть превышение, то функция не срабатывает и пишем сообщение об ошибке
   {
    cout << "Error: " << file << " very big picture" << endl;
    SDL_FreeSurface(temp);//освобождаем память от временной SDL текстуры
    return 0;//и функция возвращает 0 (ложь), т.е функцию свою задачу выполнить не может
   }

  //Если все проверки пройдены без проблем, то генерим текстуру с изображением из SDL текстуры temp
  glGenTextures(1, &tex);//создаем 1 OpenGL текстуру с именем tex
  glBindTexture(GL_TEXTURE_2D, tex);

  //передаем изображение из SDL текстуре temp текстуре tex
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);//GL_NEAREST - текстура загружается как есть
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//без фильтрации. Осторожно - пиксели! :)
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
  glTexImage2D(GL_TEXTURE_2D, 0, glFormat, temp->w, temp->h, 0, glFormat, GL_UNSIGNED_BYTE, temp->pixels);

  SDL_FreeSurface(temp);//освобождаем память от временной SDL текстуры
  return 1;//и функция возвращает 1 (истина), т.е функцию свою задачу выполнила
}

void Circle (float x, float y, float r)
 {
  glBegin( GL_TRIANGLE_FAN );
  for( float i = 0; i <= 2 * 3.142 + 0.1; i += 0.1 )
   {
    glVertex2f( x + sin( i ) * r, y + cos( i ) * r );
   }
  glEnd();
 }

void Quad (float x,
           float y,
           float dX,
           float dY,
           float delta,
           int center)
 {
  glLoadIdentity();
  glTranslatef(x,y,0);//задаем местоположение
  glRotatef(delta,0,0,-1);//поворачиваемся на delta градусов

  //Если поворот происходит относительно центре, то смещаемся еще немного на 0.5dX b 0.5dY
  if (center==1) {glTranslatef(-dX/2,-dY/2,0);}

  glBegin(GL_QUADS);
   glVertex2f(0, 0 );// Верхний левый угол
   glVertex2f(0, dY);//Нижний левый угол
   glVertex2f(dX,dY);//Нижний правый угол
   glVertex2f(dX,0 );//Верхний правый угол
  glEnd();

  glLoadIdentity();
 }

//Говорят, для текстур желательно использовать файлы с изображением размерами по высоте и ширине
//равные 2 в степени n. Это связано с особенностями работы видеопамяти. На видеокартах
//nvidia я глюков особо не замечал, но на видеокартах от Intel "неправильные" изображения в
//текстуру просто не загружаются. Вместо спрайта рисуется залитый цветом полигон и больше ничего.

//Процедура для отрисовки затекстурированного полигона в координатах x,y;размером dX, dY;
//повернутом на delta град. относительно верхней левой точки, либо - относительно центра.
//Эту процедуру удобно использовать для рисования фонов или спрайтов/тайлов,
//текстура которых использует все изображение целиком.
void DrawRubberPoly (float x,
              float y,
              float dX,
              float dY,
              float delta,
              int center)
 {
  glEnable(GL_TEXTURE_2D);
  glLoadIdentity();
  glTranslatef(x,y,0);//задаем местоположение
  glRotatef(delta,0,0,-1);//поворачиваемся на delta градусов

  //Если поворот происходит относительно центре, то смещаемся еще немного на 0.5dX b 0.5dY
  if (center==1) {glTranslatef(-dX/2,-dY/2,0);}

  //Рисуем затекстурированный полигон, текстурные координаты glTexCoord2i задаются в %/100
  glBegin(GL_QUADS);
   glTexCoord2i(0,0); glVertex2f(0, 0 );// Верхний левый угол
   glTexCoord2i(0,1); glVertex2f(0, dY);//Нижний левый угол
   glTexCoord2i(1,1); glVertex2f(dX,dY);//Нижний правый угол
   glTexCoord2i(1,0); glVertex2f(dX,0 );//Верхний правый угол
  glEnd();

  glLoadIdentity();
 }



//Процедура для рисования затекстурированного полигона текстурой, взятой "кусочком" из большой, общей текстуры.
//Мы, как бы, вырезаем необходимый нам элемент для текстуры из одной большой картинки.Я использую эту процедуру
//для рисования анимированных спрайтов, когда каждый кадр для анимации берется из одной большой текстуры.
void DrawPoly (float W,//ширина большой текстуры, в пикселях
               float H,//высота большой текстуры, в пикселях
               float t_x,//координаты верхнего левого угла "кусочка"
               float t_y,//в большой текстуре, в пикселях
               float t_dx,//размер вырезаемого "кусочка", в пикселях
               float t_dy,

               float x,//аналогично предыдущей процедуре
               float y,
               float dX,
               float dY,
               float delta,
               int center)
 {
  glEnable(GL_TEXTURE_2D);
  glLoadIdentity();
  glTranslatef(x,y,0);
  glRotatef(delta,0,0,-1);

  if (center==1) {glTranslatef(-dX/2,-dY/2,0);}//смещаем по центре

  glBegin(GL_QUADS);
   glTexCoord2f((t_x/W),(t_y/H));               glVertex2f(0, 0); //Верхний левый угол
   glTexCoord2f((t_x/W),((t_y+t_dy)/H));        glVertex2f(0, dY);//Нижний левый угол
   glTexCoord2f(((t_x+t_dx)/W),((t_y+t_dy)/H)); glVertex2f(dX,dY);//Нижний правый угол
   glTexCoord2f(((t_x+t_dx)/W),(t_y/H));        glVertex2f(dX, 0);//Верхний правый угол
  glEnd();

  glLoadIdentity();
 }

//Так как для текстур нужны изображения правильных размеров, лучше всего использовать DrawPoly если
//текстура не квадратная.

void TTF_LoadFont() //инициализация текста
 {
//Массив фонтов, задаем шрифт и болд
  fonts[1]=TTF_OpenFont("data/font/font1.ttf",win_DY/48/*64*/);//открываем фонт 5
  fonts[2]=TTF_OpenFont("data/font/font1.ttf",win_DY/12/*64*/);//открываем фонт 6
  fonts[3]=TTF_OpenFont("data/font/font2.ttf",win_DY/12/*64*/);//открываем фонт 8
 }

/*
FTGLPixmapFont *SmallRusFont = 0;
FTGLPixmapFont *LargeRusFont = 0;
FTGLPixmapFont *LargestRusFont = 0;
//static const int scrHalfWidth = win_DX/2;
//static const int scrHalfHeight = win_DY/2;

void PrintRusText(FTFont* font, const wchar_t* str, float x, float y, bool xCentered, bool yCentered)
 {
  if (font == 0) return;
  if (xCentered || yCentered)
   {
    float llx, lly, llz, upx, upy, upz;
    font->BBox(str, llx, lly, llz, upx, upy, upz);
    if (xCentered) x -= ((upx -llx)/2.0f);
    if (yCentered) y += ((upy -lly)/2.0f);
   }

  glRasterPos2f(x, y);
  font->Render(str);
 }

void Load_RusFonts()
 {
  SmallRusFont = new FTGLPixmapFont("data/font/font4.ttf");
  LargeRusFont = new FTGLPixmapFont("data/font/font4.ttf");
  LargestRusFont = new FTGLPixmapFont("data/font/font5.ttf");

  SmallRusFont->CharMap(ft_encoding_unicode);
  SmallRusFont->FaceSize(win_DY/52);
  LargeRusFont->CharMap(ft_encoding_unicode);
  LargeRusFont->FaceSize(80);
  LargestRusFont->CharMap(ft_encoding_unicode);
  LargestRusFont->FaceSize(160);
 }

void Kill_RusFonts()
 {
  delete SmallRusFont;
  delete LargeRusFont;
  delete LargestRusFont;
 }
*/
//Процедура для создания текстовой текстуры.
//Сперто из исходников sdl-bаll и не совсем допилена, но както работает
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

//Процедура для вывода текста. Текст выводится как полигон с нанесенной текстовой текстурой.
//Все входные параметры аналогичны DrawRubberPoly, за исключением fonts - используемый фонт и
//text - собственно, сам выводимый текст.
void DrawText(float x,//вывод текста
              float y,
              float dX,
              float dY,
              int font_index,
              string text,
              float delta,
              int center)
 {
  SDL_Color txtColorWhite = {255,255,255};//цвет текста по умолчанию белый
  GLuint t_txt;//временная текстовая текстура
  glGenTextures(1, &t_txt);//генерим текстовую текстуру
  GlTxt(dX,
        dY,
        fonts[font_index],
        txtColorWhite,
        //StrToChar(text),
        text.c_str(),
        t_txt);


  //рисуем полигон с текстовой текстурой
  //glEnable(GL_TEXTURE_2D);
  glBindTexture( GL_TEXTURE_2D, t_txt);
  DrawRubberPoly(x,
          y,
          dX,
          dY,
          delta,
          center);

  glDeleteTextures(1, &t_txt);
 }

/*
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
*/

//процедура смены кадра, и контроля
void ControlAnimate(int *frame,//номер текущего кадра
                    int frame_start,//стартовый кадр
                    int frame_finish,//финальный кадр
                    int frame_delta,//шаг между кадрами
                    int *frame_count,//переменная отвечающая за счетчик
                    int delay,//задержка между кадрами в циклах
                    int frame_cikl)//зациклить ли анимацию
 {
  if (*frame_count>=delay)
   {
    *frame_count=0;
    if (frame_start<frame_finish) {*frame=*frame+frame_delta;}//следующий кадр анимации
    if (frame_start>frame_finish) {*frame=*frame-frame_delta;}//следующий кадр анимации
   }

  if (*frame>frame_finish and frame_start<frame_finish or
      *frame<frame_finish and frame_start>frame_finish)
   {
    *frame=frame_finish;
    if (frame_cikl>0) {*frame=frame_start;}//возвращаемся на старый кадр
   }
  *frame_count=*frame_count+1;//счетчик задержки
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
  float x,y,old_x,old_y,
        dx,dy,//координаты и размеры кнопки
        W,H,t_x,t_y,t_dx,t_dy;

  int anim,
      anim_horiz,
      anim_vert,
      anim_start,//стартовый кадр
      anim_finish,//финальный кадр
      anim_delta,//шаг между кадрами
      anim_count,//переменная отвечающая за счетчик
      anim_delay,//задержка между кадрами в циклах
      anim_loop,
      frame;

  bool cursor,//курсор находится на кнопке
       click,//была ли нажата клавиша
       on_screen;//видна ли кнопка на экране

  string name;//имя кнопки
  GLuint texture;//текстура кнопки
 }
Button[button_max];

//Иництализация 1-й кнопки
void InitButton(int button_index,
                string button_text,
                string file_name,
                float x,
                float y,
                float dx,
                float dy,
                bool on_screen)
 {
  Button[button_index].name=button_text;
  load_LINEAR(file_name,Button[button_index].texture);
  Button[button_index].x=x;
  Button[button_index].y=y;
  Button[button_index].dx=dx;
  Button[button_index].dy=dy;
  Button[button_index].on_screen=on_screen;
 }

//Иництализация 1-й кнопки из единой текстуры кнопок
void InitButton(int button_index,
                    string button_text,
                    string file_name,

                    float W,
                    float H,
                    float t_x,
                    float t_y,
                    float t_dx,
                    float t_dy,

                    float x,
                    float y,
                    float dx,
                    float dy,

                    int anim,
                    int anim_horiz,
                    int anim_vert,
                    int anim_start,//стартовый кадр
                    int anim_finish,//финальный кадр
                    int anim_delta,//шаг между кадрами
                    int anim_delay,//задержка между кадрами в циклах
                    int anim_loop,

                    bool on_screen)
 {
  Button[button_index].name=button_text;
  load_LINEAR(file_name,Button[button_index].texture);

  Button[button_index].W=W;
  Button[button_index].H=H;
  Button[button_index].t_x=t_x;
  Button[button_index].t_y=t_y;
  Button[button_index].t_dx=t_dx;
  Button[button_index].t_dy=t_dy;

  Button[button_index].x=x;
  Button[button_index].y=y;
  Button[button_index].dx=dx;
  Button[button_index].dy=dy;

  Button[button_index].anim=anim;//анимация (0-нету,1-приподводе курсора,2-все время)
  Button[button_index].anim_horiz=anim_horiz;
  Button[button_index].anim_vert=anim_vert;
  Button[button_index].anim_start=anim_start;//стартовый кадр
  Button[button_index].anim_finish=anim_finish;//финальный кадр
  Button[button_index].anim_delta=anim_delta;//шаг между кадрами
  Button[button_index].anim_delay=anim_delay;//задержка между кадрами в циклах
  Button[button_index].anim_loop=anim_loop;
  Button[button_index].frame=Button[button_index].anim_start;

  Button[button_index].on_screen=on_screen;
 }


//Обработка нажатий на кнопки
//для целых значений
void KeyButton_vol(int button_index,
                     SDL_Event event,
                     float mouse_X,
                     float mouse_Y,
                     int *click,
                     int volume)//число присваиваемое переменной click после нажатия
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

        *click=volume;
       }
     }
    else Button[button_index].cursor=false;

    //отжимаем клавишу если на ней нету курсора а она нажималась
    if (Button[button_index].cursor==false and
        Button[button_index].click==true)
     {Button[button_index].click=false;}
   }
 }


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
                     float click_limit,//до какого значения - граница
                     float click_val)//чему приравнивать значение после click_limit
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

//Рисуем кнопку
void DrawButton(int button_index,
                float R, float G, float B, float A)
 {
  if (Button[button_index].on_screen==true)//если кнопка рисуется из текстуры
   {
    glColor4d(R,G,B,A);
    glEnable(GL_TEXTURE_2D);

    if (Button[button_index].W==0)//если обычная кнопка
     {
      glBindTexture(GL_TEXTURE_2D, Button[button_index].texture);
      DrawRubberPoly(Button[button_index].x,
              Button[button_index].y,
              Button[button_index].dx-5*Button[button_index].click,
              Button[button_index].dy-5*Button[button_index].click,0,1);
     }

    if (Button[button_index].W>0)//если кнопка анимационная
     {

      if (Button[button_index].anim==3)//если анимация постоянна
       {
	    ControlAnimate(&Button[button_index].frame,//номер текущего кадра
                        Button[button_index].anim_start,//стартовый кадр
                        Button[button_index].anim_finish,//финальный кадр
                        Button[button_index].anim_delta,//шаг между кадрами
                       &Button[button_index].anim_count,//переменная отвечающая за счетчик
                        Button[button_index].anim_delay,//задержка между кадрами в циклах
                        Button[button_index].anim_loop);//зациклить ли анимацию
       }

      glBindTexture(GL_TEXTURE_2D, Button[button_index].texture);
      DrawPoly(Button[button_index].W,
               Button[button_index].H,
               Button[button_index].t_x+(Button[button_index].frame-1)*Button[button_index].t_dx*ZorO(Button[button_index].anim_horiz),
               Button[button_index].t_y+(Button[button_index].frame-1)*Button[button_index].t_dy*ZorO(Button[button_index].anim_vert),
               Button[button_index].t_dx,
               Button[button_index].t_dy,
               Button[button_index].x,
               Button[button_index].y,
               Button[button_index].dx-5*Button[button_index].click,
               Button[button_index].dy-5*Button[button_index].click,0,1);
     }

    glDisable(GL_TEXTURE_2D);
    glColor4d(1,1,1,1);

   }

 }

//подцветка кнопок при наведении мыши
void LightButton(int button_index, GLuint cursor_TX,
                 float R, float G, float B, float A)
 {
  if (Button[button_index].on_screen==true and
      Button[button_index].cursor==true)
   {

    glColor4d(R,G,B,A);
    glEnable(GL_TEXTURE_2D);
    //рисуем курсор панели на кнопке
    glBindTexture( GL_TEXTURE_2D, cursor_TX);
    DrawRubberPoly(Button[button_index].x,
            Button[button_index].y,
            Button[button_index].dx+5,
            Button[button_index].dy+5,0,1);
    glDisable(GL_TEXTURE_2D);
    glColor4d(1,1,1,1);

    if (Button[button_index].anim==1 or //если анимация кнопки происходит при наведении на кнопку
        Button[button_index].anim==2)
     {
	  ControlAnimate(&Button[button_index].frame,//номер текущего кадра
                      Button[button_index].anim_start,//стартовый кадр
                      Button[button_index].anim_finish,//финальный кадр
                      Button[button_index].anim_delta,//шаг между кадрами
                     &Button[button_index].anim_count,//переменная отвечающая за счетчик
                      Button[button_index].anim_delay,//задержка между кадрами в циклах
                      Button[button_index].anim_loop);//зациклить ли анимацию
     }

   }

  if (Button[button_index].cursor==false)//если курсор вне конпки-обнуляем кадр
   {
    if (Button[button_index].anim==1)//после отведения курсорра кажр становится СРАЗУ стартовым
     {
      Button[button_index].frame=Button[button_index].anim_start;
     }

    if (Button[button_index].anim==2 and //после отведения курсорра - обратная анимация до стартового кадра
        Button[button_index].frame>Button[button_index].anim_start)
     {
      ControlAnimate(&Button[button_index].frame,//номер текущего кадра
                      Button[button_index].anim_finish,//стартовый кадр
                      Button[button_index].anim_start,//финальный кадр
                      Button[button_index].anim_delta,//шаг между кадрами
                     &Button[button_index].anim_count,//переменная отвечающая за счетчик
                      Button[button_index].anim_delay,//задержка между кадрами в циклах
                      1);//зациклить ли анимацию
     }
   }

 }

//просто постоянная подцветка кнопок
void ConstLightButton(int button_index, GLuint cursor_TX,
                      float R, float G, float B, float A)
 {
  if (Button[button_index].on_screen==true)
   {
    glColor4d(R,G,B,A);
    glEnable(GL_TEXTURE_2D);
    //рисуем курсор панели на кнопке
    glBindTexture( GL_TEXTURE_2D, cursor_TX);
    DrawRubberPoly(Button[button_index].x,
            Button[button_index].y,
            Button[button_index].dx+5,
            Button[button_index].dy+5,0,1);
    glDisable(GL_TEXTURE_2D);
    glColor4d(1,1,1,1);

    if (Button[button_index].anim==1 or //если анимация кнопки происходит при наведении на кнопку
        Button[button_index].anim==2)
     {
	  ControlAnimate(&Button[button_index].frame,//номер текущего кадра
                      Button[button_index].anim_start,//стартовый кадр
                      Button[button_index].anim_finish,//финальный кадр
                      Button[button_index].anim_delta,//шаг между кадрами
                     &Button[button_index].anim_count,//переменная отвечающая за счетчик
                      Button[button_index].anim_delay,//задержка между кадрами в циклах
                      Button[button_index].anim_loop);//зациклить ли анимацию
     }

   }

  if (Button[button_index].cursor==false)//если курсор вне конпки-обнуляем кадр
   {
    if (Button[button_index].anim==1)//после отведения курсорра кажр становится СРАЗУ стартовым
     {
      Button[button_index].frame=Button[button_index].anim_start;
     }

    if (Button[button_index].anim==2 and //после отведения курсорра - обратная анимация до стартового кадра
        Button[button_index].frame>Button[button_index].anim_start)
     {
      ControlAnimate(&Button[button_index].frame,//номер текущего кадра
                      Button[button_index].anim_finish,//стартовый кадр
                      Button[button_index].anim_start,//финальный кадр
                      Button[button_index].anim_delta,//шаг между кадрами
                     &Button[button_index].anim_count,//переменная отвечающая за счетчик
                      Button[button_index].anim_delay,//задержка между кадрами в циклах
                      1);//зациклить ли анимацию
     }
   }

 }

void Kill_Button(int button_index)
 {
  Button[button_index].on_screen=false;
  Button[button_index].cursor=false;
  Button[button_index].click=false;
  Button[button_index].frame=Button[button_index].anim_start;
 }

void Reset_Button(int button_index)//чтобы при последущем входе не было глюков с отжатием кнопки
 {
  Button[button_index].cursor=false;
  Button[button_index].click=false;
  Button[button_index].frame=Button[button_index].anim_start;
 }

//==[Радиокнопки]==
struct RButton_type//кнопки
 {
  float x,y,old_x,old_y,
        dx,dy,//координаты и размеры кнопки
        W,H,t_x,t_y,t_dx,t_dy;

  int orient,//тип кнопки-вертикальный или горизонтальный
      case_max,//сколько всего случаев начиная с 1 (1,2,3,4,...)
      case_index_cursor,//номер сегмента под курсорм
      case_index,//номер выбранного сегмента
      case_dx,//размер сегмента по горизонтали
      case_dy,//размер сегмента по вертикали

      case_value[999];//значение сегмента

  bool cursor,//курсор находится на кнопке
       on_screen;//видна ли кнопка на экране

  string name;//имя кнопки
  GLuint texture;//текстура кнопки
 }
RButton[button_max];

//Иництализация радио кнопки-вертикальной
void InitRadioButton(int button_index,//номер кнопки-поля
                     int click,//к какой величине привязана
                     string button_text,//текс кнопки
                     string file_name,//имя текстуры
                     int case_max,//количество сегментов
                     int button_type,//тип кнопки-вертикальный или горизонтальный
                     float x,//координаты поля
                     float y,
                     float dx,//размеры поля
                     float dy,
                     bool on_screen)//на экране ли
 {
  RButton[button_index].name=button_text;
  load_LINEAR(file_name,RButton[button_index].texture);
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
  for(int i=1;i<=RButton[button_index].case_max;i++)
   {
    if (RButton[button_index].case_value[i]==click)
     {
      RButton[button_index].case_index=i;//выбранный сегмент
      break;
     }
   } //конец цикла
 }

//Иництализация радио кнопки куском из текстуры кнопок
void InitRadioButton(int button_index,//номер кнопки-поля
                     int click,//к какой величине привязана
                     string button_text,//текс кнопки
                     string file_name,//имя текстуры
                     int case_max,//количество сегментов
                     int button_type,//тип кнопки-вертикальный или горизонтальный

                     float W,
                     float H,
                     float t_x,
                     float t_y,
                     float t_dx,
                     float t_dy,

                     float x,//координаты поля
                     float y,
                     float dx,//размеры поля
                     float dy,
                     bool on_screen)//на экране ли
 {
  RButton[button_index].name=button_text;
  load_LINEAR(file_name,RButton[button_index].texture);
  RButton[button_index].case_max=case_max;
  RButton[button_index].orient=button_type;
  if (button_type==1) {RButton[button_index].case_dy=dy/case_max;}//высота сегмента
  if (button_type==2) {RButton[button_index].case_dx=dx/case_max;}//длина сегмента

  RButton[button_index].W=W;
  RButton[button_index].H=H;
  RButton[button_index].t_x=t_x;
  RButton[button_index].t_y=t_y;
  RButton[button_index].t_dx=t_dx;
  RButton[button_index].t_dy=t_dy;

  RButton[button_index].x=x;
  RButton[button_index].y=y;
  RButton[button_index].dx=dx;
  RButton[button_index].dy=dy;
  RButton[button_index].on_screen=on_screen;

  int i;
  //значение click по умолчанию
  for(int i=1;i<=RButton[button_index].case_max;i++)
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
      for(int i=1;i<=RButton[button_index].case_max;i++)
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

void DrawRadioButton(int button_index,GLuint cursor_TX,
                     float R, float G, float B, float A,
                     float c_R, float c_G, float c_B, float c_A)
 {
  if (RButton[button_index].on_screen==true)
   {
    glColor4d(R,G,B,A);
    glEnable(GL_TEXTURE_2D);

    if (RButton[button_index].W==0)//временно - потом оставить только кнопку из большой текстуры
     {
      glBindTexture(GL_TEXTURE_2D, RButton[button_index].texture);
      DrawRubberPoly(RButton[button_index].x,
              RButton[button_index].y,
              RButton[button_index].dx,
              RButton[button_index].dy,0,1);
     }

    if (RButton[button_index].W>0)
     {
      glBindTexture(GL_TEXTURE_2D, RButton[button_index].texture);
      DrawPoly(RButton[button_index].W,
               RButton[button_index].H,
               RButton[button_index].t_x,
               RButton[button_index].t_y,
               RButton[button_index].t_dx,
               RButton[button_index].t_dy,
               RButton[button_index].x,
               RButton[button_index].y,
               RButton[button_index].dx,
               RButton[button_index].dy,0,1);
      }

    //рисуем курсор на выбранный ранее сегмент
    if (RButton[button_index].case_index>0)
     {
	  glColor4d(c_R,c_G,c_B,c_A);	 
      glBindTexture(GL_TEXTURE_2D, cursor_TX);

      if (RButton[button_index].orient==1)//вертикальный курсор
       {
        DrawRubberPoly(RButton[button_index].x,
                RButton[button_index].y-0.5*RButton[button_index].dy
                                       +(RButton[button_index].case_index-1)*RButton[button_index].case_dy
                                       +0.5*RButton[button_index].case_dy,
                RButton[button_index].dx+5,
                RButton[button_index].case_dy+5,0,1);
       }

      if (RButton[button_index].orient==2)//горизонтальный курсор
       {
        DrawRubberPoly(RButton[button_index].x-0.5*RButton[button_index].dx
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
void LightRadioButton(int button_index, GLuint cursor_TX,
                      float R, float G, float B, float A)
 {
  if (RButton[button_index].on_screen==true and
      RButton[button_index].cursor==true)
   {
    //рисуем курсор панели на сегменте кнопки-поля
    glColor4d(R,G,B,A);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, cursor_TX);

    if (RButton[button_index].orient==1)//вертикальный курсор
     {
      DrawRubberPoly(RButton[button_index].x,
              RButton[button_index].y-0.5*RButton[button_index].dy
                                     +(RButton[button_index].case_index_cursor-1)*RButton[button_index].case_dy
                                     +0.5*RButton[button_index].case_dy,
              RButton[button_index].dx+5,
              RButton[button_index].case_dy+5,0,1);
     }

    if (RButton[button_index].orient==2)//горизонтальный курсор
     {
      DrawRubberPoly(RButton[button_index].x-0.5*RButton[button_index].dx
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


/*

struct ButtonFI_type//кнопки
 {
  float x,y,dx,dy,//координаты и размеры кнопки

  bool cursor,//курсор находится на кнопке
       click,//была ли нажата клавиша
       on_screen;//видна ли кнопка на экране

  string name;//имя кнопки
  GLuint texture;//текстура кнопки
 }
Button[button_max];
*/


void InitSound()//инициировать звук
 {

  // начало инициализации
  if (SDL_Init (SDL_INIT_AUDIO) == -1)
   {
    printf ("Apshipka s SDL_Init: %s\n", SDL_GetError ());
    exit (1);
   }

  //окрываем поток с дискретизацией 44100(первый параметр)
  //герц и 16 бит.
  //Стерео(2 канала 3 параметр)

  if (Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
   {
    printf ("Apshipka s Mix_OpenAudio: %s\n", Mix_GetError ());
    exit (2); //Если ошибка то прога закрывается
   }
  cout << "Init Sound" << endl;
 }

typedef struct
 {
  Mix_Music  *tmp;
 }
MUSIC_type;

MUSIC_type music_pl[100];//музыкальный плэйлист

/*
void loadmusic(char * name, int num)//какой файл запихиваем в массив плэйлиста
 {
  music_pl[num].tmp = Mix_LoadMUS (name);
 }
*/


void loadmusic(string name, int num)//какой файл запихиваем в массив плэйлиста
 {
  music_pl[num].tmp = Mix_LoadMUS (name.c_str());
 }


void music(int num,int num_raz)//играть музыку из плэйлиста(номер массива треков)
 {
  //-1 играть бесконечное число раз
  //0 играть 1 раз
  //1 играть 2 раза
  Mix_PlayMusic (music_pl[num].tmp, num_raz);
 }

void stopmusic()//остановть музыку
 {
  Mix_FadeOutMusic(1000);
 }

typedef struct
 {
  Mix_Chunk  *tmp;
 }
SOUND_type;

SOUND_type sound_pl[100];//звуковой плейлист

void loadsound(string name, int num)//загрузить звук в плэйлист с номером такимто
 {
  sound_pl[num].tmp = Mix_LoadWAV (name.c_str());
 }

void sound (int num)//проиграть звук из плэйлиста с такимто номером
 {
  Mix_PlayChannel (-1, sound_pl[num].tmp, 0);
 }








/*
void hren()
 {
  //Преобразование int в wchar_t
  wchar_t buf[256];
  int her=55;
  swprintf(buf, sizeof(buf), L"%i",her);

  wchar_t wstr[]=L"ХУЙ";
  wchar_t *wszNewStr = new wchar_t[(sizeof(buf)+sizeof(wstr))/sizeof(wchar_t)];
  wcscpy(wszNewStr, wcscat(buf, wstr));


  PrintRusText(SmallRusFont,wszNewStr,mouse_X, mouse_Y, false, false);

 }
*/
