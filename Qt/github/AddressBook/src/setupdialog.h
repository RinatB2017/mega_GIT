#pragma once
#include <QDialog>

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SetupDialog(QWidget *parent = 0);
	~SetupDialog();

	QString selectedFile() const;
	void setReasonText( const QString& reason );

private slots:
	void on_browseButton_clicked();

private:
	Ui::SetupDialog *ui;
};
