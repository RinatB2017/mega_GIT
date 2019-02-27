#include "www.h"

www::www(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	QStringList tmp;
	for (int i = 0; i < 100; i++)
	{
		tmp.append(QString::number(i));
	}
	ui.comboBox_test->addItems(tmp);
	ui.pushButton_test->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
}

www::~www()
{

}


// PushButton +1
void www::on_pushButton_test_clicked ()
{
	ui.verticalSlider_test->setValue(ui.verticalSlider_test->value()+1);
}



// ToolButton -1
void www::on_toolButton_test_clicked ()
{
	ui.verticalSlider_test->setValue(ui.verticalSlider_test->value()-1);
}

void www::on_verticalSlider_test_valueChanged ( int value )
{
	ui.lineEdit_test->setText(QString::number(value));
	ui.comboBox_test->setCurrentIndex(value);
}
