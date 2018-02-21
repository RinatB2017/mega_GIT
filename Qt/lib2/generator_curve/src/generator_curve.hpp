//--------------------------------------------------------------------------------
#ifndef GENERATOR_CURVE_HPP
#define GENERATOR_CURVE_HPP
//--------------------------------------------------------------------------------
#include <QByteArray>
#include <QWidget>
//--------------------------------------------------------------------------------
#include "mywidget.hpp"
//--------------------------------------------------------------------------------
class Settings;
class QTimer;
//--------------------------------------------------------------------------------
#define MAX_SLIDER  32
//--------------------------------------------------------------------------------
class QRadioButton;
class QPushButton;
class QGroupBox;
class QSlider;
class QwtKnob;
//--------------------------------------------------------------------------------
class Generator_Curve : public MyWidget
{
    Q_OBJECT
public:
    Generator_Curve(QWidget *parent = 0);
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

    void start(void);

    void update_sliders(void);

private:
    QPushButton *btnPower = 0;
    QPushButton *btnSinus = 0;
    QPushButton *btnTriangle = 0;
    QPushButton *btnSaw = 0;
    QPushButton *btnMeandr = 0;

    QList<QSlider *> sliders;

    QGroupBox *group = 0;
    QRadioButton *btn_1bytes = 0;
    QRadioButton *btn_2bytes = 0;

    void init_timer(void);

    QWidget *add_frame(void);
    QWidget *add_grapher(void);


    void load_setting(void);
    void save_setting(void);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
