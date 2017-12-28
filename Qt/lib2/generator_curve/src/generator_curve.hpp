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
#define MIN_VALUE   0
#define MAX_VALUE   1024
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
    
signals:
    void send(const QByteArray &data);
    
public slots:
    void close_gen(void);

private slots:
    void update(void);
    void send_4_bytes(void);
    void send_2_bytes(void);

    void update_values(void);

    void gen_sinus(void);
    void gen_triangle(void);
    void gen_saw(void);
    void gen_meandr(void);

    void start(bool state);

private:
    QWidget *parent = 0;
    QTimer *timer = 0;
    QSlider *sliders[MAX_SLIDER];
    int values[MAX_SLIDER];
    int index;
    QPushButton *btnPower = 0;
    QPushButton *btnSinus = 0;
    QPushButton *btnTriangle = 0;
    QPushButton *btnSaw = 0;
    QPushButton *btnMeandr = 0;

    QGroupBox *group = 0;
    QRadioButton *btn_4bytes = 0;
    QRadioButton *btn_2bytes = 0;

    QwtKnob *knob_Interval = 0;

    void init_timer(void);

    void load_setting(void);
    void save_setting(void);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
