#ifndef SBCONTROL_H
#define SBCONTROL_H

#include <QWidget>
#include <sbglass.h>
#include <QColorDialog>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

#include "ui_sbcontrol.h"

class SBControl : public QWidget
{
	Q_OBJECT

public:
	SBControl(QWidget * inTestWidget, QWidget *parent = nullptr);
	~SBControl();

private:
	Ui::SBControl ui;
	QWidget *testWidget_;
	SBGlass glass_;

	void closeEvent(QCloseEvent* );


private slots:
	void on_pushButton_control_12_clicked ();
	void on_pushButton_control_11_clicked ();
	void on_pushButton_control_10_clicked ();
	void on_pushButton_control_9_clicked ();
	void on_pushButton_control_8_clicked ();
	void on_pushButton_control_7_clicked ();
	void on_pushButton_control_6_clicked ();
	void on_pushButton_control_5_clicked ();
	void on_pushButton_control_4_clicked ();
	void on_pushButton_control_3_clicked ();
	void on_pushButton_control_2_clicked ();
	void on_pushButton_control_clicked ();
};

#endif // SBCONTROL_H
