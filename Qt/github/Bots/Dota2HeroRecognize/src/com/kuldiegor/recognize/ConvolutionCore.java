package com.kuldiegor.recognize;

/**
 * Created by aeterneus on 17.03.2017.
 */
public class ConvolutionCore {
    public int unitMin; //количество -1
    public int unitMax; //количество 1
    public  int matrix[][];
    public ConvolutionCore(int width,int height,int haar){
        matrix = new int[height][width];
        unitMax=0;
        unitMin=0;
        switch (haar){
            case 0:{
                // -1 = черная часть
                //  1 = белая часть
                /*
                    -1  -1  -1  1 1 1
                    -1  -1  -1  1 1 1
                    -1  -1  -1  1 1 1
                */
                for (int y=0;y<height;y++){
                    for (int x=0;x<(width/2);x++){
                        matrix[y][x]=-1;
                        unitMin++;
                    }
                    for (int x=width/2;x<width;x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                }
                break;
            }
            case 1:{
                // -1 = черная часть
                //  1 = белая часть
                /*
                    1  1  1  1  1  1
                    1  1  1  1  1  1
                    1  1  1  1  1  1
                   -1 -1 -1 -1 -1 -1
                   -1 -1 -1 -1 -1 -1
                   -1 -1 -1 -1 -1 -1
                */
                for (int y=0;y<(height/2);y++){
                    for (int x=0;x<width;x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                }
                for (int y=(height/2);y<height;y++){
                    for (int x=0;x<width;x++){
                        matrix[y][x]=-1;
                        unitMin++;
                    }
                }
                break;
            }
            case 2:{
                // -1 = черная часть
                //  1 = белая часть
                /*
                    1 1 -1 -1 -1 1 1
                    1 1 -1 -1 -1 1 1
                    1 1 -1 -1 -1 1 1
                */
                for (int y=0;y<height;y++){
                    for (int x=0;x<(width/3);x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                    for (int x=(width/3);x<(width*2/3);x++){
                        matrix[y][x]=-1;
                        unitMin++;
                    }
                    for (int x=(width*2/3);x<width;x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                }
                break;
            }
            case 3:{
                // -1 = черная часть
                //  1 = белая часть
                /*
                    1  1  1  1
                    1  1  1  1
                    1  1  1  1
                   -1 -1 -1 -1
                   -1 -1 -1 -1
                   -1 -1 -1 -1
                    1  1  1  1
                    1  1  1  1
                    1  1  1  1
                */
                for (int y=0;y<(height/3);y++){
                    for (int x=0;x<width;x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                }
                for (int y=(height/3);y<(height*2/3);y++){
                    for (int x=0;x<width;x++){
                        matrix[y][x]=-1;
                        unitMin++;
                    }
                }
                for (int y=(height*2/3);y<height;y++){
                    for (int x=0;x<width;x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                }
                break;
            }
            case 4:{
                // -1 = черная часть
                //  1 = белая часть
                /*
                    1  1  1  1  1
                    1  1  1  1  1
                    1  1  1  1  1
                    1 -1 -1 -1  1
                    1 -1 -1 -1  1
                    1 -1 -1 -1  1
                    1  1  1  1  1
                    1  1  1  1  1
                    1  1  1  1  1
                */
                for (int y=0;y<(height/3);y++){
                    for (int x=0;x<width;x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                }
                for (int y=(height/3);y<(height*2/3);y++){
                    for (int x=0;x<(width/3);x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                    for (int x=(width/3);x<(width*2/3);x++){
                        matrix[y][x]=-1;
                        unitMin++;
                    }
                    for (int x=(width*2/3);x<width;x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                }
                for (int y=(height*2/3);y<height;y++){
                    for (int x=0;x<width;x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                }
                break;
            }
            case 5:{
                // -1 = черная часть
                //  1 = белая часть
                /*
                    1  1  1 -1 -1 -1
                    1  1  1 -1 -1 -1
                   -1 -1 -1  1  1  1
                   -1 -1 -1  1  1  1
                */
                for (int y=0;y<(height/2);y++){
                    for (int x=0;x<(width/2);x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                    for (int x=width/2;x<width;x++){
                        matrix[y][x]=-1;
                        unitMin++;
                    }
                }
                for (int y=(height/2);y<height;y++){
                    for (int x=0;x<(width/2);x++){
                        matrix[y][x]=-1;
                        unitMin++;
                    }
                    for (int x=width/2;x<width;x++){
                        matrix[y][x]=1;
                        unitMax++;
                    }
                }
                break;
            }
        }
    }
}
