//Разработано aureli0,NeonLight_Child,Ph0Eni}{,Zinoviev54,imanavhod-это все я:)
//http://sourceforge.net/projects/swibplex/
//последний билд от 23.12.2009

const 
 int flame_max=999;//максимальное количество частиц пламени

float flame_X[flame_max+1],
      flame_Y[flame_max+1],
      flame_DX[flame_max+1], 
      flame_DY[flame_max+1],
      flame_life[flame_max+1],
      flame_uskor[flame_max+1],
      flame_fade[flame_max+1],
      flame_rotate[flame_max+1];

bool flame_true[flame_max+1];

GLuint flame_texture;

//Пламя
void Init_Flame()
 {
  load("./data/img/gfx/flame/flame.png",flame_texture);
 }

void Draw_Flame(int win_DX,
                int win_DY,
                int flame_kolvo,
                float X,
                float Y,
                float flame_A,
                float flame_B,
                float flame_C,
                int flame_D,  

                float flame_E,
                float flame_F,//fade
                float flame_G,
                float flame_H,  
                float flame_I,
                float flame_J,

                float x_amp,
                float y_amp,
                float y_n,        
         
                int flame_init_life,
                int flame_init_life_rnd,               
           
                float c_R,
                float c_G,
                float c_B,
                int var_color)                
 {
  for(i=1; i <= flame_kolvo; i++)  
   {
    if (flame_life[i]>0)
     {
      flame_Y[i]=flame_Y[i]-flame_A*flame_uskor[i];
      flame_life[i]=flame_life[i]-flame_B;
      flame_uskor[i]=flame_uskor[i]+flame_C*(rand()%flame_D);
      flame_fade[i]=flame_fade[i]-flame_E*flame_uskor[i];
      if (flame_X[i]>win_DX/2)
        {flame_rotate[i]=flame_rotate[i]+flame_G*flame_uskor[i];}
      if (flame_X[i]<win_DX/2)
        {flame_rotate[i]=flame_rotate[i]-flame_G*flame_uskor[i];}
      
      flame_DX[i]=flame_H*(flame_life[i]);//0.5
      if (flame_DX[i]<flame_J) {flame_DX[i]==flame_J;} 

      flame_DY[i]=flame_I*(flame_life[i]);//0.3
      if (flame_DY[i]<flame_J) {flame_DY[i]==flame_J;} 

      glBlendFunc(GL_SRC_ALPHA,GL_ONE);
      glEnable(GL_TEXTURE_2D);
      glBindTexture( GL_TEXTURE_2D,flame_texture); 
    
      if (var_color==1)
       {
        glColor4d(flame_uskor[i]*flame_F,
                  c_G,
                  c_B,
                  flame_fade[i]);
       } 
      if (var_color==2)
       {
        glColor4d(c_R,
                  flame_uskor[i]*flame_F,
                  c_B,
                  flame_fade[i]);
       } 
      if (var_color==3)
       {
        glColor4d(c_R,
                  c_G,
                  flame_uskor[i]*flame_F,
                  flame_fade[i]);
       } 

      DrawTXT (flame_X[i]+y_amp*sin(y_n*flame_Y[i]),
               flame_Y[i],
               flame_DX[i],
               flame_DY[i],
               flame_rotate[i],
               1);
     }
   }

  for(i=1; i <= flame_kolvo; i++)  
   {
    if (flame_life[i]<=0)
     {
      flame_X[i]=X-x_amp*(rand()%win_DX)+x_amp*(rand()%win_DX);
      flame_Y[i]=Y;
      flame_uskor[i]=1;
      flame_fade[i]=1;
      flame_life[i]=flame_init_life+(rand()%flame_init_life_rnd);
      break;
     }
   } 

 }

void Flame()//завязана с жизню колобка
 {
  if (flame_summ>0)
   {
    Draw_Flame(win_DX,
               win_DY,
               flame_summ,//flame_kolvo
               win_DX/2,//X,
               win_DY,//Y,
               0.01,//flame_A
               1,//flame_B
               0.08,//flame_C
               12, //flame_D
               0.000026*option_show+0.0000014*(p_live+15)*game_show,//(p_live)*game_show,//0.000026,//flame_E

               0.0025,//flame_F

               0.0003,//flame_G
               0.2*option_show+0.0035*(140-p_live)*game_show,//flame_H//0.2
               0.3*option_show+0.0035*(110-p_live)*game_show,//flame_I//0.3
               1.3,//flame_J

               0.5,//x_amp
               1,//y_amp
               0.1,//y_n

               400,// flame_init_life,
               450,

               1,//c_R
               0,//c_G
               0,//c_B
               2);//изменения в c_B

    glColor4f(1,1,1,1);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_TEXTURE_2D);
   }
 }

//Эффекты
void Effect_Draw()//Рисуем эффекты
 {  
  for(i=0;i<eff_num_max;i++)
   {
    if (eff_true[i]==true)
     {
      if  (eff_type[i]==1)//молния
       {
        ControlAnimate(&eff_frame[i],//номер текущего кадра
                       1,//стартовый кадр
                       16,//финальный кадр
                       1,//шаг между кадрами
                       &eff_frame_count[i],//переменная отвечающая за счетчик
                       1,//задержка между кадрами в циклах
                       false);//зациклить ли анимацию

        if (eff_frame[i]>=16) {eff_true[i]=false;}//16 кадров молнии
       }

      if  (eff_type[i]==2)//взрыв
       {
        ControlAnimate(&eff_frame[i],
                       1,
                       20,
                       1,
                       &eff_frame_count[i],
                       1,
                       false);
        if (eff_frame[i]>=20) {eff_true[i]=false;}//20 кадров взрыва
       }

      if  (eff_type[i]==3)//кровища
       {
        ControlAnimate(&eff_frame[i],
                       1,
                       8,
                       1,
                       &eff_frame_count[i],
                       2,
                       false);
        if (eff_frame[i]>=8) {eff_true[i]=false;}//8 кадров крови
       }
       
      if (diff_game_mode==1 and //при обычном режиме сложности
          eff_x[i]+x_cam>0-eff_x[i] and
          eff_x[i]+x_cam<win_DX+eff_x[i] and 
          eff_y[i]+y_cam>0-eff_y[i] and
          eff_y[i]+y_cam<win_DY+eff_y[i] or

          diff_game_mode==2 and //при повышенном режиме сложности
          dark_matrix[eff_x[i]/grid][eff_y[i]/grid]==0 and
          eff_x[i]+x_cam>0-eff_x[i] and
          eff_x[i]+x_cam<win_DX+eff_x[i] and 
          eff_y[i]+y_cam>0-eff_y[i] and
          eff_y[i]+y_cam<win_DY+eff_y[i] or

          diff_game_mode==2 and //при повышенном режиме сложности
          eff_x[i]>(p_x-dark_radius)*grid and
          eff_x[i]<(p_x+dark_radius)*grid and 
          eff_y[i]>(p_y-dark_radius)*grid and
          eff_y[i]<(p_y+dark_radius)*grid) 
       {
        glBlendFunc(GL_SRC_ALPHA,GL_ONE);
        glColor4f(eff_cR[i], eff_cG[i],eff_cB[i],eff_cAlpha[i]);
        glBindTexture(GL_TEXTURE_2D,eff_texture[eff_type[i]][eff_frame[i]]);
        DrawTXT(eff_x[i]+x_cam,
                eff_y[i]+y_cam,
                eff_W[i],
                eff_H[i],0,1); 
        glColor4f(1,1,1,1);
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
       }
     }//конец условия 
   }//конец цикла i
 }

//Тьма
int x,y,z;
void Dark_Refresh()
 {
  for(x=0; x <= lev_width; x++) 
  for(y=0; y <= lev_height;y++)
   {
    dark_matrix[x][y]=1;
   }

  //там где стоя лампы тьму убираем 
  for(x=0;x<lev_num_max;x++) 
   {
    if (lev[x].unit==31)//лампочка вкл
     {
      for(y=lev[x].x-lev_dark_radius+1; y <= lev[x].x+lev_dark_radius-1; y++) 
      for(z=lev[x].y-lev_dark_radius+1; z <= lev[x].y+lev_dark_radius-1; z++) 
       {
        dark_matrix[y][z]=0;
       }
     } 
   }
 }

void Dark_Init()//заполняем уровень тьмой
 {
  if (diff_game_mode==2)//сложный ночной режим
   {
    Dark_Refresh();
   }
 }
 
void Dark_Draw()//Рисуем тьму
 {
  if (diff_game_mode==2)//сложный ночной режим
   {
    for(i=0; i <= lev_width; i++) 
    for(j=0; j <= lev_height; j++)
     { 
      if (dark_matrix[i][j]==1 and //если есть темнота по матрице
          i*grid+x_cam>0-grid and//условие видимости по тем элементам что видны на экране
          i*grid+x_cam<win_DX+grid and 
          j*grid+y_cam>0-grid and
          j*grid+y_cam<win_DY+grid) 
       {
        if (i<p_x-dark_radius and//1 //вокруг Свиборга свет
            j<=p_y-dark_radius or
  
            i>=p_x-dark_radius and//2
            i<=p_x+dark_radius and 
            j<=p_y-dark_radius or

            i>p_x+dark_radius and//3
            j<=p_y-dark_radius or

            i<=p_x-dark_radius and//8
            j>p_y-dark_radius and
            j<p_y+dark_radius or

            i>=p_x+dark_radius and//4
            j>p_y-dark_radius and
            j<p_y+dark_radius or

            i<p_x-dark_radius and//7
            j>=p_y+dark_radius or
  
            i>=p_x-dark_radius and//6
            i<=p_x+dark_radius and 
            j>=p_y+dark_radius or

            i>p_x+dark_radius and//5
            j>=p_y+dark_radius)

         {
          glColor4f(1,1,1,0.8);
          glBindTexture(GL_TEXTURE_2D,dark_texture);
          DrawTXT (i*grid+x_cam,
                   j*grid+y_cam,
                   grid,grid,0,1); 
          glColor4f(1,1,1,1); 
         }
       }
     }
   }
 }

