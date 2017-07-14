

#ifndef QSWITCH_H
#define QSWITCH_H

#include <Qt>
#include <QWidget>
#include <QtDesigner/QDesignerExportWidget>

class QDESIGNER_WIDGET_EXPORT QSwitch : public QWidget
{
 Q_OBJECT
	Q_ENUMS( SwitchType )
	Q_PROPERTY(bool value READ value WRITE setValue);
	Q_PROPERTY(SwitchType type READ type WRITE setType);

public: 
    enum SwitchType {Horizontal,Vertical};
    QSwitch(QWidget *parent = 0);
    bool value() const { return m_value; }
    SwitchType type() const { return m_type; }
public slots:
	void setValue(bool);
	void setType(SwitchType);
	void toggleValue();

signals:
/*!
  \brief This signal reports when the switch is toggled
*/
	void valueChanged(bool);
	
protected:
    bool m_value;
    SwitchType m_type;
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
    void keyReleaseEvent(QKeyEvent *);
};

#endif
