#include "settings.h"
#include "ui_settings.h"
#include <QLabel>
#include <QPushButton>
Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    this->redef_keys = this->keypress = false;
    counter = 0xff;

}

Settings::~Settings()
{
    delete ui;
}

void Settings::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;


    default:
        break;
    }
}



void Settings::keyPressEvent(QKeyEvent *e)
{
    QLabel *lblmessage = this->findChild<QLabel *>("keymessage");

    if(this->redef_keys)
    {
        if(counter == 0xff)
            counter = 0;

        switch(counter)
        {
        case 0:
            new_input->a = e->key();



            break;
        case 1:
            new_input->b = e->key();

            break;
        case 2:
            new_input->start = e->key();

            break;

        case 3:
            new_input->select = e->key();

            break;
        case 4:
            new_input->up = e->key();

            break;
        case 5:
            new_input->down = e->key();

            break;
        case 6:
            new_input->right = e->key();

            break;
        case 7:
            new_input->left = e->key();

            break;

        }

    }


}

void Settings::keyReleaseEvent(QKeyEvent *e)
{

    QLabel *lblmessage = this->findChild<QLabel *>("keymessage");

    if(this->redef_keys)
    {


    switch(counter)
    {

        break;
    case 0:
        lblmessage->setText("Define 'B'");

        break;
    case 1:
              lblmessage->setText("Define 'Start'");

        break;
    case 2:
            lblmessage->setText("Define 'Select'");

        break;
    case 3:
            lblmessage->setText("Define 'Up'");

        break;
    case 4:
               lblmessage->setText("Define 'Down'");
        break;
    case 5:

        lblmessage->setText("Define 'Right'");
        break;
    case 6:
        lblmessage->setText("Define 'Left'");

        break;

    }
    if(counter>=7)
    {
        counter = 0xff;
        redef_keys = false;
        lblmessage->setText("");
        QPushButton *btn = this->findChild<QPushButton *>("redefine_keys_button");
        btn->setEnabled(true);
        this->ShowCurrentKeymap();
        emu->GetInputHandle()->SetControllerBinding1(this->new_input, true);
    }
    else counter++;

     }
}


void Settings::ShowCurrentKeymap()
{
    QLabel *lbl = this->findChild<QLabel *>("labelA");
    lbl->setText("A: "+QString::number(this->new_input->a, 16));
    lbl = this->findChild<QLabel *>("labelB");
    lbl->setText("B: " +QString::number(this->new_input->b, 16));
    lbl = this->findChild<QLabel *>("labelStart");
    lbl->setText("Start: "+QString::number(this->new_input->start, 16));
    lbl = this->findChild<QLabel *>("labelSelect");
    lbl->setText("Select: "+QString::number(this->new_input->select, 16));
    lbl = this->findChild<QLabel *>("labelUp");
    lbl->setText("Up: "+QString::number(this->new_input->up, 16));
    lbl = this->findChild<QLabel *>("labelDown");
    lbl->setText("Down: "+QString::number(this->new_input->down, 16));
    lbl = this->findChild<QLabel *>("labelLeft");
    lbl->setText("Left: "+QString::number(this->new_input->left, 16));
    lbl = this->findChild<QLabel *>("labelRight");
    lbl->setText("Right: "+QString::number(this->new_input->right, 16));
}

void Settings::on_redefine_keys_button_clicked()
{

    if(!redef_keys)
    {


    redef_keys = true;
    QLabel *lblmessage = this->findChild<QLabel *>("keymessage");
    lblmessage->setText("Define 'A'");
    QPushButton * btndi = this->findChild<QPushButton*>("redefine_keys_button");
    btndi->setEnabled(false);


    }
}
