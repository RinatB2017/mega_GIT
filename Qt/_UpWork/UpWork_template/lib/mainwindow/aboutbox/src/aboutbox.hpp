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
                      const QString &author,
                      QWidget *parent = 0);
    ~AboutBox();

private:
    Ui::AboutBox *ui = 0;
};
//--------------------------------------------------------------------------------
#endif // ABOUTBOX_HPP
