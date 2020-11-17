//--------------------------------------------------------------------------------
#ifndef QLED_H
#define QLED_H
//--------------------------------------------------------------------------------
#include <QFrame>
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

private:
    QString tooltip = "led";
    QColor color_off = Qt::gray;
    QColor color_on = Qt::red;
    QColor current_color = Qt::gray;

protected:
    void paintEvent(QPaintEvent *);
};
//--------------------------------------------------------------------------------
#endif
