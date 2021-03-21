# http://rus-linux.net/MyLDP/algol/install-and-use-gprof.html

# для профилирование нужно просто раскомментировать нижеследующие опции и запустить программу
# в режиме debug. После позапускать то, что следует изучить и закрыть программу.
# В каталоге будет лежать файл gmon.out
# Анализ: gprof my_programm gmon.out > profile.txt
QMAKE_CXXFLAGS_DEBUG    += -pg
QMAKE_CXXFLAGS_MT_DBG   += -pg
QMAKE_LFLAGS_DEBUG      += -pg
