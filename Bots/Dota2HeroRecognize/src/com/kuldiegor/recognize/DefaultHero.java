package com.kuldiegor.recognize;

import javax.imageio.ImageIO;
import java.awt.image.BufferedImage;
import java.io.*;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;

/**
 * Created by aeterneus on 17.03.2017.
 */
public class DefaultHero {
    public ArrayList<Hero> heroes;
    public String path;
    public DefaultHero(String path,int tload){
        heroes = new ArrayList<>();
        this.path = path;
        switch (tload){
            case 0:{
                LoadFromFolder(path);
                break;
            }
            case 1:{
                LoadFromFile(path);
            }
        }

    }
    private void LoadFromFolder(String path){
        //Загрузка из каталога изображений с последующем получением свёрток
        File folder = new File(path);
        File[] folderEntries = folder.listFiles();
        for (File entry : folderEntries)
        {
            if (!entry.isDirectory())
            {
                BufferedImage image = null;
                try {
                    image = ImageIO.read(entry);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                Hero hero = new Hero(StringTool.parse(entry.getName(),"",".png"));
                hero.convolutions = new Convolution(image).getConvolutionMatrix();
                heroes.add(hero);


            }
        }

    }
    private void LoadFromFile(String name){
        //Загрузка готовых свёрток из файла
        try {
            BufferedReader bufferedReader = new BufferedReader(new FileReader(name));
            String str;
            while ((str = bufferedReader.readLine())!= null){
                Hero hero =new Hero(StringTool.parse(str,"",":"));
                String s = StringTool.parse(str,":","");
                String mas[] = s.split(";");
                int n=mas.length/48;
                hero.convolutions = new ArrayList<>(n);
                for (int c=0;c<n;c++) {
                    double dmas[] = new double[48];
                    for (int i = 0; i < 48; i++) {
                        dmas[i] = Double.parseDouble(mas[i+c*48]);
                    }
                    hero.convolutions.add(dmas);
                }
                heroes.add(hero);
            }
        } catch (IOException e){
            e.printStackTrace();
        }


    }
    public void SaveToFile(String name){
        //Сохранение свёрток в файл
        Collections.sort(heroes, Comparator.comparing(o -> o.name));
        FileWriter fileWriter = null;
        try {
            fileWriter = new FileWriter(name);
        } catch (IOException e){
            e.printStackTrace();
        }
        for (int i=0;i<heroes.size();i++){
            Hero hero = heroes.get(i);
            StringBuilder stringBuilder = new StringBuilder();
            stringBuilder.append(hero.name).append(":");
            for (int i2=0;i2<hero.convolutions.size();i2++){
                double matrix[] = hero.convolutions.get(i2);
                for (int i3=0;i3<matrix.length;i3++){
                    stringBuilder.append(matrix[i3]).append(";");
                }
            }
            try {
                fileWriter.write(stringBuilder.append("\r\n").toString());
            } catch (IOException e){
                e.printStackTrace();
            }

        }
        try {
            fileWriter.close();
        } catch (IOException e){
            e.printStackTrace();
        }
    }
    public String getSearhHeroName(Hero hero){
        //Поиск и получение имени персонажа
        for (int i=0;i<heroes.size();i++){
            if (equalsHero(hero,heroes.get(i))){
                return heroes.get(i).name;
            }
        }
        return "0";

    }
    public boolean equalsHero(Hero hero1,Hero hero2){
        //Сравнение 2 персонажей
        int min=0;
        int max=0;
        for (int i=0;i<hero1.convolutions.size();i++){
            double average=0;
            for (int i1=0;i1<hero1.convolutions.get(i).length;i1++){
                //Разность 2 сверток по модулю
                average += Math.abs(hero1.convolutions.get(i)[i1]-hero2.convolutions.get(i)[i1]);
            }
            average /=hero1.convolutions.get(0).length;
            if (average<0.02){
                //Если среднее арифметическое меньше порога нахождение то добавляем бал к положительному результату
                min++;
            } else {
                max++;
            }

        }

        return (min>=max);
    }
}
