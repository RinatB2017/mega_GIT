#include "webget.h"
#include "ui_webget.h"

webget::webget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::webget)
{
    ui->setupUi(this);
}

webget::~webget()
{
    delete ui;
}
