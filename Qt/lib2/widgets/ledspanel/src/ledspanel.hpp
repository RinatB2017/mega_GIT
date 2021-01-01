//--------------------------------------------------------------------------------
#ifndef LEDSPANEL_HPP
#define LEDSPANEL_HPP
//--------------------------------------------------------------------------------
#include <QPointer>
#include <QFrame>
//--------------------------------------------------------------------------------
class QFrame;
class QLabel;
class QGridLayout;
class QVBoxLayout;
//--------------------------------------------------------------------------------
class LedsPanel : public QFrame
{
    Q_OBJECT
    
public:
    explicit LedsPanel(QWidget *parent = nullptr);
    ~LedsPanel();

    void set_text(unsigned int, const QString &);

public slots:
    void draw_value(unsigned char);
    
private:
    QFrame *frame[8];
    QLabel *label[8];

    QPointer<QVBoxLayout> vbox;
    QPointer<QGridLayout> main_frame;

    void set_value(unsigned int, bool);
};
//--------------------------------------------------------------------------------
#endif // LEDSPANEL_HPP
