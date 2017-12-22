package com.kuldiegor.recognize;

import java.awt.image.BufferedImage;
import java.util.ArrayList;

/**
 * Created by aeterneus on 17.03.2017.
 */
public class Convolution {
    static ConvolutionCore convolutionCores[]; //Ядра свертки
    static {
        //Добавляем все признаки Хаара
        convolutionCores = new ConvolutionCore[6];
        for (int i=0;i<6;i++){
            convolutionCores[i] = new ConvolutionCore(10,10,i);
        }
    }

    private int matrixx[][]; //Матрица значений изображения 0 .. 255

    public Convolution(BufferedImage image){
        matrixx = getReadyMatrix(image);

    }
    private int[][] getReadyMatrix(BufferedImage bufferedImage){
        //Получение матрицы значений из изображения
        int width  = bufferedImage.getWidth();
        int heigth = bufferedImage.getHeight();

        int[] lineData = new int[width * heigth];
        bufferedImage.getRaster().getPixels(0, 0, width, heigth, lineData);

        int[][] res = new int[heigth][width];
        int shift = 0;
        for (int row = 0; row < heigth; ++row) {
            System.arraycopy(lineData, shift, res[row], 0, width);
            shift += width;
        }

        return res;
    }
    private double[] ColapseMatrix(int[][] matrix,ConvolutionCore convolutionCore){
        //Произведение матрицы на ядро свёртки
        int cmh=convolutionCore.matrix.length; //Высота ядра свёртки
        int cmw=convolutionCore.matrix[0].length; //Ширина ядра свёртки
        int mh=matrix.length; //Высота матрицы значений
        int mw=matrix[0].length; //Ширина матрицы значений
        int addWidth = cmw - (mw%cmw); //В случае если ядро ровно не ложится, то добавляем нули в матрицу
        int addHeight = cmh - (mh%cmh);
        int nmatrix[][]=new int[mh+addHeight][mw+addWidth];
        for (int row = 0; row < mh; row++) {
            System.arraycopy(matrix[row], 0, nmatrix[row], 0, mw);
        }
        int nw = nmatrix[0].length/cmw;
        int nh = nmatrix.length/cmh;
        double result[] = new double[nh*nw];
        int dmin = -convolutionCore.unitMin*255; //Для нормализации значений
        int dm = convolutionCore.unitMax*255-dmin;
        int q=0;
        for (int ny=0;ny<nh;ny++){
            for (int nx=0;nx<nw;nx++){
                int sum=0;
                for (int y=0;y<cmh;y++){
                    for (int x=0;x<cmw;x++){
                        sum += nmatrix[ny*cmh+y][nx*cmw+x]*convolutionCore.matrix[y][x];
                    }
                }
                result[q++]=((double)sum-dmin)/dm;
            }
        }
        return result;
    }
    public ArrayList<double[]> getConvolutionMatrix(){
        //Создаём массив сверток по всем признакам Хаара
        ArrayList<double[]> result = new ArrayList<>();
        for (int i=0;i<convolutionCores.length;i++){
            result.add(ColapseMatrix(matrixx,convolutionCores[i]));
        }
        return result;
    }
}
