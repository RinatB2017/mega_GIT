TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

#BOOK REFERED : DATA STRUCTURES AND
#               PROBLEM SOLVING USING C++ by MARK ALLEN WElSS


LESSON_1_SOURCES =  \
                     ../../learn_vectors2.cpp \
                     ../../learn_vectors1.cpp \
                     ../../learn_string.cpp \
                     ../../learn_new.cpp \
                     ../../shallowVsDeepCopy.cpp

LESSON_2_SOURCES  = \
                     ../../lesson2_main.cpp \
                     ../../whatIsClass.cpp

SOURCES += $$LESSON_2_SOURCES \
    ../../rationalNumberClass.cpp

HEADERS += \
    ../../whatIsClass.h \
    ../../rationalNumberClass.h



