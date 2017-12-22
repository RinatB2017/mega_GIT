#include "Precompiled.h"

#include <QtYandexApi/QtYandexApi.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QtYandexDictionary yandexDictionary(QtYandexApi::getYandexKeyFromFile("dictKey"), new QNetworkAccessManager(&a));
    QObject::connect(&yandexDictionary, &QtYandexDictionary::translated,
                     [](const QtYaWordTranslation& wordTranslation)
    {
        if (wordTranslation.isError())
            qDebug() << wordTranslation.errorString();
        else {
            QtYaWord wordForTranslation = wordTranslation.wordForTranslation();
            QtYaTranslatedWord translatedWord = wordTranslation.translatedWord();
            qDebug() << "\n***************";
            qDebug() << "Word: " << wordForTranslation.wordName();
            qDebug() << "Direction: " << wordForTranslation.fromLanguage() << "-" << wordForTranslation.toLanguage();
            qDebug() << "Main translation: " << translatedWord.mainTranslation();
            qDebug() << "Synonyms: " << translatedWord.synonyms();
            qDebug() << "Examples: ";
            for (const auto& example : translatedWord.examples()) {
                qDebug() << example.first << "-" << example.second;
            }
        }
    });

    QStringList russianWords, englishWords;
    russianWords << "дом" << "время" << "легенда" << "ключ" << "клавиатура" << "монитор" << "случай" << "один" << "два" << "три" << "четыре" << "пять" << "шесть";
    englishWords << "home" << "time" << "legend" << "key" << "keyboard" << "monitor" << "infection" << "one" << "two" << "three" << "four" << "five" << "success";

    for (const QString& word : russianWords) {
        yandexDictionary.translate(QtYaWord(word, "ru", "en"));
    }
    for (const QString& word : englishWords) {
        yandexDictionary.translate(QtYaWord(word, "en", "ru"));
    }

    return a.exec();
}
