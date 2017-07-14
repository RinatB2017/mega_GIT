//--------------------------------------------------------------------------------
#include "qled.hpp"
//--------------------------------------------------------------------------------
QLed::QLed(unsigned int width,
           unsigned int height,
           const QString &tooltip,
           const QColor &color,
           QWidget *parent) :
    QFrame(parent)
{
    ledColor = "";

    if(width < 16) width = 16;
    if(height < 16) height = 16;
    setMinimumSize(width, height);
    setToolTip(tooltip);
    setFrameShape(StyledPanel);
    setFrameShadow(Sunken);
    ledColor = color == Qt::white ? "background:white" : ledColor;
    ledColor = color == Qt::black ? "background:black" : ledColor;
    ledColor = color == Qt::red ? "background:red" : ledColor;
    ledColor = color == Qt::darkRed ? "background:darkRed" : ledColor;
    ledColor = color == Qt::green ? "background:green" : ledColor;
    ledColor = color == Qt::darkGreen ? "background:darkGreen" : ledColor;
    ledColor = color == Qt::blue ? "background:blue" : ledColor;
    ledColor = color == Qt::darkBlue ? "background:darkBlue" : ledColor;
    ledColor = color == Qt::cyan ? "background:cyan" : ledColor;
    ledColor = color == Qt::darkCyan ? "background:darkCyan" : ledColor;
    ledColor = color == Qt::magenta ? "background:magenta" : ledColor;
    ledColor = color == Qt::darkMagenta ? "background:darkMagenta" : ledColor;
    ledColor = color == Qt::yellow ? "background:yellow" : ledColor;
    ledColor = color == Qt::darkYellow ? "background:darkYellow" : ledColor;
    ledColor = color == Qt::gray ? "background:gray" : ledColor;
    ledColor = color == Qt::darkGray ? "background:darkGray" : ledColor;
    ledColor = color == Qt::lightGray ? "background:lightGray" : ledColor;
}
//--------------------------------------------------------------------------------
void QLed::setState(bool state)
{
    setStyleSheet(state ? ledColor : "");
}
//--------------------------------------------------------------------------------
