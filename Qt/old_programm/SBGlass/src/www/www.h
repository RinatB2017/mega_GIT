#ifndef WWW_H
#define WWW_H

#include <QWidget>
#include <QKeySequence>
#include "ui_www.h"

class www : public QWidget
{
	Q_OBJECT

public:
	www(QWidget *parent = 0);
	~www();
	QWidget * testWidget();


private:
	Ui::www ui;

private slots:
	void on_toolButton_test_clicked ();

	void on_pushButton_test_clicked ();

	void on_verticalSlider_test_valueChanged ( int value );

};

#endif // WWW_H
