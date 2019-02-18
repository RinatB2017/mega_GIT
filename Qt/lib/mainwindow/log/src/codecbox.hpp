//--------------------------------------------------------------------------------
#ifndef CODECBOX_H
#define CODECBOX_H
//--------------------------------------------------------------------------------
#include <QDialog>
//--------------------------------------------------------------------------------
namespace Ui {
    class CodecBox;
}
//--------------------------------------------------------------------------------
class QTextCodec;
//--------------------------------------------------------------------------------
class CodecBox : public QDialog
{
    Q_OBJECT
    
public:
    explicit CodecBox(QWidget *parent = nullptr);
    ~CodecBox();

private slots:
    void accept(void);
    
private:
    Ui::CodecBox *ui;
    QTextCodec *unixcodec;
    QTextCodec *wincodec;
    QList<QString> codecs;

    void findCodecs(void);
};
//--------------------------------------------------------------------------------
#endif // CODECBOX_H
