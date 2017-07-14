
#ifndef _ANGLE_LINE_EDIT_H
#define _ANGLE_LINE_EDIT_H

#include <QLineEdit>
#include <QRegExp>
#include <QtDebug>
#include <QLineEdit>
#include <QRegExpValidator>

class AngleLineEdit : public QLineEdit
{
  Q_OBJECT

  public:
    AngleLineEdit(bool dbl = true, bool decimal = false, 
		  QWidget* parent = 0) : QLineEdit(parent)
    {    
    QChar dg(0x00b0);
    QRegExp rx;
    QString pattern, input_mask, text;
    if (dbl && !decimal)
      {
	pattern = QString("[-+]?\\b([0]?\\d?\\d|1[0-7]\\d|18[0-0])\\b\\%1\\s[0-5]\\d\\'").arg(dg);
	input_mask = QString("#000\\%1 00';0").arg(dg);
	text = QString("+000%1 00'").arg(dg);
      }
    if (dbl && decimal)
      {
	pattern = QString("[-+]?\\b([0]?\\d?\\d|1[0-7]\\d|18[0-0])\\b\\%1\\s[0-5]\\d\\,\\d\\'").arg(dg);
	input_mask = QString("#000\\%1 00,0';0").arg(dg);
	text = QString("+000%1 00,0'").arg(dg);
      }
    if (!dbl && !decimal)
      {
	pattern = QString("[+]?\\b([0-2]?\\d?\\d|3[0-5]\\d|36[0-0])\\b\\%1\\s[0-5]\\d\\'").arg(dg);
	input_mask = QString("#000\\%1 00';0").arg(dg);
	text = QString("+000%1 00'").arg(dg);
      }
    if (!dbl && decimal)
      {
	pattern = QString("[+]?\\b([0-2]?\\d?\\d|3[0-5]\\d|36[0-0])\\b\\%1\\s[0-5]\\d\\,\\d\\'").arg(dg);
	input_mask = QString("#000\\%1 00,0';0").arg(dg);
	text = QString("+000%1 00,0'").arg(dg);
      }
    rx.setPattern(pattern);
    QValidator* validator = new QRegExpValidator(rx, this);
    setInputMask(input_mask);
    setValidator(validator);
    setText(text);
    }

    static QString getAngleString(QString t, bool dbl = true, bool decimal = false)
    {
      AngleLineEdit* ale = new AngleLineEdit(dbl, decimal);
      //qDebug() << ">>>" << t;
      ale->setText(t);      
      QString str = ale->displayText();
      //      qDebug() << "<<<" << str;
      delete ale;
      return str;
    }
};

#endif // _ANGLE_LINE_EDIT_H
