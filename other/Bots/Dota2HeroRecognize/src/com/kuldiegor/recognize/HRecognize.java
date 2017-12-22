package com.kuldiegor.recognize;

import java.awt.image.BufferedImage;
import java.util.ArrayList;

/**
 * Created by aeterneus on 17.03.2017.
 */
public class HRecognize {
    private DefaultHero defaultHero;
    public String heroes[]; //Список имён распознанных персонажей

    public HRecognize(BufferedImage screen, DefaultHero defaultHero){
        heroes = new String[10];
        ArrayList<Hero> heroArrayList = new ArrayList<>();
        this.defaultHero = defaultHero;
        for (int i=0;i<5;i++){
            //Создаем пустое изображение в режиме градаций серого
            BufferedImage bufferedImage = new BufferedImage(78,53,BufferedImage.TYPE_BYTE_GRAY);
            //Вырезаем изображения из скриншота
            bufferedImage.getGraphics().drawImage(screen.getSubimage(43+i*96,6,78,53),0,0,null);
            Hero hero = new Hero("");
            //Получение свёрток
            hero.convolutions = new Convolution(bufferedImage).getConvolutionMatrix();
            heroArrayList.add(hero);
        }
        for (int i=0;i<5;i++) {
            //Создаем пустое изображение в режиме градаций серого
            BufferedImage bufferedImage = new BufferedImage(78, 53, BufferedImage.TYPE_BYTE_GRAY);
            //Вырезаем изображения из скриншота
            bufferedImage.getGraphics().drawImage(screen.getSubimage(777 + i * 96, 6, 78, 53), 0, 0, null);
            Hero hero = new Hero("");
            //Получение свёрток
            hero.convolutions = new Convolution(bufferedImage).getConvolutionMatrix();
            heroArrayList.add(hero);
        }
        for (int i=0;i<10;i++){
            //Поиск персонажа и получении имени
            heroes[i] = defaultHero.getSearhHeroName(heroArrayList.get(i));
        }


    }
}
