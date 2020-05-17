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
    explicit AboutBox(const QString &orgName,
                      const QString &programmName,
                      const QString &version,
                      const QString &email,
                      const QString &author,
                      const QString &telegram,
                      QWidget *parent = nullptr);
    ~AboutBox();

private slots:
    void send_mail(QString link);
    void go_telegramm(QString link);
    void show_env(void);

private:
    Ui::AboutBox *ui;

protected:
    void changeEvent(QEvent *event);
};
//--------------------------------------------------------------------------------
#endif // ABOUTBOX_HPP
