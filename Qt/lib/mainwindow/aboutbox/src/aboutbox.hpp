//--------------------------------------------------------------------------------
#ifndef ABOUTBOX_HPP
#define ABOUTBOX_HPP
//--------------------------------------------------------------------------------
#include <QDialog>
//--------------------------------------------------------------------------------
namespace Ui {
class AboutBox;
}
//--------------------------------------------------------------------------------
class AboutBox : public QDialog
{
    Q_OBJECT

public:
    AboutBox(const QString &orgName,
             const QString &programmName,
             const QString &version,
             const QString &author,
             QWidget *parent = 0);
    ~AboutBox();

private slots:
    void send_mail(QString link);

    void show_env(void);

private:
    Ui::AboutBox *ui;

protected:
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // ABOUTBOX_HPP
