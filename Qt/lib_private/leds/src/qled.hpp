//--------------------------------------------------------------------------------
#ifndef QLED_H
#define QLED_H
//--------------------------------------------------------------------------------
#include <QFrame>
//--------------------------------------------------------------------------------
enum {
    MODE_CIRCLE = 0,
    MODE_RECT
};
//--------------------------------------------------------------------------------
class QLed : public QWidget
{
    Q_OBJECT
public:
    explicit QLed(QWidget *parent = nullptr);
    void set_state(bool state);
    void set_size(int w, int h);
    void set_tooltip(const QString &text);
    void set_color(QColor color);
    void set_color_on(QColor color);
    void set_color_off(QColor color);

    bool get_state();

    void set_rect_mode();

private:
    QString tooltip = "led";
    QColor color_off = Qt::gray;
    QColor color_on = Qt::red;
    QColor current_color = Qt::gray;

    int mode = MODE_CIRCLE;

    bool led_state = false;

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif
