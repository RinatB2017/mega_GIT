//--------------------------------------------------------------------------------
#ifndef GENERATOR_CURVE_HPP
#define GENERATOR_CURVE_HPP
//--------------------------------------------------------------------------------
#include <QScrollArea>
#include <QByteArray>
#include <QWidget>
#include <QtMath>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define MAX_SLIDER  32
//--------------------------------------------------------------------------------
class Generator_Curve : public MyWidget
{
    Q_OBJECT
public:
    explicit Generator_Curve(QWidget *parent = nullptr);
    virtual ~Generator_Curve(void);

    QByteArray get_data(void);

signals:
    void send(const QString &data);
    
public slots:
    void close_gen(void);

private slots:
    void gen_sinus(void);
    void gen_triangle(void);
    void gen_saw(void);
    void gen_meandr(void);

    void set_slider_tooltip(int value);

private:
    QPushButton *btnSinus = nullptr;
    QPushButton *btnTriangle = nullptr;
    QPushButton *btnSaw = nullptr;
    QPushButton *btnMeandr = nullptr;

    QList<QSlider *> sliders;

    QWidget *add_frame(void);
    QWidget *add_grapher(void);

    void init(void);

    void updateText(void);
    bool programm_is_exit(void);
    void load_setting(void);
    void save_setting(void);
};
//--------------------------------------------------------------------------------
#endif
