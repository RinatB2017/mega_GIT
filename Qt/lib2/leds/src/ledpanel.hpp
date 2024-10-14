//--------------------------------------------------------------------------------
#ifndef LEDPANEL_H
#define LEDPANEL_H
//--------------------------------------------------------------------------------
#include <QWidget>
#include <QFrame>
//--------------------------------------------------------------------------------
#define MAXLEDS 32
//--------------------------------------------------------------------------------
class QLed;
class QHBoxLayout;
//--------------------------------------------------------------------------------
class LedPanel : public QFrame
{
    Q_OBJECT
public:
    explicit LedPanel(QWidget *parent = nullptr);
    explicit LedPanel(unsigned int max_leds,
                      unsigned int width_leds,
                      unsigned int height_leds,
                      QWidget *parent = nullptr);
    virtual ~LedPanel();

signals:
    void log(const QString &);

public slots:
    void drawValue(unsigned long data);
    int setLedToolTip(unsigned int position, const QString &string);
    void setEnabled(bool state);

private:
    QLed *led[MAXLEDS] = { nullptr };
    QHBoxLayout *hbox = nullptr;
    unsigned int count = 0;
};
//--------------------------------------------------------------------------------
#endif // LEDPANEL4_H
