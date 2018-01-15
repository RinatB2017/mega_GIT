#**********************************************************************************
#**                   Author: Bikbao Rinat Zinorovich                            **
#**********************************************************************************

TEMPLATE  = subdirs
CONFIG   *= ordered

SUBDIRS += \
    Cards \
    Chess \
    Game_15 \
    Test_pixmap \

unix {
SUBDIRS += \
    Test_SDL \
    Test_SDLWidget \
    Template_game
}
