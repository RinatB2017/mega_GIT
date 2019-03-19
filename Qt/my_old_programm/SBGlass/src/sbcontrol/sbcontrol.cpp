#include "sbcontrol.h"

SBControl::SBControl(QWidget * inTestWidget, QWidget *parent)
    : QWidget(parent)
    , testWidget_(inTestWidget)
{
    ui.setupUi(this);
}

SBControl::~SBControl()
{

}

void SBControl::closeEvent(QCloseEvent* )
{
    qApp->quit();
}

// enableColor...
void SBControl::on_pushButton_control_clicked ()
{
    glass_.enableColor(QColorDialog::getColor());
}



// disableColor
void SBControl::on_pushButton_control_2_clicked ()
{
    glass_.disableColor();
}



// enableOpacity...
void SBControl::on_pushButton_control_3_clicked ()
{
    glass_.enableOpacity(QInputDialog::getDouble(this, "Set opacity", "Set opacity", 0,0,1));
}



// disableOpacity
void SBControl::on_pushButton_control_4_clicked ()
{
    glass_.disableOpacity();
}



// enableInfoBlock...
void SBControl::on_pushButton_control_5_clicked ()
{
    QString path = QFileDialog::getOpenFileName(this, "Change animation file", ".", "Animation file (*.gif)");
    QMovie * movie = new QMovie(path);
    if (!movie->isValid())
    {
        movie = nullptr;
    }
    glass_.enableInfoBlock(movie,QInputDialog::getText(this, "Enter text", "Enter text"));
}



// disableInfoBlock
void SBControl::on_pushButton_control_6_clicked ()
{
    glass_.disableInfoBlock();
}



// enableAnimationBlock...
void SBControl::on_pushButton_control_7_clicked ()
{
    QString path = QFileDialog::getOpenFileName(this, "", ".", "Animation file (*.gif)");
    QMovie * movie = new QMovie(path);
    if (movie->isValid())
        glass_.setMovie(movie);
    else
        QMessageBox::warning(this, "Error", QString("%1 not animation file(not valid QMovie).").arg(QDir::toNativeSeparators(path)));
}



// disableAnimationBlock
void SBControl::on_pushButton_control_8_clicked ()
{
    glass_.disableAnimationBlock();
}



// enableInfoTextBlock...
void SBControl::on_pushButton_control_9_clicked ()
{
    glass_.enableInfoTextBlock(QInputDialog::getText(this, "Enter text", "Enter text"));
}



// disableInfoTextBlock
void SBControl::on_pushButton_control_10_clicked ()
{
    glass_.disableInfoTextBlock();
}



// Install SBGlass
void SBControl::on_pushButton_control_11_clicked ()
{
    glass_.install(testWidget_);
    glass_.enableOpacity();
    glass_.enableColor();
    QMovie * movie = new QMovie(":/SBGlass/1627.gif");
    glass_.enableInfoBlock(movie, "Test Test");
}



// Remove SBGlass
void SBControl::on_pushButton_control_12_clicked ()
{
    glass_.remove();
}

