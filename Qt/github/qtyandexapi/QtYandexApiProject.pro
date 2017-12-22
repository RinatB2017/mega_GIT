TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    QtYandexApi \
    TestQtYandexApi \
    YandexDictionaryExample

OnlyQtYandexApiLib {
    SUBDIRS -= TestQtYandexApi \
               YandexDictionaryExample
}
