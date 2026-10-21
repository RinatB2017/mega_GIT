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

    void set_u8_value(uint8_t value);
    uint8_t get_u8_value();

    void set_tool_tips(const QStringList &sl);
    void set_other_color(uint8_t mask_value,
                         QColor other_color);

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
