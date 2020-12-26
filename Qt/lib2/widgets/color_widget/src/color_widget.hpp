//--------------------------------------------------------------------------------
#ifndef COLOR_WIDGET_HPP
#define COLOR_WIDGET_HPP
//--------------------------------------------------------------------------------
#include <QWidget>
//--------------------------------------------------------------------------------
class Color_widget : public QWidget
{
    Q_OBJECT
public:
    explicit Color_widget(QWidget *parent = nullptr);

    void set_color(QColor color);

private:
    QColor current_color;

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif
