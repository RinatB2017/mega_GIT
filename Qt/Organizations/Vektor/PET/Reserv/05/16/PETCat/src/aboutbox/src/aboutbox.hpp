//--------------------------------------------------------------------------------
#ifndef ABOUTBOX_H
#define ABOUTBOX_H
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
                      const QString &author,
                      QWidget *parent = 0);
    ~AboutBox();

private slots:

private:
    Ui::AboutBox *ui = 0;
};
//--------------------------------------------------------------------------------
#endif // ABOUTBOX_H
