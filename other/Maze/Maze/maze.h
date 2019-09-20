#ifndef MAZE_H
#define MAZE_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <cmath>

#include "square.h"

class Maze : public QWidget
{
    Q_OBJECT

    int cells_h;    // ячеек по горизонтали
    int cells_v;    // ячеек по вертикали

    int width;      // ширина ячейки
    int height;     // высота ячейки

    Square ***cells;    // все ячейки лабиринта
    int **alg_Li;       // массив атрибутов для алгоритма Ли

    bool path_found;    // индикатор того что путь найден

    int x1, y1, x2, y2; // координаты первой и второй точки между которыми ищется кратчайший путь
    int shiftForPoints; // смещение в количестве пикселей для точек

    void generate(int nH, int nV, int sH, int sV); // рекурсивно вызываемая функция при генерации лабиринта
                                                   // nH и nV - количество ячеек по горизонтали и вертикали в области
                                                   // в которой происходит генерация новых комнат
                                                   // sH и sV - смещение области (в количестве ячеек по горизонтали и вертикали)
                                                   // в которой происходит генерация новых комнат
                                                   // относительно нулевой ячейки (верхний левый угол)

    void clearCells();  // очистка поля лабиринта

    void saveAsFile(QString fileName); // функция записывающая в файл лабиринт в виде 20-ти строк
                                       // в каждой строке по 20 чисел разделённых ";" каждое число - состояние ячейки
                                       // подробнее о состояниях см. в реализации функции

    bool paintingMode;  // включен ли режим рисования
public:
    explicit Maze(QWidget *parent = 0);
    ~Maze();

    virtual void paintEvent(QPaintEvent * event);   // обработчик события отрисовки
    virtual void mousePressEvent(QMouseEvent *e);   // срабатывает при нажатии на любую кнопку мыши
    virtual void mouseMoveEvent(QMouseEvent *);     // срабатывает при движении мыши
signals:
    void emitGenerationComplete(); // сигнал отправляется после того как завершится генерация лабиринта

public slots:

    void generate();    // генерация лабиринта
    void pathfinding(); // поиск пути алгоритмом волновой трассировки (алгоритм Ли)

    void save();        // сохранить (в виде текстового файла)
    void saveAs();      // сохранить как (изображение .png или текстовый файл .txt)

    void open(); // открыть

    void paintingModeChanged(bool b) {paintingMode = b;} // смена режима на рисование и обратно
};

#endif // MAZE_H
