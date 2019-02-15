//--------------------------------------------------------------------------------
#ifndef GENERATOR_CURVE_HPP
#define GENERATOR_CURVE_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
#define MAX_SLIDER  32
//--------------------------------------------------------------------------------
class QRadioButton;
class QPushButton;
class QGroupBox;
class QSlider;
class QTimer;
//--------------------------------------------------------------------------------
class Generator_Curve : public MyWidget
{
    Q_OBJECT
public:
    Generator_Curve(QWidget *parent = nullptr);
    ~Generator_Curve(void);
    
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

    void start(void);

private:
    QPushButton *btnPower;
    QPushButton *btnSinus;
    QPushButton *btnTriangle;
    QPushButton *btnSaw;
    QPushButton *btnMeandr;

    QList<QSlider *> sliders;

    QWidget *add_frame(void);
    QWidget *add_grapher(void);

    void init(void);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
