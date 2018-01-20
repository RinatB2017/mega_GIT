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
    void update(void);

    void gen_sinus(void);
    void gen_triangle(void);
    void gen_saw(void);
    void gen_meandr(void);

    void start(bool state);

    void update_sliders(void);

private:
    QTimer *timer = 0;
    QPushButton *btnPower = 0;
    QPushButton *btnSinus = 0;
    QPushButton *btnTriangle = 0;
    QPushButton *btnSaw = 0;
    QPushButton *btnMeandr = 0;

    QList<QSlider *> sliders;

    QGroupBox *group = 0;
    QRadioButton *btn_1bytes = 0;
    QRadioButton *btn_2bytes = 0;

    QwtKnob *knob_Interval = 0;

    void init_timer(void);

    QWidget *add_frame(void);
    QWidget *add_grapher(void);

    void send_1_bytes(void);
    void send_2_bytes(void);

    void load_setting(void);
    void save_setting(void);

    void updateText(void);
};
//--------------------------------------------------------------------------------
#endif
