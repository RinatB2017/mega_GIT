#ifndef LOGDIALOG_H
#define LOGDIALOG_H

#include <QDialog>

namespace Ui {
    class LogDialog;
}

class LogDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LogDialog(QWidget *parent = nullptr);
    ~LogDialog();
    QStringList userLogs;
    QStringList appLogs;
    
private:
    Ui::LogDialog *ui;
    
signals:
    void closed(bool);
    
public slots:
    virtual void closeEvent( QCloseEvent * );
    void refreshLog(QString section, QString newLog);
};

#endif // LOGDIALOG_H
