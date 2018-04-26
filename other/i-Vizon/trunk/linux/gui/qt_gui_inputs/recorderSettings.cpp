#include "recorderSettings.h"
#include "ui_recordersettings.h"
#include <iostream>


RecorderSettings::RecorderSettings(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RecorderSettings)
{
    ui->setupUi(this);

    viewQ = new MessageQ(9471);

    view.msgType = 1;
    view.flags.signBit = 0b0;
    view.flags.startNPause = DVR_TRUE;
    view.flags.stop = DVR_TRUE;
    view.flags.resolution = R600X800;
    view.flags.frameChannels = 0b010;
    view.flags.videoFileType = 0b010;
}

RecorderSettings::~RecorderSettings()
{
    delete ui;
}

void RecorderSettings::on_camIdTab1_activated(int index)
{
    std::cout<<"camera ID "<<index<<std::endl;
}

void RecorderSettings::on_setTab1_clicked()
{

}

void RecorderSettings::on_startTab1_clicked()
{
    std::cout<<"Send the data\n";
    viewQ->sendMsg(&view, sizeof(DvrSettings));
}

void RecorderSettings::on_stopTab1_clicked()
{

}

void RecorderSettings::on_colorChannelsTab1_activated(int index)
{

}

void RecorderSettings::on_setTab1_clicked(bool checked)
{

}
