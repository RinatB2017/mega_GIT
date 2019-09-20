#include "maze.h"

Maze::Maze(QWidget *parent) :
    QWidget(parent)
{
    paintingMode = false;

    cells_h = 20;
    cells_v = 20;

    width = 20;
    height = 20;

    this->resize(cells_h * height, cells_v * width);

    shiftForPoints = 2;

    x1 = shiftForPoints;
    y1 = shiftForPoints;
    x2 = cells_h * width - (width - shiftForPoints);
    y2 = cells_v * height - (height - shiftForPoints);

    path_found = false;

    // Выделяем память
     alg_Li = new int*[cells_h];
     for (int i = 0; i < cells_h; i++)
         alg_Li[i] = new int[cells_v];

     cells = new Square** [cells_h];
     for (int i = 0; i < cells_h; i++)
         cells[i] = new Square* [cells_v];

     for (int i = 0; i < cells_v; i++)
        for (int j = 0; j < cells_h; j++)
            cells[i][j] = new Square(0, 0, 0, 0);

     // приводим ячейки к виду пустого лабиринта
     clearCells();

     // говорим отслеживать движения мыши
     setMouseTracking(true);
}

Maze::~Maze() {
    // Освобождаем выделенную ранее память
    for (int i = 0; i < cells_h; i++)
        for (int j =0; j < cells_v; j++)
            delete cells[i][j];

    for (int i = 0; i < cells_h; i++) {
       delete [] cells[i];
    }
    delete [] cells;

    for (int i = 0; i < cells_h; i++) {
        delete [] alg_Li[i];
    }
    delete [] alg_Li;
}

void Maze::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);

    //  рисуем фоновый прямоугольник
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, this->size().width(), this->size().height());

    // рисуем ячейки
    painter.setPen(QPen(Qt::blue, 2));
    for (int i = 0; i < cells_v; i++)
        for (int j = 0; j < cells_h; j++) {
            if ( cells[i][j]->get_left_edge() )
                painter.drawLine(j * width,  i * height, j * width,  (i + 1) * height);
            if ( cells[i][j]->get_top_edge() )
                painter.drawLine(j * width,  i * height, (j + 1) * width, i * height);
            if ( cells[i][j]->get_right_edge() )
                painter.drawLine((j + 1) * width,  i * height, (j + 1) * width, (i + 1) * height);
            if ( cells[i][j]->get_bottom_edge() )
                painter.drawLine(j * width,  (i + 1) * height, (j + 1) * width, (i + 1) * height);
        }

    // рисуем сетку врежиме редактирования
    if (paintingMode) {

        // определяем положение курсора
        QPoint p = this->mapFromGlobal(QCursor::pos());
        int tmpX, tmpY;
        tmpX = p.x() - p.x() % cells_h;
        tmpY = p.y() - p.y() % cells_v;

        // сетка
        painter.setPen(QPen(Qt::gray, 0.2));
        for (int i = 0; i < cells_v; i++)
            for (int j = 0; j < cells_h; j++) {
                painter.drawLine((j + 1) * width,  i * height, (j + 1) * width, (i + 1) * height);
                painter.drawLine(j * width,  (i + 1) * height, (j + 1) * width, (i + 1) * height);
            }

        // вспомогательные палочки
        if  ( fabs(p.x() - tmpX) < 5 && fabs(p.y() - tmpY) > 5) {
             painter.setPen(QPen(Qt::red, 2));
             painter.drawLine(tmpX, tmpY, tmpX, tmpY + height);
         }

        if  ( fabs(p.y() - tmpY) < 5 && fabs(p.x() - tmpX) > 5) {
             painter.setPen(QPen(Qt::red, 2));
             painter.drawLine(tmpX, tmpY, tmpX + width, tmpY);
         }
    }


    // если найден путь между точками, рисуем его
    painter.setPen(QPen());
    painter.setBrush(QBrush(Qt::red, Qt::Dense4Pattern));
    if ( path_found )
        for (int i = 0; i < cells_v; i++)
            for (int j = 0; j < cells_h; j++)
                if (alg_Li[i][j] == -3)
                    painter.drawRect(j * width + 4, i * height + 4, 11, 11);

    // рисуем две точки
    painter.setPen(QPen(Qt::green, 1));
    painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    painter.drawEllipse(x1, y1, 15, 15);

    painter.setPen(QPen(Qt::yellow, 1));
    painter.setBrush(QBrush(Qt::gray, Qt::SolidPattern));
    painter.drawEllipse(x2, y2, 15, 15);

}

void Maze::generate() {
    clearCells();
    path_found = false;
    generate(cells_h, cells_v, 0, 0);
    emitGenerationComplete();
    repaint();
}

void Maze::generate(int nH, int nV, int sH, int sV) {

    // 1) если комната в длинну или ширину равна 1 ячейке, то внутри уже нельзя ничего генерировать => выходим
    if ( (nH == 1 || nV == 1) )
        return;

    // 2) если все стороны больше чем в две ячейки, продолжаем выполнять всё что ниже

    // находим номера ячеек по которым будут проходить границы
    int indexH, indexV;
    if (nH == 2)
        indexH = 0;
    else
        indexH = qrand() % (nH - 1);

    if (nV == 2)
        indexV = 0;
    else
        indexV = qrand() % (nV - 1);

    // разбиваем комнату на четыре сектора
    for (int i = 0; i < nV; i++)
        cells[sV + i][indexH + sH]->set_right_edge(true);

    for (int i = 0; i < nH; i++)
        cells[indexV + sV][sH + i]->set_bottom_edge(true);


    // находим номера ячеек на границе в которых делаем отверстия
    int holeH, holeV;

    if ( indexH == 0 ) {
        holeH = 0;
        cells[indexV + sV][holeH + sH]->set_bottom_edge(false);
        holeH = 1;
        cells[indexV + sV][holeH + sH]->set_bottom_edge(false);
    } else if ( nH - indexH == 2 ) {
        holeH = qrand() % (indexH + 1);
        cells[indexV + sV][holeH + sH]->set_bottom_edge(false);
        holeH = indexH + 1;
        cells[indexV + sV][holeH + sH]->set_bottom_edge(false);
    } else {
        holeH = qrand() % (indexH + 1);
        cells[indexV + sV][holeH + sH]->set_bottom_edge(false);
        holeH = qrand() % (nH - indexH - 2) + indexH + 1;
        cells[indexV + sV][holeH + sH]->set_bottom_edge(false);
    }

    if ( indexV == 0 ) {
        holeV = 0;
        cells[holeV + sV][indexH + sH]->set_right_edge(false);
        holeV = 1;
        cells[holeV + sV][indexH + sH]->set_right_edge(false);
    } else if ( nV - indexV == 2 ) {
        holeV = qrand() % (indexV + 1);
        cells[holeV + sV][indexH + sH]->set_right_edge(false);
        holeV = indexV + 1;
        cells[holeV + sV][indexH + sH]->set_right_edge(false);
    } else {
        holeV = qrand() % (indexV + 1);
        cells[holeV + sV][indexH + sH]->set_right_edge(false);
        holeV = qrand() % (nV - indexV - 2 ) + indexV + 1;
        cells[holeV + sV][indexH + sH]->set_right_edge(false);
    }

    // закрываем один из проходов
    switch (qrand() % 4) {
        case 0: // горизонтальный слева
            for (int i = sH; i < sH + indexH + 1; i++)
                cells[indexV + sV][i]->set_bottom_edge(true);
            break;
        case 1: // горизонтальный справа
            for (int i = 0; i < nH - indexH - 1; i++)
                cells[indexV + sV][indexH + 1 + i + sH]->set_bottom_edge(true);
            break;
        case 2: // вертикальный сверху
            for (int i = sV; i < sV + indexV + 1; i++)
                cells[i][indexH + sH]->set_right_edge(true);
            break;
        case 3: // вертикальный снизу
            for (int i =  0; i < nV - indexV - 1; i++)
                cells[indexV + 1 + i + sV][indexH + sH]->set_right_edge(true);
            break;
    }

    // рекурсия в нижний правый отсек
    generate(nH - indexH - 1, nV - indexV - 1, sH + indexH + 1 , sV + indexV + 1);
    // рекурсия в верхний левый отсек
    generate(indexH + 1, indexV + 1, sH, sV);
    // рекурсия в верхний правый отсек
    generate(nH - indexH - 1, indexV + 1, sH + indexH + 1, sV);
    // рекурсия в нижний левый отсек
    generate(indexH + 1, nV - indexV - 1, sH, sV + indexV + 1);
}

void Maze::mousePressEvent(QMouseEvent *e) {
    int tmpX, tmpY;

    if (!paintingMode) {
        switch (e->button()) {
            case 1: // левая кнопка мыши
                tmpX = e->pos().x() - e->pos().x() % cells_h + shiftForPoints;
                tmpY = e->pos().y() - e->pos().y() % cells_v + shiftForPoints;
                if (tmpX == x2 && tmpY == y2) {
                    x2 = x1;
                    y2 = y1;
                    x1 = tmpX;
                    y1 = tmpY;
                } else {
                    x1 = e->pos().x() - e->pos().x() % cells_h + shiftForPoints;
                    y1 = e->pos().y() - e->pos().y() % cells_v + shiftForPoints;
                }
                break;
            case 2: // правая кнопка мыши
                tmpX = e->pos().x() - e->pos().x() % cells_h + shiftForPoints;
                tmpY = e->pos().y() - e->pos().y() % cells_v + shiftForPoints;
                if (tmpX == x1 && tmpY == y1) {
                    x1 = x2;
                    y1 = y2;
                    x2 = tmpX;
                    y2 = tmpY;
                } else {
                    x2 = e->pos().x() - e->pos().x() % cells_h + shiftForPoints;
                    y2 = e->pos().y() - e->pos().y() % cells_v + shiftForPoints;
                }
                break;
        }
    } else {
            switch (e->button()) {
                case 1: // левая кнопка мыши
                    tmpX = e->pos().x() - e->pos().x() % cells_h;
                    tmpY = e->pos().y() - e->pos().y() % cells_v;
                    if  ( fabs(e->pos().x() - tmpX) < 5 && fabs(e->pos().y() - tmpY) > 5) {
                        cells[tmpY / height][tmpX / width - 1]->set_right_edge(true);
                    }
                    if  ( fabs(e->pos().y() - tmpY) < 5 && fabs(e->pos().x() - tmpX) > 5) {
                        cells[tmpY / height - 1][tmpX / width ]->set_bottom_edge(true);
                    }
                    break;
                case 2: // правая кнопка мыши
                    tmpX = e->pos().x() - e->pos().x() % cells_h;
                    tmpY = e->pos().y() - e->pos().y() % cells_v;
                    if  ( fabs(e->pos().x() - tmpX) < 5 && fabs(e->pos().y() - tmpY) > 5) {
                        cells[tmpY / height][tmpX / width - 1]->set_right_edge(false);
                    }
                    if  ( fabs(e->pos().y() - tmpY) < 5 && fabs(e->pos().x() - tmpX) > 5) {
                        cells[tmpY / height - 1][tmpX / width ]->set_bottom_edge(false);
                    }
                    break;
            }
    }

    repaint();
}

void Maze::mouseMoveEvent(QMouseEvent *)
{
    repaint();
}

void Maze::clearCells() {
    for (int i = 0; i < cells_v; i++)
       for (int j = 0; j < cells_h; j++) {
           cells[i][j]->set_left_edge(false);
           cells[i][j]->set_top_edge(false);
           cells[i][j]->set_right_edge(false);
           cells[i][j]->set_bottom_edge(false);

           if (i == 0)
               cells[i][j]->set_top_edge(true);
           else if (i == cells_v - 1)
               cells[i][j]->set_bottom_edge(true);

           if (j == 0)
               cells[i][j]->set_left_edge(true);
           else if (j == cells_h - 1)
               cells[i][j]->set_right_edge(true);
        }
}

void Maze::pathfinding() {

    for (int i = 0; i < cells_v; i++)
        for (int j = 0; j < cells_h; j++)
            alg_Li[i][j] = -1; // так помечены все ячейки через которые мы не прошли


    alg_Li[ y1 / height ][ x1 / width ] = 0;    // ячейка первой точки
    alg_Li[ y2 / height ][ x2 / width ] = - 2;  // ячейка второй точки

    int attribute = 0;

    path_found = false;

    // распространение волны
    while ( !path_found ) {

        for (int i = 0; i < cells_v; i++)
            for (int j = 0; j < cells_h; j++)
                if (alg_Li[i][j] == attribute) {
                    // ячейка сверху
                    if (i != 0)
                        if (!cells[i - 1][j]->get_bottom_edge()) {
                            if (alg_Li[i - 1][j] == -1)
                                alg_Li[i - 1][j] = attribute + 1;
                            else if (alg_Li[i - 1][j] == -2) {
                                path_found = true;
                                alg_Li[i - 1][j] = attribute + 1;
                            }
                        }
                    // ячейка слева
                    if (j != 0)
                        if (!cells[i][j - 1]->get_right_edge()) {
                            if (alg_Li[i][j - 1] == -1)
                                alg_Li[i][j - 1] = attribute + 1;
                            else if (alg_Li[i][j - 1] == -2) {
                                path_found = true;
                                alg_Li[i][j - 1] = attribute + 1;
                            }
                        }
                    // ячейка снизу
                    if (i != cells_v - 1)
                        if (!cells[i][j]->get_bottom_edge()) {
                            if (alg_Li[i + 1][j] == -1)
                                alg_Li[i + 1][j] = attribute + 1;
                            else if (alg_Li[i + 1][j] == -2) {
                                path_found = true;
                                alg_Li[i + 1][j] = attribute + 1;
                            }
                        }
                    // ячейка справа
                    if (j != cells_h + 1)
                         if (!cells[i][j]->get_right_edge()) {
                            if (alg_Li[i][j + 1] == -1)
                                alg_Li[i][j + 1] = attribute + 1;
                            else if (alg_Li[i][j + 1] == -2) {
                                path_found = true;
                                alg_Li[i][j + 1] = attribute + 1;
                            }
                        }          
            }

        // проверка на то есть ли дальше возможность для распространения волны, если нет и путь всё ещё не найден,
        // то решения у лабиринта нет
        int maxAttribute = 0; // текущее максимальное значение в массиве alg_Li
                                        // если оно равно текущему атрибуту attribute, то волна не смогла распространиться
        for (int i = 0; i < cells_v; i++)
            for (int j = 0; j < cells_h; j++) {
                if (maxAttribute <  alg_Li[i][j])
                    maxAttribute =  alg_Li[i][j];
            }

        if (maxAttribute == attribute) {
            QMessageBox msgBox;
            msgBox.setText("У лабиринта нет решения!");
            msgBox.exec();
            return;
        }
        attribute++;
    }

    // востанавливаем путь: все ячейки обозначющие путь отмечаем индексом "-3"
    int i =  y2 / height;
    int j =  x2 / width;
    alg_Li[ i ][ j ] = -3;

    while (attribute != 0) {

        attribute--;
        // ячейка сверху
        if (i != 0)
            if (!cells[i - 1][j]->get_bottom_edge()) {
                if (alg_Li[i - 1][j] == attribute) {
                    alg_Li[i - 1][j] = -3;
                    i = i - 1;
                    continue;
                }
            }
        // ячейка слева
        if (j != 0)
            if (!cells[i][j - 1]->get_right_edge()) {
                if (alg_Li[i][j - 1] == attribute) {
                    alg_Li[i][j - 1] = -3;
                    j = j - 1;
                    continue;
                }
            }
        // ячейка снизу
        if (i != cells_v - 1)
            if (!cells[i][j]->get_bottom_edge()) {
                if (alg_Li[i + 1][j] == attribute) {
                    alg_Li[i + 1][j] = -3;
                    i = i + 1;
                    continue;
                }
            }
        // ячейка справа
        if (j != cells_h + 1)
            if (!cells[i][j]->get_right_edge()) {
                if (alg_Li[i][j + 1] == attribute) {
                    alg_Li[i][j + 1] = -3;
                    j = j + 1;
                    continue;
                }
            }
    }

    repaint();
}

void Maze::saveAsFile(QString fileName) {
    QFile file(fileName);

    // если не удалось открыть/создать файл для записи показываем окошко с сообщением об ошибке.
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox msgBox;
        msgBox.setText("Не удалось сохранить файл.\nВозможно отсутствуют права на запись.");
        msgBox.exec();
        return;
    }

    // если же файл для записи удалось открыть/создать, то записываем в него информацию о лабиринте
    // для этого создаём пооток ввода-вывода
    QTextStream stream(&file); // поток ввода-вывода текста

    /* лабиринт состоит из 400 квадратных ячеек (20 x 20)
     * каждая ячейка может находится в одном из 15-ти состояний
     * 0 - нет граней
     * 1 - есть только верхняя +
     * 2 - только правая +
     * 3 - только нижняя +
     * 4 - только левая
     * 5 - есть верхняя и правая +
     * 6 - верхняя и нижняя +
     * 7 - верхняя и левая +
     * 8 - левая и правая +
     * 9 - левая и нижняя
     * 10 - правая и нижняя +
     * 11 - есть левая, верхняя и правая +
     * 12 - верхняя, правая и нижняя +
     * 13 - правая, нижняя и левая +
     * 14 - нижняя, левая и верхняя +
     * 15 - есть все грани
    */

    QString stringOfStates; // строка сосотяний 20-ти ячеек, выглядит после заполнения примерно 4;2;13;2;6;2;3;2;4;4;2;12;2;2;14;2;1;2;3;2;
                                      // в файл запишутся 20 таких строк
    // пробегаем по всем ячейкам определяя их принадлежность к одному из 15-ти состояний
    for (int i = 0; i < cells_v; i++) {
        stringOfStates = ""; // при переходе на новую строку ячеек, обнуляем строку сосотояний

        for (int j = 0; j < cells_h; j++) {
            if (cells[i][j]->get_top_edge()) {
                if (!cells[i][j]->get_left_edge() && !cells[i][j]->get_right_edge() && !cells[i][j]->get_bottom_edge())
                    stringOfStates += "1;";
                else if (!cells[i][j]->get_left_edge() && cells[i][j]->get_right_edge() && !cells[i][j]->get_bottom_edge())
                    stringOfStates += "5;";
                else if (!cells[i][j]->get_left_edge() && !cells[i][j]->get_right_edge() && cells[i][j]->get_bottom_edge())
                    stringOfStates += "6;";
                else if (cells[i][j]->get_left_edge() && !cells[i][j]->get_right_edge() && !cells[i][j]->get_bottom_edge())
                    stringOfStates += "7;";
                else if (cells[i][j]->get_left_edge() && cells[i][j]->get_right_edge() && !cells[i][j]->get_bottom_edge())
                    stringOfStates += "11;";
                else if (!cells[i][j]->get_left_edge() && cells[i][j]->get_right_edge() && cells[i][j]->get_bottom_edge())
                    stringOfStates += "12;";
                else if (cells[i][j]->get_left_edge() && !cells[i][j]->get_right_edge() && cells[i][j]->get_bottom_edge())
                    stringOfStates += "14;";
            } else if (cells[i][j]->get_right_edge()) {
                if (!cells[i][j]->get_left_edge() && !cells[i][j]->get_bottom_edge())
                    stringOfStates += "2;";
                else if (cells[i][j]->get_left_edge() && !cells[i][j]->get_bottom_edge())
                    stringOfStates += "8;";
                else if (!cells[i][j]->get_left_edge() && cells[i][j]->get_bottom_edge())
                    stringOfStates += "10;";
                else if (cells[i][j]->get_left_edge() && cells[i][j]->get_bottom_edge())
                    stringOfStates += "13;";
            }  else if (cells[i][j]->get_bottom_edge()) {
                if (!cells[i][j]->get_left_edge())
                    stringOfStates += "3;";
                else
                    stringOfStates += "9;";
            } else if (cells[i][j]->get_left_edge()) {
                 stringOfStates += "4;";
            } else
                 stringOfStates += "0;";
        }

        stringOfStates += "\n";
        // записываем полученную строку сосотояний в файл с помощью потока ввода-вывода
        stream << stringOfStates;
    }

    file.close();
}

void Maze::save() {
    QString fileName =  QFileDialog::getSaveFileName(0, "Сохранить лабиринт", "", "Текстовый формат (*.txt)");
    // если было введено имя для сохранения
    if (!fileName.isEmpty()) {
        // сохраняем в текстовом формате
        saveAsFile(fileName);
    }
}

void Maze::saveAs() {
    QString fileName =  QFileDialog::getSaveFileName(0, "Сохранить лабиринт", "", tr("Изображение (*.png);;Текстовый формат (*.txt)"));

    // если было введено имя для сохранения
    if (!fileName.isEmpty()) {
        // то определяем с каким расширением сохранить файл
        QString fileExtension = fileName.mid(fileName.lastIndexOf("."));
        if (fileExtension == ".png") {
            // сохраняем в виде изображения
            QPixmap pixmap(this->size());
            this->render(&pixmap);
            pixmap.save(fileName);
        } else if (fileExtension == ".txt") {
            // сохраняем в текстовом формате
            saveAsFile(fileName);
        }

    }
}

void Maze::open() {
    QString fileName = QFileDialog::getOpenFileName(0, "Открыть файл лабиринта", "", "*.txt");

    // если файл не был выбран - выходим из функции
    if (fileName.isEmpty()) {
        return;
    }

    QFile file(fileName);

    // если не удалось открыть файл для чтения, то показываем окошко с сообщением об ошибке.
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox msgBox;
        msgBox.setText("Не удалось открыть файл.");
        msgBox.exec();
        return;
    }

    // если же файл удалось открыть, то считываем из него информацию о лабиринте
    // для этого создаём пооток ввода-вывода
    QTextStream stream(&file); // поток ввода-вывода текста
    QString string; // строка считываемая из файла

    // очищаем ячейки перед их заполнением
    clearCells();
    // говорим что в новом лабиринте кратчайший путь не найден
    path_found = false;

    int stringNum = -1; // номер считанной строки, первую будем считать нулевой

    // пока не прочтём все строки из файла или пока число строк привысит ожидаемое число ячеек по вертикали - читаем его
    while (!stream.atEnd() && stringNum < cells_v) {
        // считываем строку
        string = stream.readLine();
        stringNum++;

        // парсим строку
        for (int i = 0; i < cells_h; i++)  { // cells_h = 20, в строке содержится 20 чисел определяющих состояние каждой ячейки
            // считываем первое число-состояние из строки
            int state = string.mid(0, string.indexOf(";") ).toInt();
            string = string.mid(string.indexOf(";") + 1); // отрезаем из строки считанное состояние

            // на основе считанного состояния заполняем ячейку
            switch (state) {
                case 0:
                    cells[stringNum][i]->set_top_edge(false);
                    cells[stringNum][i]->set_right_edge(false);
                    cells[stringNum][i]->set_bottom_edge(false);
                    cells[stringNum][i]->set_left_edge(false);
                    break;
                case 1:
                    cells[stringNum][i]->set_top_edge(true);
                    break;
                case 2:
                    cells[stringNum][i]->set_right_edge(true);
                    break;
                case 3:
                    cells[stringNum][i]->set_bottom_edge(true);
                    break;
                case 4:
                    cells[stringNum][i]->set_left_edge(true);
                    break;
                case 5:
                    cells[stringNum][i]->set_top_edge(true);
                    cells[stringNum][i]->set_right_edge(true);
                    break;
                case 6:
                    cells[stringNum][i]->set_top_edge(true);
                    cells[stringNum][i]->set_bottom_edge(true);
                    break;
                case 7:
                    cells[stringNum][i]->set_top_edge(true);
                    cells[stringNum][i]->set_left_edge(true);
                    break;
                case 8:
                    cells[stringNum][i]->set_left_edge(true);
                    cells[stringNum][i]->set_right_edge(true);
                    break;
                case 9:
                    cells[stringNum][i]->set_left_edge(true);
                    cells[stringNum][i]->set_bottom_edge(true);
                    break;
                case 10:
                    cells[stringNum][i]->set_right_edge(true);
                    cells[stringNum][i]->set_bottom_edge(true);
                    break;
                case 11:
                    cells[stringNum][i]->set_top_edge(true);
                    cells[stringNum][i]->set_left_edge(true);
                    cells[stringNum][i]->set_right_edge(true);
                    break;
                case 12:
                    cells[stringNum][i]->set_top_edge(true);
                    cells[stringNum][i]->set_right_edge(true);
                    cells[stringNum][i]->set_bottom_edge(true);
                    break;
                case 13:
                    cells[stringNum][i]->set_right_edge(true);
                    cells[stringNum][i]->set_bottom_edge(true);
                    cells[stringNum][i]->set_left_edge(true);
                    break;
                case 14:
                    cells[stringNum][i]->set_bottom_edge(true);
                    cells[stringNum][i]->set_left_edge(true);
                    cells[stringNum][i]->set_top_edge(true);
                    break;
                case 15:
                    cells[stringNum][i]->set_top_edge(true);
                    cells[stringNum][i]->set_right_edge(true);
                    cells[stringNum][i]->set_bottom_edge(true);
                    cells[stringNum][i]->set_left_edge(true);
                    break;
                default:
                    break;
            }
        }
    }

    file.close();

}
