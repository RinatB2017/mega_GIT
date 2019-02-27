//Разработано aureli0,NeonLight_Child,Ph0Eni}{,Zinoviev54,imanavhod-это все я:)
//http://sourceforge.net/projects/swibplex/
//последний билд от 23.12.2009

void InitSound()//инициировать звук
 {

  // начало инициализации
  if (SDL_Init (SDL_INIT_AUDIO) == -1)
   {
    printf ("SDL_Init: %s\n", SDL_GetError ());
    exit (1);
   }

  //окрываем поток с дискретизацией 44100(первый параметр)
  //герц и 16 бит.
  //Стерео(2 канала 3 параметр)

  if (Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
   {
    printf ("Mix_OpenAudio: %s\n", Mix_GetError ());
    exit (2); //Если ошибка то прога закрывается
   }
  cout << "InitSound-OK" << endl;
 }

typedef struct 
 {
  Mix_Music  *tmp;
 }
MUSIC_type;

MUSIC_type music_pl[100];//музыкальный плэйлист


void loadmusic(char * name, int num)//какой файл запихиваем в массив плэйлиста
 {
  music_pl[num].tmp = Mix_LoadMUS (name);
 }

void music(int num)//играть музыку из плэйлиста(номер массива треков)
 {
  //-1 играть бесконечное число раз
  //0 играть 1 раз 
  //1 играть 2 раза
  Mix_PlayMusic (music_pl[num].tmp, 0);
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


void loadsound(char * name, int num)//загрузить звук в плэйлист с номером такимто
 {
  sound_pl[num].tmp = Mix_LoadWAV (name); 
 }

void sound (int num)//проиграть звук из плэйлиста с такимто номером
 {
  Mix_PlayChannel (-1, sound_pl[num].tmp, 0);
 }
