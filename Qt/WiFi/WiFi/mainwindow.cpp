#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    findTimer = new QTimer();
    findTimer->setInterval(1000);
    connect(findTimer,&QTimer::timeout,this,&MainWindow::findActiveWirelesses);
    findTimer->start();
    foundCount = 0;
    ui->treeWidgetWiFis->setColumnWidth(0,50);
    ui->treeWidgetWiFis->setColumnWidth(1,200);
    findActiveWirelesses();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::findActiveWirelesses()
{
    QNetworkConfigurationManager ncm;
    netcfgList = ncm.allConfigurations();
    WiFisList.clear();
    for (auto &x : netcfgList)
    {
        if (x.bearerType() == QNetworkConfiguration::BearerWLAN)
        {
            if(x.name() == "")
                WiFisList << "Unknown(Other Network)";
            else
                WiFisList << x.name();

            qDebug() << x.type();
        }
    }
    for(int i=0; i<WiFisList.size(); i++)
    {
        bool exist = false;
        QTreeWidgetItem * item = new QTreeWidgetItem();
        for(int j=0; j<ui->treeWidgetWiFis->topLevelItemCount(); j++)
        {
            QTreeWidgetItem *index = ui->treeWidgetWiFis->topLevelItem(j);
            QString str = index->text(1);
            if(str == WiFisList[i])
            {
                exist = true;
                break;
            }
        }
        if(!exist)
        {
            item->setTextAlignment(0,Qt::AlignVCenter);
            item->setTextAlignment(1,Qt::AlignHCenter);
            item->setText(0,QString::number(++foundCount));
            item->setText(1,WiFisList[i]);
            ui->treeWidgetWiFis->addTopLevelItem(item);
        }
    }
}
