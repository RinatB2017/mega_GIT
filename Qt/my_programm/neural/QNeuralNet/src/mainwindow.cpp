/*
    Copyright (C) 2012  Martin Klapetek <martin.klapetek@gmail.com>

    Based on C++ implementation by Chesnokov Yuriy, Copyright (C) 2008

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "network.h"
#include "neuron.h"
#include "race-client.h"

#include <QGraphicsEllipseItem>
#include <QPointF>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QInputDialog>
#include <QTimer>

#include <QList>

#include <math.h>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_netForRecognition = false;
    qDebug() << "Ready";

    //connect UI stuff
    connect(ui->trainButton, SIGNAL(clicked()),
            this, SLOT(train()));

    connect(ui->openFileButton, SIGNAL(clicked()),
            this, SLOT(openFileDialog()));

    connect(ui->runButton, SIGNAL(clicked()),
            this, SLOT(run()));

    connect(ui->lrEdit, SIGNAL(editingFinished()),
            this, SLOT(netParamsChanged()));

    connect(ui->momentumEdit, SIGNAL(editingFinished()),
            this, SLOT(netParamsChanged()));

    connect(ui->maxErrorEdit, SIGNAL(editingFinished()),
            this, SLOT(netParamsChanged()));

    connect(ui->connectButton, SIGNAL(clicked()),
            this, SLOT(connectToServer()));

    connect(ui->resetButton, SIGNAL(clicked()),
            this, SLOT(reset()));

    connect(ui->display, SIGNAL(cellClicked(int,int)),
            this, SLOT(toggleDisplaySegment(int,int)));

    connect(ui->recognizeButton, SIGNAL(clicked(bool)),
            this, SLOT(recognizeNumber()));

    connect(ui->oneButton, SIGNAL(clicked(bool)),
            this, SLOT(paintNumber()));

    connect(ui->twoButton, SIGNAL(clicked(bool)),
            this, SLOT(paintNumber()));

    connect(ui->threeButton, SIGNAL(clicked(bool)),
            this, SLOT(paintNumber()));

    connect(ui->fourButton, SIGNAL(clicked(bool)),
            this, SLOT(paintNumber()));

    connect(ui->fiveButton, SIGNAL(clicked(bool)),
            this, SLOT(paintNumber()));

    connect(ui->sixButton, SIGNAL(clicked(bool)),
            this, SLOT(paintNumber()));

    connect(ui->sevenButton, SIGNAL(clicked(bool)),
            this, SLOT(paintNumber()));

    connect(ui->eightButton, SIGNAL(clicked(bool)),
            this, SLOT(paintNumber()));

    connect(ui->nineButton, SIGNAL(clicked(bool)),
            this, SLOT(paintNumber()));

    connect(ui->zeroButton, SIGNAL(clicked(bool)),
            this, SLOT(paintNumber()));

    ui->runButton->setDisabled(true);
    ui->trainButton->setDisabled(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::train()
{
    int i = 0;
    QList<float> *netOutputs;
    bool trained = false;
    while (!trained && i < ui->iterationsEdit->text().toInt())
    {
        qDebug() << "Epoch" << i;
        trained = m_network->trainSet(m_normTrainingSet, m_expectedOutputs);
        i++;
        ui->iterationsCountEdit->setText(QString::number(i));

        if (i % 2 == 0)
        {
            for (int k = 0; k < m_normTrainingSet.size(); k++)
            {
                netOutputs = m_network->run(m_normTrainingSet.at(k));
                QString nout;
                for (int j = 0; j < netOutputs->size(); j++)
                {
                    nout.append(QString::number(netOutputs->at(j)));
                    if (j != netOutputs->size() - 1)
                    {
                        nout.append("; ");
                    }
                }
                ui->trainTableWidget->setItem(k, 2, new QTableWidgetItem(nout));
                QTimer::singleShot(0, ui->trainTableWidget, SLOT(resizeColumnsToContents()));
            }
        }
    }
    qDebug() << "Trained after" << i;
    ui->trainFirstLabel->hide();
    ui->runButton->setEnabled(true);
}

void MainWindow::reset()
{
    m_iteration = 0;
    m_trainingSet.clear();
    m_normTrainingSet.clear();
    m_expectedOutputs.clear();
    m_runSet.clear();
    m_normRunSet.clear();
    m_inputsList.clear();
    if (m_network)
    {
        delete m_network;
        m_network = nullptr;
    }
    m_neuronsInLayersList.clear();
    m_outputsList.clear();

    ui->iterationsCountEdit->setText(QString("0"));
    ui->trainFirstLabel->show();
    ui->runButton->setDisabled(true);
    ui->trainButton->setDisabled(true);
    ui->trainTableWidget->clearContents();
    ui->runSetTableWidget->clearContents();
    ui->trainTableWidget->setRowCount(0);
    ui->runSetTableWidget->setRowCount(0);
}

void MainWindow::openFileDialog()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), QString(), tr("Text files (*.txt)"));

    loadFile(fileName);
}

void MainWindow::loadFile(QString filename)
{
    reset();

    QString lineString;
    bool layers = false;
    bool inputs = false;
    bool neuronsInLayers = false;
    bool outputs = false;
    bool lr = false;
    bool momentum = false;
    bool trainingSet = false;
    bool runSet = false;

    bool trainCountRead = false;
    bool runCountRead = false;

    int inputsRead = 0;
    int trainRead = 0;
    int trainToRead = 999;
    int runRead = 0;
    int runToRead = 999;

    int layersCount = 0;

    int inputNumber = 0;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();

        qDebug() << "Read line:" << line;

        if(line.contains("#") || line.isEmpty() || line == "\n")
            continue;

        if(layers == false)
        {
            QString sline(line.trimmed());
            layersCount = sline.toInt();

            //add input layer
            layersCount++;

            qDebug() << "Layers Count:" << layersCount;
            ui->layerCountEdit->setText(QString::number(layersCount));
            layers = true;
            continue;
        }

        if(layers && inputs == false)
        {
            qDebug() << "* Inputs while";
            QString sline(line.trimmed());
            inputNumber = sline.toInt();

            qDebug() << "Inputs to read:" << inputNumber;

            while(inputsRead < inputNumber)
            {
                line = file.readLine();
                if(line.contains("#") || line.isEmpty() || line == "\n")
                    continue;

                qDebug() << "Appending line with inputs";

                m_inputsList.append(QString(line.simplified()).split(" "));
                inputsRead++;
            }

            inputs = true;
            continue;
        }

        if(layers && inputs && neuronsInLayers == false)
        {
            qDebug() << "* Neurons in layers while";

            //             line = file.readLine();

            if(line.contains("#") || line.isEmpty() || line == "\n")
                continue;

            qDebug() << "Read in Neurons in layers while:" << line;

            foreach(const QString &n, QString(line.simplified()).split(" "))
            {
                m_neuronsInLayersList.append(n.toInt());
            }
            //ui->layerCountEdit->setText(QString::number(m_neuronsInLayersList.size()));
            neuronsInLayers = true;
            //             continue;
        }

        if(layers && inputs && neuronsInLayers && outputs == false)
        {
            qDebug() << "* Outputs while";

            int outputsRead = 0;
            int outputNumber = m_neuronsInLayersList.last();

            while(outputsRead < outputNumber)
            {
                line = file.readLine();
                if(line.contains("#") || line.isEmpty() || line == "\n")
                    continue;

                qDebug() << "Outputs read:" << line;
                m_outputsList.append(QString(line.simplified()));
                outputsRead++;
            }

            outputs = true;
            //             continue;
        }

        if(layers && inputs && neuronsInLayers && outputs && lr == false)
        {
            qDebug() << "* LR while";

            while(lr == false)
            {
                line = file.readLine();

                qDebug() << "Read in LR while:" << line;

                if(line.contains("#") || line.isEmpty() || line == "\n")
                    continue;

                ui->lrEdit->setText(QString(line.simplified()));
                lr = true;
            }
        }

        if(layers && inputs && neuronsInLayers && outputs && lr && momentum == false)
        {
            qDebug() << "* Momentum while";

            while(momentum == false)
            {
                line = file.readLine();

                qDebug() << "Read in momentum while:" << line;

                if(line.contains("#") || line.isEmpty() || line == "\n")
                    continue;

                ui->momentumEdit->setText(QString(line.simplified()));
                momentum = true;
            }
        }

        if(layers && inputs && neuronsInLayers && outputs && lr && momentum && trainingSet == false)
        {
            qDebug() << "* TS while";
            //             QString sline(line.trimmed());
            //             trainToRead = sline.toInt();

            while(trainRead < trainToRead)
            {
                line = file.readLine();
                line = line.trimmed();

                qDebug() << "Read in TS while:" << line;

                if(line.contains("#") || line.isEmpty() || line == "\n")
                    continue;

                if(trainCountRead == false)
                {
                    trainToRead = line.toInt();
                    qDebug() << "Train data to read:" << trainToRead;
                    trainCountRead = true;
                    continue;
                }

                QString sline(line.simplified());
                QStringList numbers = sline.split(" ");
                qDebug() << numbers.size() << numbers;

                QList<float> l;

                for(int j = 0; j < numbers.size() - m_outputsList.size(); j++)
                {
                    l.append(numbers.at(j).toFloat());
                }

                m_trainingSet.append(l);


                QList<float> o;

                for(int j = numbers.size() - m_outputsList.size(); j < numbers.size(); j++)
                {
                    o.append(numbers.at(j).toFloat());
                }

                m_expectedOutputs.append(o);

                trainRead++;
            }

            trainingSet = true;
        }

        if(layers && inputs && neuronsInLayers && outputs && lr && momentum && trainingSet && runSet == false)
        {
            while(runRead < runToRead)
            {
                line = file.readLine();
                line = line.trimmed();

                qDebug() << "Read in RS while:" << line;

                if(line.contains("#") || line.isEmpty() || line == "\n")
                    continue;

                if(runCountRead == false)
                {
                    runToRead = line.toInt();
                    qDebug() << "Run data to read:" << runToRead;
                    runCountRead = true;
                    continue;
                }

                QString sline(line);
                QStringList numbers = sline.split(" ");

                QList<float> l;

                for(int j = 0; j < numbers.size(); j++)
                {
                    l.append(numbers.at(j).toFloat());
                }

                m_runSet.append(l);


                runRead++;
            }

            runSet = true;
        }

    }

    m_neuronsInLayersList.prepend(m_inputsList.size());

    //normalize training set
    float x;
    QList<float> n;

    for (int i = 0; i < m_trainingSet.size(); i++)
    {
        n.clear();
        for (int j = 0; j < m_inputsList.size(); j++)
        {
            x = (m_trainingSet.at(i).at(j) - m_inputsList.at(j).at(1).toFloat()) / (m_inputsList.at(j).at(2).toFloat() - m_inputsList.at(j).at(1).toFloat());
            n.append(x);
        }
        m_normTrainingSet.append(n);
    }

    //normalize run set
    QList<float> r;

    for (int i = 0; i < m_runSet.size(); i++)
    {
        r.clear();
        for (int j = 0; j < m_inputsList.size(); j++)
        {
            x = (m_runSet.at(i).at(j) - m_inputsList.at(j).at(1).toFloat()) / (m_inputsList.at(j).at(2).toFloat() - m_inputsList.at(j).at(1).toFloat());
            r.append(x);
        }
        m_normRunSet.append(r);
    }

    qDebug() << "##### Summary #####";
    qDebug() << "Inputs list:" << m_inputsList;
    qDebug() << "Layers:" << layersCount;
    qDebug() << "Neurons in layer:" << m_neuronsInLayersList;
    qDebug() << "Outputs list:" << m_outputsList;
    qDebug() << "Training set:" << m_trainingSet;
    qDebug() << "Norm. training set:" << m_normTrainingSet;
    qDebug() << "Expected outputs:" << m_expectedOutputs;
    qDebug() << "Run set:" << m_runSet;
    qDebug() << "Norm. run set:" << m_normRunSet;
    qDebug() << "Learning:" << ui->lrEdit->text();
    qDebug() << "Momentum:" << ui->momentumEdit->text();

    m_network = new Network(m_neuronsInLayersList,
                            ui->momentumEdit->text().toFloat(),
                            ui->lrEdit->text().toFloat(),
                            ui->maxErrorEdit->text().toFloat());
    m_netForRecognition = false;

    for (int i = 0; i < m_trainingSet.size(); i++)
    {
        QString tset, eout;
        for (int j = 0; j < m_trainingSet.at(i).size(); j++)
        {
            tset.append(QString::number(m_trainingSet.at(i).at(j)));
            if (j != m_trainingSet.at(i).size() - 1)
            {
                tset.append("; ");
            }
        }
        for (int j = 0; j < m_expectedOutputs.at(i).size(); j++)
        {
            eout.append(QString::number(m_expectedOutputs.at(i).at(j)));
            if (j != m_expectedOutputs.at(i).size() - 1)
            {
                eout.append("; ");
            }
        }
        ui->trainTableWidget->setRowCount(ui->trainTableWidget->rowCount() + 1);
        ui->trainTableWidget->setItem(i, 0, new QTableWidgetItem(tset));
        ui->trainTableWidget->setItem(i, 1, new QTableWidgetItem(eout));
    }

    for (int i = 0; i < m_runSet.size(); i++)
    {
        QString rset;
        for (int j = 0; j < m_runSet.at(i).size(); j++)
        {
            rset.append(QString::number(m_runSet.at(i).at(j)));
            if (j != m_runSet.at(i).size() - 1)
            {
                rset.append("; ");
            }
        }

        ui->runSetTableWidget->setRowCount(ui->runSetTableWidget->rowCount() + 1);
        ui->runSetTableWidget->setItem(i, 0, new QTableWidgetItem(rset));
    }

    ui->trainButton->setEnabled(true);
    ui->tabWidget->setCurrentIndex(0);
}

void MainWindow::run()
{
    QList<float> *result;
    for (int i = 0; i < m_normRunSet.size(); i++)
    {
        result = m_network->run(m_normRunSet.at(i));
        qDebug() << "Net output for" << m_normRunSet.at(i) << ":" << *result;
        QString rout;
        for (int j = 0; j < result->size(); j++)
        {
            rout.append(QString::number(result->at(j)));
            if (j != result->size() - 1)
            {
                rout.append("; ");
            }
        }

        ui->runSetTableWidget->setItem(i, 1, new QTableWidgetItem(rout));
    }

    //     QMessageBox::information(this, "Running set completed", perceptronOutput, QMessageBox::Ok);
}

void MainWindow::netParamsChanged()
{
    if (m_network)
    {
        m_network->setLearningRate(ui->lrEdit->text().toFloat());
        m_network->setMomentum(ui->momentumEdit->text().toFloat());
        m_network->setMaxError(ui->maxErrorEdit->text().toFloat());
    }
}

void MainWindow::connectToServer()
{
    bool ok;
    QString text = QInputDialog::getText(this,
                                         tr("Enter server:port"),
                                         tr("Connect to:"),
                                         QLineEdit::Normal,
                                         QString("127.0.0.1:9477"), &ok);

    if (ok)
    {
        QStringList s = text.split(":");
        m_raceClient = new RaceClient(s.at(0), s.at(1).toInt(), this);
        connect(m_raceClient, SIGNAL(stateChanged(RaceClient::ClientState)),
                this, SLOT(handleRaceStateChange(RaceClient::ClientState)));
        //         ui->tabWidget->setCurrentIndex(2);
    }
}

void MainWindow::handleRaceStateChange(RaceClient::ClientState state)
{
    //     qDebug() << "Got state change" << state;
    switch (state)
    {
    case RaceClient::Connecting:
        ui->raceStateLabel->setText("Connecting...");
        break;
    case RaceClient::ConnectingToRace:
        ui->raceStateLabel->setText("Connecting to race...");
        ui->serverHostLabel->setText(m_raceClient->host());
        break;
    case RaceClient::GettingRaceList:
        ui->raceStateLabel->setText("Getting race list...");
        ui->serverHostLabel->setText(m_raceClient->host());
        break;
    case RaceClient::Racing:
        ui->raceStateLabel->setText("Race in progress");
        break;
    case RaceClient::Disconnected:
        ui->raceStateLabel->setText("Disconnected");
        ui->serverHostLabel->setText("Not connected");
        break;
    case RaceClient::Error:
        ui->raceStateLabel->setText("Error");
        ui->serverHostLabel->setText("Not connected");
        break;
    }
}

void MainWindow::toggleDisplaySegment(int row, int column)
{
    if (ui->display->item(row, column)->backgroundColor() == Qt::black)
    {
        ui->display->item(row, column)->setBackground(Qt::white);
    }
    else
    {
        ui->display->item(row, column)->setBackground(Qt::black);
    }
}

void MainWindow::recognizeNumber()
{
    if (!m_netForRecognition)
    {
        if (m_network)
        {
            delete m_network;
            m_network = nullptr;
        }

        //init network with 4 layers, alpha=0.8, learningRate=0.1 and maxError=0.05
        m_network = new Network(QList<int>() << 25 << 18 << 8 << 4,
                                0.8f,
                                0.1f,
                                0.05f);
        m_netForRecognition = true;
    }

    bool train = false;
    if (train)
    {
        QList<QList<float> > trainSet;
        //prepare the train set
        trainSet << (QList<float>() << 0 << 0 << 0 << 1 << 0
                     << 0 << 0 << 1 << 1 << 0
                     << 0 << 1 << 0 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0)

                 << (QList<float>() << 0 << 0 << 1 << 1 << 0
                     << 0 << 1 << 0 << 0 << 1
                     << 0 << 0 << 0 << 1 << 0
                     << 0 << 0 << 1 << 0 << 0
                     << 0 << 1 << 1 << 1 << 1)

                 << (QList<float>() << 0 << 1 << 1 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0
                     << 0 << 1 << 1 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0
                     << 0 << 1 << 1 << 1 << 0)

                 << (QList<float>() << 0 << 1 << 0 << 1 << 0
                     << 0 << 1 << 0 << 1 << 0
                     << 0 << 1 << 1 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0)

                 << (QList<float>() << 0 << 1 << 1 << 1 << 0
                     << 0 << 1 << 0 << 0 << 0
                     << 0 << 1 << 1 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0
                     << 0 << 1 << 1 << 1 << 0)

                 << (QList<float>() << 0 << 1 << 1 << 1 << 0
                     << 0 << 1 << 0 << 0 << 0
                     << 0 << 1 << 1 << 1 << 0
                     << 0 << 1 << 0 << 1 << 0
                     << 0 << 1 << 1 << 1 << 0)

                 << (QList<float>() << 0 << 1 << 1 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0)

                 << (QList<float>() << 0 << 1 << 1 << 1 << 0
                     << 0 << 1 << 0 << 1 << 0
                     << 0 << 1 << 1 << 1 << 0
                     << 0 << 1 << 0 << 1 << 0
                     << 0 << 1 << 1 << 1 << 0)

                 << (QList<float>() << 0 << 1 << 1 << 1 << 0
                     << 0 << 1 << 0 << 1 << 0
                     << 0 << 1 << 1 << 1 << 0
                     << 0 << 0 << 0 << 1 << 0
                     << 0 << 1 << 1 << 1 << 0)

                 << (QList<float>() << 0 << 1 << 1 << 1 << 0
                     << 0 << 1 << 0 << 1 << 0
                     << 0 << 1 << 0 << 1 << 0
                     << 0 << 1 << 0 << 1 << 0
                     << 0 << 1 << 1 << 1 << 0);

        QList<QList<float> > outputs;
        //expected outputs for the trainset
        outputs << (QList<float>() << 0 << 0 << 0 << 1 )
                << (QList<float>() << 0 << 0 << 1 << 0 )
                << (QList<float>() << 0 << 0 << 1 << 1 )
                << (QList<float>() << 0 << 1 << 0 << 0 )
                << (QList<float>() << 0 << 1 << 0 << 1 )
                << (QList<float>() << 0 << 1 << 1 << 0 )
                << (QList<float>() << 0 << 1 << 1 << 1 )
                << (QList<float>() << 1 << 0 << 0 << 0 )
                << (QList<float>() << 1 << 0 << 0 << 1 )
                << (QList<float>() << 0 << 0 << 0 << 0);

        bool trained = false;
        int i = 0;

        while (!trained && i < 1000000)
        {
            qDebug() << "Epoch" << i;
            trained = m_network->trainSet(trainSet, outputs);
            i++;
        }
        qDebug() << "Trained after" << i;

        m_network->printWeights();

    }
    else if (!m_network->isTrained())
    {
        QList<QList<QList<float> > > weights;
        //these are already learnt weights,
        //we can plug them in directly
        weights << (QList<QList<float> >() //Layer 1
                    << (QList<float>() //======= Neuron 0
                        << -0.638249f
                        << 5.1979e+34f
                        << -1.20019f
                        << -1.6819f
                        << -1.09797f
                        << 0.924777f
                        << 0.657582f
                        << 1.43451f
                        << 0.858619f
                        << -1.06778f
                        << 5.1979e+34f
                        << 0.690148f
                        << 1.82082f
                        << 8.44057f
                        << -0.887892f
                        << 0.268565f
                        << 8.39364f
                        << 0.963709f
                        << 7.91213f
                        << -1.00071f
                        << 0.0830466f
                        << 0.295485f
                        << 8.28963f
                        << 1.14708f
                        << -0.124517f
                        << 0.304544f
                        )
                    << (QList<float>() //======= Neuron 1
                        << 0.29967f
                        << 0.870814f
                        << 0.765024f
                        << 0.679504f
                        << 0.069199f
                        << 0.240901f
                        << 0.110031f
                        << 0.726196f
                        << 0.539977f
                        << 0.126177f
                        << 0.650974f
                        << 0.197559f
                        << 0.0849223f
                        << 0.765929f
                        << 5.1979e+34f
                        << 0.879934f
                        << 0.456076f
                        << 0.846467f
                        << 0.699583f
                        << 0.68227f
                        << 0.115032f
                        << 0.593228f
                        << 0.293917f
                        << 0.527159f
                        << 0.706633f
                        << 0.376964f
                        )
                    << (QList<float>() //======= Neuron 2
                        << 0.459642f
                        << 0.139617f
                        << 0.141882f
                        << 0.944169f
                        << 0.0812357f
                        << 7.97222f
                        << 0.683023f
                        << 0.0548349f
                        << -0.180621f
                        << 0.29584f
                        << 0.450087f
                        << 0.261754f
                        << 0.0199526f
                        << 0.792234f
                        << 0.0250054f
                        << 0.129392f
                        << 0.187624f
                        << 0.551229f
                        << 8.39532f
                        << 0.0685427f
                        << 0.352788f
                        << 0.351397f
                        << 0.314208f
                        << 0.0884204f
                        << -0.329294f
                        << 0.392975f
                        )
                    << (QList<float>() //======= Neuron 3
                        << 0.627829f
                        << 0.327585f
                        << 0.952842f
                        << 0.402285f
                        << 0.686778f
                        << 0.742779f
                        << 0.49185f
                        << 0.864678f
                        << 0.500939f
                        << 0.903072f
                        << 0.352692f
                        << 0.238011f
                        << 0.0900838f
                        << 0.502744f
                        << 0.972462f
                        << 0.595284f
                        << 0.762793f
                        << 0.46233f
                        << 0.588605f
                        << 0.129226f
                        << 0.598043f
                        << 0.772971f
                        << 0.640006f
                        << 0.5098f
                        << 0.186489f
                        << 0.979626f
                        )
                    << (QList<float>() //======= Neuron 4
                        << 0.814555f
                        << 0.482391f
                        << 0.0466217f
                        << 0.918806f
                        << 0.845161f
                        << 0.67434f
                        << 0.206014f
                        << 0.776563f
                        << -0.0222101f
                        << 0.872025f
                        << 0.53864f
                        << 0.518424f
                        << 0.722334f
                        << 0.0767335f
                        << 0.424393f
                        << 0.136975f
                        << 0.331279f
                        << 0.600645f
                        << 0.638374f
                        << 0.290789f
                        << 0.194916f
                        << 0.400807f
                        << 0.795931f
                        << 0.786033f
                        << 0.521326f
                        << 0.388687f
                        )
                    << (QList<float>() //======= Neuron 5
                        << 0.413598f
                        << 0.175111f
                        << 0.811081f
                        << 0.654055f
                        << 0.0115422f
                        << 0.726331f
                        << 0.240075f
                        << 0.147409f
                        << 0.570754f
                        << 0.0102338f
                        << 0.818822f
                        << 0.810774f
                        << 0.868903f
                        << 0.870152f
                        << 0.58166f
                        << 0.449223f
                        << 0.399556f
                        << 0.512367f
                        << 0.506754f
                        << 0.750066f
                        << 0.645971f
                        << 0.87377f
                        << 0.412466f
                        << 0.242805f
                        << 0.0553436f
                        << 0.612802f
                        )
                    << (QList<float>() //======= Neuron 6
                        << 0.569417f
                        << 0.985444f
                        << 0.356942f
                        << 0.194374f
                        << 0.258291f
                        << 0.982039f
                        << 0.411436f
                        << 0.182654f
                        << 0.701165f
                        << 0.464978f
                        << 0.976098f
                        << 0.979798f
                        << 0.642279f
                        << 0.540673f
                        << -0.0202861f
                        << 0.597694f
                        << 0.397084f
                        << -0.030039f
                        << 0.473526f
                        << 0.00808778f
                        << 0.455406f
                        << 0.878382f
                        << 0.595643f
                        << 0.966126f
                        << 0.616998f
                        << 0.268087f
                        )
                    << (QList<float>() //======= Neuron 7
                        << 0.753884f
                        << 0.185996f
                        << 0.645607f
                        << -0.00577245f
                        << 0.716753f
                        << 0.242165f
                        << 0.0523934f
                        << 0.058172f
                        << 0.452261f
                        << 0.445495f
                        << 0.0655982f
                        << 0.889925f
                        << 0.638352f
                        << 1.01554f
                        << 0.334988f
                        << 0.662814f
                        << 0.964899f
                        << 0.129958f
                        << 0.0693398f
                        << 0.121968f
                        << 0.793669f
                        << 0.646209f
                        << -0.110862f
                        << 0.0952327f
                        << 0.646846f
                        << 0.341836f
                        )
                    << (QList<float>() //======= Neuron 8
                        << 0.100053f
                        << 0.392045f
                        << 0.495312f
                        << 0.879876f
                        << 0.61502f
                        << 0.391291f
                        << 0.0692459f
                        << 0.213325f
                        << 0.42099f
                        << 0.839641f
                        << 0.476256f
                        << 0.510634f
                        << 0.115516f
                        << 0.932432f
                        << 0.886265f
                        << 0.4125f
                        << 0.833385f
                        << 0.606765f
                        << 0.408887f
                        << 0.224176f
                        << 0.280755f
                        << 0.362501f
                        << 0.489256f
                        << 0.536698f
                        << 0.371874f
                        << 0.287155f
                        )
                    << (QList<float>() //======= Neuron 9
                        << 0.170275f
                        << 0.488962f
                        << 0.613596f
                        << 1.05904f
                        << 0.00491128f
                        << 0.699241f
                        << 0.332762f
                        << 0.48596f
                        << 0.518941f
                        << 0.951626f
                        << 0.914193f
                        << 0.651736f
                        << 0.147743f
                        << 0.378435f
                        << 0.550325f
                        << 0.68597f
                        << 0.890478f
                        << 0.704469f
                        << 0.622018f
                        << 0.828893f
                        << 0.135139f
                        << 0.462814f
                        << 0.48916f
                        << 0.576806f
                        << 0.743847f
                        << 0.71844f
                        )
                    << (QList<float>() //======= Neuron 10
                        << -0.200217f
                        << 0.23124f
                        << -3.28007f
                        << 0.35858f
                        << -0.588327f
                        << 0.431818f
                        << 0.806845f
                        << -3.66991f
                        << 1.17144f
                        << 8.70239f
                        << 2.39643f
                        << 0.705297f
                        << -0.477115f
                        << 1.47038f
                        << -0.391965f
                        << 0.269451f
                        << 0.98894f
                        << 1.9322f
                        << 2.29101f
                        << -2.19258f
                        << 0.61046f
                        << 0.539773f
                        << 2.22004f
                        << 2.19271f
                        << -0.72845f
                        << 2.04387f
                        )
                    << (QList<float>() //======= Neuron 11
                        << 0.676969f
                        << 0.812025f
                        << 0.988102f
                        << 0.522151f
                        << 0.512143f
                        << 0.83022f
                        << 0.682983f
                        << 0.748511f
                        << 0.073214f
                        << 0.155559f
                        << 0.221455f
                        << 0.954948f
                        << 0.156676f
                        << 0.597879f
                        << 0.746748f
                        << 0.000279446f
                        << 0.303257f
                        << 0.0925613f
                        << 0.333456f
                        << -0.0153699f
                        << 0.389778f
                        << 0.326424f
                        << 0.934926f
                        << 0.0427942f
                        << 0.845148f
                        << 0.537651f
                        )
                    << (QList<float>() //======= Neuron 12
                        << 0.501227f
                        << 0.137979f
                        << 0.783623f
                        << 0.909891f
                        << 0.462751f
                        << 0.48429f
                        << 0.757801f
                        << 0.487276f
                        << 0.888869f
                        << 0.2144f
                        << 0.273183f
                        << 0.619016f
                        << 0.0548286f
                        << 0.467659f
                        << 0.731538f
                        << 0.31471f
                        << 0.408466f
                        << 0.0510212f
                        << 0.880433f
                        << 0.135566f
                        << 0.0549525f
                        << 0.215928f
                        << 0.276269f
                        << 0.36743f
                        << 0.145076f
                        << 0.658816f
                        )
                    << (QList<float>() //======= Neuron 13
                        << 0.0116832f
                        << 0.153915f
                        << 0.604098f
                        << 0.845789f
                        << -0.0115643f
                        << 0.308974f
                        << 0.727722f
                        << 0.20564f
                        << -0.225678f
                        << -0.471452f
                        << 0.860859f
                        << 0.0125502f
                        << -0.00463327f
                        << 0.118323f
                        << -0.39898f
                        << 0.0485463f
                        << 0.516539f
                        << 0.700111f
                        << 0.401385f
                        << -0.281075f
                        << 0.712222f
                        << 0.91053f
                        << 0.495492f
                        << 0.74f
                        << -0.615617f
                        << 0.334743f
                        )
                    << (QList<float>() //======= Neuron 14
                        << 0.474491f
                        << 0.392752f
                        << 1.49001f
                        << -1.50425f
                        << -0.282762f
                        << 0.546569f
                        << 0.217412f
                        << 3.8008f
                        << -1.54673f
                        << 0.272263f
                        << 0.777552f
                        << 0.864033f
                        << -1.68972f
                        << -0.891027f
                        << -0.234574f
                        << 0.351716f
                        << 7.89021f
                        << 7.21943f
                        << 0.903861f
                        << -0.322442f
                        << 0.923649f
                        << 0.765779f
                        << -2.53046f
                        << -2.20075f
                        << -0.275142f
                        << 1.46239f
                        )
                    << (QList<float>() //======= Neuron 15
                        << -1.1465f
                        << 0.471745f
                        << -1.57734f
                        << -2.98888f
                        << -0.575554f
                        << 0.27388f
                        << 0.820471f
                        << 7.65122f
                        << 2.23622f
                        << 4.07006f
                        << -1.08973f
                        << 0.554789f
                        << 1.55879f
                        << 3.77034f
                        << -1.01493f
                        << 0.841116f
                        << 0.281638f
                        << -1.52002f
                        << -1.15223f
                        << 0.301965f
                        << 0.209756f
                        << 0.608977f
                        << -2.17572f
                        << -2.00542f
                        << -1.17533f
                        << -1.15865f
                        )
                    << (QList<float>() //======= Neuron 16
                        << 0.195776f
                        << 0.403355f
                        << 0.766659f
                        << 0.403445f
                        << 0.182523f
                        << 0.974314f
                        << 0.787944f
                        << 0.550817f
                        << 0.340515f
                        << 0.697159f
                        << 0.822887f
                        << 0.222505f
                        << 0.37848f
                        << 0.273933f
                        << 0.0977766f
                        << 0.710982f
                        << 0.810627f
                        << 0.827038f
                        << 0.0330758f
                        << 0.345249f
                        << 0.686108f
                        << 0.410226f
                        << 0.105523f
                        << 0.874096f
                        << 0.794886f
                        << 0.25077f
                        )
                    << (QList<float>() //======= Neuron 17
                        << 0.437263f
                        << 0.112545f
                        << 0.822089f
                        << 0.785233f
                        << 0.248118f
                        << 0.0300051f
                        << 0.224622f
                        << 0.921445f
                        << 0.312252f
                        << 0.391278f
                        << 0.877626f
                        << 0.134149f
                        << 0.0501969f
                        << 0.348447f
                        << 0.75238f
                        << 0.999187f
                        << 0.56183f
                        << 0.246947f
                        << 0.195374f
                        << 0.671379f
                        << 0.969781f
                        << 0.0656514f
                        << 0.483887f
                        << 0.0486363f
                        << 0.267307f
                        << 0.160138f
                        )
                    )
                << (QList<QList<float> >() //Layer 2
                    << (QList<float>() //======= Neuron 0
                        << 0.178451f
                        << -3.976f
                        << -0.205545f
                        << 0.326222f
                        << 0.490684f
                        << 0.308852f
                        << -0.0362052f
                        << 0.164522f
                        << -0.193196f
                        << 0.40153f
                        << 0.357726f
                        << 8.67246f
                        << 0.426068f
                        << -0.25771f
                        << -0.299917f
                        << -4.15451f
                        << -8.9028f
                        << -0.249595f
                        << -0.339952f
                        )
                    << (QList<float>() //======= Neuron 1
                        << 1.18769f
                        << -7.96305f
                        << 0.787468f
                        << 0.0298932f
                        << 0.561004f
                        << 0.37888f
                        << 0.262285f
                        << 0.292738f
                        << 0.987077f
                        << 0.437383f
                        << 0.982195f
                        << 0.378854f
                        << 0.904507f
                        << 0.474314f
                        << 0.271511f
                        << -6.74124f
                        << 5.50723f
                        << 0.671667f
                        << 0.702404f
                        )
                    << (QList<float>() //======= Neuron 2
                        << 0.747726f
                        << 0.186832f
                        << 0.297231f
                        << 0.410272f
                        << 0.864299f
                        << 0.0306146f
                        << 0.418939f
                        << -0.0146307f
                        << 0.701318f
                        << 0.561328f
                        << 0.185698f
                        << 0.711099f
                        << 0.533675f
                        << 0.386463f
                        << 0.280103f
                        << 0.793565f
                        << 0.845675f
                        << 0.54373f
                        << -0.0362906f
                        )
                    << (QList<float>() //======= Neuron 3
                        << -0.747092f
                        << 5.8468f
                        << -0.848339f
                        << -0.943121f
                        << -0.769602f
                        << -1.03691f
                        << -0.487673f
                        << -0.314098f
                        << -0.755436f
                        << -0.412112f
                        << -0.898585f
                        << 8.00953f
                        << -0.653425f
                        << -0.715515f
                        << -1.28258f
                        << -2.67403f
                        << 4.37444f
                        << -0.821575f
                        << -0.722394f
                        )
                    << (QList<float>() //======= Neuron 4
                        << 0.435513f
                        << -0.066507f
                        << 0.0852716f
                        << 0.647979f
                        << 0.731186f
                        << 0.672742f
                        << 0.115318f
                        << 0.072359f
                        << 0.491632f
                        << 0.0126811f
                        << 0.666747f
                        << 0.515901f
                        << 0.428924f
                        << 0.0550359f
                        << 0.75176f
                        << 0.761755f
                        << 0.432166f
                        << -0.0494573f
                        << 0.34277f
                        )
                    << (QList<float>() //======= Neuron 5
                        << 0.0837435f
                        << 0.292227f
                        << 0.227575f
                        << 0.401492f
                        << 0.142086f
                        << 0.834204f
                        << 0.957018f
                        << 0.345003f
                        << 0.907822f
                        << 0.432286f
                        << 0.634957f
                        << 0.455256f
                        << 0.947885f
                        << 0.682272f
                        << 0.620158f
                        << 0.706003f
                        << 0.496918f
                        << 0.376103f
                        << 0.906114f
                        )
                    << (QList<float>() //======= Neuron 6
                        << 0.654446f
                        << 0.950988f
                        << 0.00950564f
                        << 0.402675f
                        << 0.547573f
                        << 0.526604f
                        << 0.546417f
                        << 0.457126f
                        << 0.327198f
                        << 0.0927709f
                        << 0.492343f
                        << 0.756157f
                        << 0.181816f
                        << 0.791764f
                        << 0.987501f
                        << 0.588576f
                        << 0.939666f
                        << 0.831041f
                        << 0.553025f
                        )
                    << (QList<float>() //======= Neuron 7
                        << 0.281296f
                        << 0.73561f
                        << 0.981949f
                        << 0.921995f
                        << 0.197693f
                        << 0.935818f
                        << 0.610236f
                        << 0.825053f
                        << 0.647473f
                        << 0.11292f
                        << 0.207159f
                        << 0.560004f
                        << 0.767486f
                        << 0.162667f
                        << 0.569349f
                        << 0.174505f
                        << 0.708094f
                        << 0.0970896f
                        << 0.719656f
                        )
                    )
                << (QList<QList<float> >() //Layer 3
                    << (QList<float>() //======= Neuron 0
                        << -1.77379f
                        << -13.5457f
                        << 4.46566f
                        << -1.25398f
                        << 10.2485f
                        << -1.84167f
                        << -1.48271f
                        << -1.7586f
                        << -1.34868f
                        )
                    << (QList<float>() //======= Neuron 1
                        << 0.880317f
                        << -3.24585f
                        << 3.10604f
                        << 1.16153f
                        << -12.6381f
                        << 0.600392f
                        << 1.10201f
                        << 1.44197f
                        << 0.559829f
                        )
                    << (QList<float>() //======= Neuron 2
                        << 0.415434f
                        << 15.9083f
                        << -6.32204f
                        << 0.530543f
                        << -11.0359f
                        << 0.537199f
                        << 0.315458f
                        << 0.359619f
                        << 1.05985f
                        )
                    << (QList<float>() //======= Neuron 3
                        << -2.53008f
                        << 2.14123f
                        << 18.1963f
                        << -2.83274f
                        << 1.06212f
                        << -2.51476f
                        << -1.98554f
                        << -2.1052f
                        << -2.24029f
                        )
                    )
                   ;

        m_network->setNetWeights(weights);
    }

    QList<float> runset;

    for (int i = 0; i < ui->display->rowCount(); i++)
    {
        for (int j = 0; j < ui->display->columnCount(); j++)
        {
            runset.append(ui->display->item(i,j)->background() == Qt::black ? 1 : 0);
        }
    }

    qDebug() << runset;

    QList<float> *netOutputs = m_network->run(runset);
    QString realNetOut;

    unsigned int n = 0;
    for (int i = 0; i < netOutputs->size(); i++)
    {
        n <<= 1;
        if (round(netOutputs->at(i)) == 1.0f)
        {
            n += 1;
        }

        realNetOut.append(QString::number(round(netOutputs->at(i)))).append(" (").append(QString::number(netOutputs->at(i))).append(")");
        if (i != netOutputs->size() -1)
        {
            realNetOut.append(";\n");
        }
    }

    delete netOutputs;

    qDebug() << "Recognized number:" << n;

    ui->recognizedNrLabel->setText(QString::number(n));
    ui->netOutLabel->setText(realNetOut);
}

void MainWindow::paintNumber()
{
    //clear the raster
    for (int i = 0; i < ui->display->rowCount(); i++)
    {
        for (int j = 0; j < ui->display->columnCount(); j++)
        {
            ui->display->item(i,j)->setBackground(Qt::white);
        }
    }

    QPushButton *button = qobject_cast<QPushButton*>(sender());

    switch(button->text().toInt())
    {
    case 1:
        ui->display->item(0,3)->setBackground(Qt::black);
        ui->display->item(1,2)->setBackground(Qt::black);
        ui->display->item(1,3)->setBackground(Qt::black);
        ui->display->item(2,1)->setBackground(Qt::black);
        ui->display->item(2,3)->setBackground(Qt::black);
        ui->display->item(3,3)->setBackground(Qt::black);
        ui->display->item(4,3)->setBackground(Qt::black);
        break;
    case 2:
        ui->display->item(0,2)->setBackground(Qt::black);
        ui->display->item(0,3)->setBackground(Qt::black);
        ui->display->item(1,1)->setBackground(Qt::black);
        ui->display->item(1,4)->setBackground(Qt::black);
        ui->display->item(2,3)->setBackground(Qt::black);
        ui->display->item(3,2)->setBackground(Qt::black);
        ui->display->item(4,1)->setBackground(Qt::black);
        ui->display->item(4,2)->setBackground(Qt::black);
        ui->display->item(4,3)->setBackground(Qt::black);
        ui->display->item(4,4)->setBackground(Qt::black);
        break;
    case 3:
        ui->display->item(0,1)->setBackground(Qt::black);
        ui->display->item(0,2)->setBackground(Qt::black);
        ui->display->item(0,3)->setBackground(Qt::black);
        ui->display->item(1,3)->setBackground(Qt::black);
        ui->display->item(2,1)->setBackground(Qt::black);
        ui->display->item(2,2)->setBackground(Qt::black);
        ui->display->item(2,3)->setBackground(Qt::black);
        ui->display->item(3,3)->setBackground(Qt::black);
        ui->display->item(4,1)->setBackground(Qt::black);
        ui->display->item(4,2)->setBackground(Qt::black);
        ui->display->item(4,3)->setBackground(Qt::black);
        break;
    case 4:
        ui->display->item(0,1)->setBackground(Qt::black);
        ui->display->item(0,3)->setBackground(Qt::black);
        ui->display->item(1,1)->setBackground(Qt::black);
        ui->display->item(1,3)->setBackground(Qt::black);
        ui->display->item(2,1)->setBackground(Qt::black);
        ui->display->item(2,2)->setBackground(Qt::black);
        ui->display->item(2,3)->setBackground(Qt::black);
        ui->display->item(3,3)->setBackground(Qt::black);
        ui->display->item(4,3)->setBackground(Qt::black);
        break;
    case 5:
        ui->display->item(0,1)->setBackground(Qt::black);
        ui->display->item(0,2)->setBackground(Qt::black);
        ui->display->item(0,3)->setBackground(Qt::black);
        ui->display->item(1,1)->setBackground(Qt::black);
        ui->display->item(2,1)->setBackground(Qt::black);
        ui->display->item(2,2)->setBackground(Qt::black);
        ui->display->item(2,3)->setBackground(Qt::black);
        ui->display->item(3,3)->setBackground(Qt::black);
        ui->display->item(4,1)->setBackground(Qt::black);
        ui->display->item(4,2)->setBackground(Qt::black);
        ui->display->item(4,3)->setBackground(Qt::black);
        break;
    case 6:
        ui->display->item(0,1)->setBackground(Qt::black);
        ui->display->item(0,2)->setBackground(Qt::black);
        ui->display->item(0,3)->setBackground(Qt::black);
        ui->display->item(1,1)->setBackground(Qt::black);
        ui->display->item(2,1)->setBackground(Qt::black);
        ui->display->item(2,2)->setBackground(Qt::black);
        ui->display->item(2,3)->setBackground(Qt::black);
        ui->display->item(3,1)->setBackground(Qt::black);
        ui->display->item(3,3)->setBackground(Qt::black);
        ui->display->item(4,1)->setBackground(Qt::black);
        ui->display->item(4,2)->setBackground(Qt::black);
        ui->display->item(4,3)->setBackground(Qt::black);
        break;
    case 7:
        ui->display->item(0,1)->setBackground(Qt::black);
        ui->display->item(0,2)->setBackground(Qt::black);
        ui->display->item(0,3)->setBackground(Qt::black);
        ui->display->item(1,3)->setBackground(Qt::black);
        ui->display->item(2,3)->setBackground(Qt::black);
        ui->display->item(3,3)->setBackground(Qt::black);
        ui->display->item(4,3)->setBackground(Qt::black);
        break;
    case 8:
        ui->display->item(0,1)->setBackground(Qt::black);
        ui->display->item(0,2)->setBackground(Qt::black);
        ui->display->item(0,3)->setBackground(Qt::black);
        ui->display->item(1,1)->setBackground(Qt::black);
        ui->display->item(1,3)->setBackground(Qt::black);
        ui->display->item(2,1)->setBackground(Qt::black);
        ui->display->item(2,2)->setBackground(Qt::black);
        ui->display->item(2,3)->setBackground(Qt::black);
        ui->display->item(3,1)->setBackground(Qt::black);
        ui->display->item(3,3)->setBackground(Qt::black);
        ui->display->item(4,1)->setBackground(Qt::black);
        ui->display->item(4,2)->setBackground(Qt::black);
        ui->display->item(4,3)->setBackground(Qt::black);
        break;
    case 9:
        ui->display->item(0,1)->setBackground(Qt::black);
        ui->display->item(0,2)->setBackground(Qt::black);
        ui->display->item(0,3)->setBackground(Qt::black);
        ui->display->item(1,1)->setBackground(Qt::black);
        ui->display->item(1,3)->setBackground(Qt::black);
        ui->display->item(2,1)->setBackground(Qt::black);
        ui->display->item(2,2)->setBackground(Qt::black);
        ui->display->item(2,3)->setBackground(Qt::black);
        ui->display->item(3,3)->setBackground(Qt::black);
        ui->display->item(4,1)->setBackground(Qt::black);
        ui->display->item(4,2)->setBackground(Qt::black);
        ui->display->item(4,3)->setBackground(Qt::black);
        break;
    case 0:
        ui->display->item(0,1)->setBackground(Qt::black);
        ui->display->item(0,2)->setBackground(Qt::black);
        ui->display->item(0,3)->setBackground(Qt::black);
        ui->display->item(1,1)->setBackground(Qt::black);
        ui->display->item(1,3)->setBackground(Qt::black);
        ui->display->item(2,1)->setBackground(Qt::black);
        ui->display->item(2,3)->setBackground(Qt::black);
        ui->display->item(3,1)->setBackground(Qt::black);
        ui->display->item(3,3)->setBackground(Qt::black);
        ui->display->item(4,1)->setBackground(Qt::black);
        ui->display->item(4,2)->setBackground(Qt::black);
        ui->display->item(4,3)->setBackground(Qt::black);
        break;
    }
}
