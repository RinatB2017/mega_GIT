//--------------------------------------------------------------------------------
#ifndef CELL_IO_H
#define CELL_IO_H
//--------------------------------------------------------------------------------
#include <QGridLayout>
#include <QPushButton>
//--------------------------------------------------------------------------------
#define MAX_X   9
#define MAX_Y   6
//--------------------------------------------------------------------------------
class Cell_IO : public QGridLayout
{
    Q_OBJECT
public:
    explicit Cell_IO(QWidget *parent = nullptr);
    void create_label(unsigned int x,
                      unsigned int y,
                      const QString &caption);
    void create_button(unsigned int x,
                       unsigned int y,
                       const QString &caption);

signals:
    void push(int x, int y, bool state);
    
public slots:

private slots:
    void click(bool state);
    
private:
    QWidget *parent;
    QPushButton *buttons[MAX_X][MAX_Y];
};
#endif // CELL_IO_H
//--------------------------------------------------------------------------------
