# Dota2HeroRecognize
Recognize hero from Dota2
Распознавание персонажей игры Dota2 используя свёртку изображений.
Для распознавания необходимо создать экземпляр класса DefaultHero. Указав путь до каталога где хранятся изображения образцы (передав вторым параметром 0) или путь до файла где хранятся готовые свёртки (передав вторым параметром 1).
Пример
DefaultHero defaultHero = new DefaultHero("Images/",0);
Или
DefaultHero defaultHero = new DefaultHero("convolutions.txt",1);
Затем создать экземпляр класса HRecognize. Передав в качестве параметра screenshot, и экземпляр DefaultHero.
Пример
HRecognize hRecognize = new HRecognize(image,defaultHero);
Имена распознанных персонажей хранятся в поле heroes[]
Не распознанные персонажи именуются как “0”
