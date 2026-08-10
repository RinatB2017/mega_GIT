/***************************************************************************
 *   Copyright (C) 2008 - Giuseppe Cigala                                  *
 *   g_cigala@virgilio.it                                                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#include "qfreq.h"

QFreq::QFreq(QWidget *parent) : QWidget(parent)
{
    initValue();
    initColor();

}

void QFreq::initValue()
{
    start_point = 0;
    zoom_point = ((1024/2)+1)*20000/22050;
    freq1 = 0;
    freq2 = 5;
    freq3 = 10;
    freq4 = 15;
    freq5 = 20;
    range_freq = 20;
    typeFFT = 0;
    lockFFT = false;

}

void QFreq::initColor()
{
	colLin = Qt::black;
    colLog = Qt::black;
    col512 = Qt::black;
    col1024 = Qt::black;
    col2048 = Qt::black;
    col4096 = Qt::black;
    colLock = Qt::red;
}

void QFreq::setZoomPoint(int N)
{
    zoom_point = ((N/2)+1)*20000/22050;
}

void QFreq::calculateFFT(int N)
{
    /*
       QFile file ("test.wav");
       if (!file.open(QIODevice::ReadOnly))
           return;
       char *data;
       file.read(data, N);

       for (i =0; i <=N; i++)
       {
           in[i] = (double)data[i];
           i++;
       }
      
       */
    double *in;
    double *out;
    fftw_plan plan;
    double A[N];
    magnitudeLin = 0;
    magnitudeLog = 0;
    min_val_lin = 5000000;
    max_val_lin = -1000;
    min_val_log = 5000000;
    max_val_log = -1000;
    int is16bitfile = 1;
    int val, val2;
    char inc[N];
    FILE *infile;
    const char * fileName = wavFile.toLatin1().data();
    infile = fopen(fileName, "r");

    // skip wav file header
    fgets(inc,37,infile);
    fgets(inc,9,infile);

    // read data and fill "A"-array
    for (int v=0; v<N; v++)
    {
        val = fgetc(infile);
        if (is16bitfile)
        {
            val2 = fgetc(infile);
            if (val2 > 127)
            {
                val2 = val2 - 255;
            }
            A[v] = 256*val2 + val;
        }
        else
        {
            A[v] = val;
        }

    }

    // prepare fft with fftw
    out = (double *)fftw_malloc(N * sizeof(double));
    in = (double *)fftw_malloc(N * sizeof(double));
    magnitudeLin = (double *)fftw_malloc(N * sizeof(double));
    magnitudeLog = (double *)fftw_malloc(N * sizeof(double));

    // create the fftw plan
    plan = fftw_plan_r2r_1d(N, in, out, FFTW_R2HC, FFTW_ESTIMATE);

    // we have no imaginary data, so clear idata
    memset((void *)out, 0, N * sizeof(double));

    // fill rdata with actual data
    for (int i = 0; i < N; i++)
    {
        in[i] = A[i];
    }

    // make fft
    fftw_execute(plan);

    // post-process FFT data: make absolute values, and calculate
    // real frequency of each power line in the spectrum (20 KHz : 22.05 KHz = num_p : (N/2)+1)
    // typeFFT = 0 -> linear, 1 -> log

    int num_p = ((N/2)+1)*20000/22050;

    for (int i = 1; i < num_p; i++)
    {
        magnitudeLin[i] = abs(2.0*sqrt(out[i]*out[i])/N);
        magnitudeLog[i] = abs(10*log10(2.0*sqrt(out[i]*out[i])/N));

        // find min and max lin value
        if (magnitudeLin[i] > max_val_lin)
            max_val_lin = magnitudeLin[i];
        if (magnitudeLin[i] < min_val_lin)
            min_val_lin = magnitudeLin[i];

        // find min and max log value
        if (magnitudeLog[i] > max_val_log)
            max_val_log = magnitudeLog[i];
        if (magnitudeLog[i] < min_val_log)
            min_val_log = magnitudeLog[i];
    }



// housekeeping

    fclose(infile);
    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

}


void QFreq::paintEvent(QPaintEvent *)
{
    paintBorder();
    paintFreq();
    paintScale();
    paintControl();
}


void QFreq::paintBorder()

{
    QPainter painter(this);
    painter.setWindow(0, 0, 800, 360);
    painter.setRenderHint(QPainter::Antialiasing);

    QLinearGradient linGrad(20, 50, 180, 250);
    linGrad.setColorAt(0, Qt::gray);
    linGrad.setColorAt(1, Qt::white);
    linGrad.setSpread(QGradient::ReflectSpread);
    painter.setBrush(linGrad);
    QRectF border(10, 10, 780, 340);
    painter.drawRoundRect(border, 5, 5);

    // draw rack point
    painter.setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.setBrush(Qt::white);
    painter.drawRoundRect(20, 80, 20, 13, 30, 30);
    painter.drawRoundRect(20, 260, 20, 13, 30, 30);
    painter.drawRoundRect(760, 80, 20, 13, 30, 30);
    painter.drawRoundRect(760, 260, 20, 13, 30, 30);

    QLinearGradient linGrad1(50, 180, 57, 180);
    linGrad1.setColorAt(0, Qt::white);
    linGrad1.setColorAt(1, QColor(80,80,80));
    linGrad1.setSpread(QGradient::PadSpread);
    painter.setBrush(linGrad1);

    painter.setPen(QPen(Qt::black, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    QRectF border1(50, 40, 700, 280);
    painter.drawRect(border1);
    painter.setPen(QPen(Qt::black, 5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(750, 318, 750, 42);

	painter.setBrush(Qt::black);
	QRectF panel(102, 63, 595, 197);
	painter.drawRect(panel);

}


void QFreq::paintFreq()
{
    if (wavFile.isEmpty())
        return;
    else
    {
        QPainter painter(this);
        painter.setWindow(0, 0, 800, 360);
        painter.setRenderHint(QPainter::Antialiasing);

        QLinearGradient linGrad(400, 320, 400, 40);
        linGrad.setColorAt(0, Qt::green);
        linGrad.setColorAt(1, Qt::red);
        linGrad.setSpread(QGradient::PadSpread);
        painter.setBrush(linGrad);

        painter.setPen(QPen(linGrad, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

        // draw the frequency value
        // X axis is 700 pt. - Y axis is 360 pt.
        // 1024/2 pt : 22050 Hz = x : 20000 Hz -> x = 464 ?????
        // DC freq (0 kz) not draw
        // typeFFT = 0 linear 1 logaritmic

        if (typeFFT == 0)
        {
            for (int i = 1; i <= zoom_point-1; i++)
            {
                painter.drawLine(100+(i*600/zoom_point), 260, 100+(i*600/zoom_point), (260-195*(magnitudeLin[i+start_point]/max_val_lin)));
            }
        }
        if (typeFFT == 1)
        {
            for (int i = 1; i <= zoom_point-1; i++)
            {
                painter.drawLine(100+(i*600/zoom_point), 260, 100+(i*600/zoom_point), (260-195*(magnitudeLog[i+start_point]/max_val_log)));
            }
        }
    }


}


void QFreq::paintScale()
{
    QPainter painter(this);
    painter.setWindow(0, 0, 800, 360);
    painter.setRenderHint(QPainter::Antialiasing);

    //draw scale

    painter.setPen(QPen(Qt::yellow, 1, Qt::SolidLine));
    painter.drawLine(100, 265, 700, 265);

    for (int i = 0; i <= 4; i++)
    {
        painter.drawLine(100+(150*i), 265, 100+(150*i), 275);
    }

    painter.save();

    for (int i = 1; i <= 4; i++)
    {
        for (int i = 1; i <= 4; i++)
        {
            painter.drawLine(100+(30*i), 265, 100+(30*i), 270);
        }
        painter.translate(150,0);
    }

    painter.restore();

    // draw frequency value

    QRectF a(80, 275, 40, 20);
    QRectF b(230, 275, 40, 20);
    QRectF c(380, 275, 40, 20);
    QRectF d(530, 275, 40, 20);
    QRectF e(680, 275, 40, 20);
    QFont labFont("Arial", 8, QFont::Normal);
    painter.setFont(labFont);
    QString fr1 = QString("%1").arg(freq1, 0, 'f', 2);
    QString fr2 = QString("%1").arg(freq2 ,0, 'f', 2);
    QString fr3 = QString("%1").arg(freq3, 0, 'f', 2);
    QString fr4 = QString("%1").arg(freq4, 0, 'f', 2);
    QString fr5 = QString("%1").arg(freq5, 0, 'f', 2);
    painter.drawText(a, Qt::AlignCenter, fr1);
    painter.drawText(b, Qt::AlignCenter, fr2);
    painter.drawText(c, Qt::AlignCenter, fr3);
    painter.drawText(d, Qt::AlignCenter, fr4);
    painter.drawText(e, Qt::AlignCenter, fr5);


    // draw vertical dot line
    painter.setPen(QPen(Qt::darkGray, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));

    painter.save();
    for (int i = 1; i <= 4; i++)
    {
        for (int i = 1; i <= 4; i++)
        {
            painter.drawLine(100+(i*30), 260, 100+(i*30), 60);
        }
        painter.translate(150,0);
    }
    painter.restore();

    painter.setPen(QPen(Qt::white, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));

    painter.save();
    for (int i = 0; i <= 4; i++)
    {
        painter.drawLine(100+(i*75), 260, 100+(i*75), 60);
        painter.translate(75,0);
    }
    painter.restore();

    // draw orizzontal dot line
    painter.setPen(QPen(Qt::darkGray, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    for (int i = 0; i <=3 ; i++)
    {
        painter.drawLine(100, 60+(i*50), 700, 60+(i*50));
    }

    // draw label
    painter.setPen(QPen(Qt::white, 1, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin));
    QFont fFont("Arial", 8, QFont::Normal);
    painter.setFont(fFont);
    QRectF f(350, 290, 100, 20);
    painter.drawText(f, Qt::AlignCenter, "Frequency [KHz]");
    QRectF m(60, 150, 30, 20);
    painter.drawText(m, Qt::AlignCenter, "Magn");


}

void QFreq::paintControl()

{
    QPainter painter(this);
    painter.setWindow(0, 0, 800, 360);
    painter.setRenderHint(QPainter::Antialiasing);
    // select color lin-log
    painter.setBrush(colLin);
    painter.drawEllipse(QRect(100, 300, 10, 10));
    painter.setBrush(colLog);
    painter.drawEllipse(QRect(145, 300, 10, 10));
    // select color point number
    painter.setBrush(col512);
    painter.drawEllipse(QRect(705, 60, 10, 10));
    painter.setBrush(col1024);
    painter.drawEllipse(QRect(705, 80, 10, 10));
    painter.setBrush(col2048);
    painter.drawEllipse(QRect(705, 100, 10, 10));
    painter.setBrush(col4096);
    painter.drawEllipse(QRect(705, 120, 10, 10));
    painter.setBrush(colLock);
    painter.drawEllipse(QRect(705, 140, 10, 10));
    // draw label lin-log
    painter.setPen(QPen(Qt::white));
    QFont labFont("Arial", 8, QFont::Normal);
    painter.setFont(labFont);
    QRectF lin(115, 300, 20, 10);
    painter.drawText(lin, Qt::AlignCenter, "Lin");
    QRectF log(160, 300, 20, 10);
    painter.drawText(log, Qt::AlignCenter, "Log");
    // draw label point number
    QFont numFont("Arial", 7, QFont::Normal);
    painter.setFont(numFont);
    QRectF p512(720, 60, 20, 10);
    painter.drawText(p512, Qt::AlignLeft, "512");
    QRectF p1024(720, 80, 20, 10);
    painter.drawText(p1024, Qt::AlignLeft, "1024");
    QRectF p2048(720, 100, 20, 10);
    painter.drawText(p2048, Qt::AlignLeft, "2048");
    QRectF p4096(720, 120, 20, 10);
    painter.drawText(p4096, Qt::AlignLeft, "4096");
    QRectF pLock(720, 140, 20, 10);
    painter.drawText(pLock, Qt::AlignLeft, "Lock");
    // draw file folder
    painter.setPen(QPen(Qt::black));
    painter.setBrush(Qt::black);
    QRectF folder(712, 172, 30, 15);
    painter.drawRoundRect(folder, 10, 10);
    painter.setPen(QPen(Qt::white));
    painter.setBrush(Qt::black);
    QRectF folder1(710, 170, 30, 15);
    painter.drawRoundRect(folder1, 10, 10);
    painter.setPen(QPen(Qt::yellow));
    QFont textFont("Arial", 8, QFont::Normal);
    painter.setFont(textFont);
    painter.drawText(folder1, Qt::AlignCenter, "FILE");
    
   
}

void QFreq::mousePressEvent(QMouseEvent *e)
{

    double x = e->x();
    double y = e->y();

    double wrun = width();
    double hrun = height();

    double wstart = 800;
    double hstart = 360;

    double dx = wrun/wstart;
    double dy = hrun/hstart;

    // set the frequency range, lin-log type and point

    if (e->button() == Qt::LeftButton)
    {
        if (y >= (100*dy) && y <= (270*dy))
        {
            if (x >= (100*dx) && x <= (250*dx) && !wavFile.isEmpty())
            {
                start_point = start_point + 0*(zoom_point/4);
                zoom_point = zoom_point/4;
                range_freq = (freq2-freq1)/4;
                freq1 = freq1 + 0*(range_freq);
                freq2 = freq1 + 1*(range_freq);
                freq3 = freq1 + 2*(range_freq);
                freq4 = freq1 + 3*(range_freq);
                freq5 = freq1 + 4*(range_freq);
                lockFFT = true;
                colLock = Qt::red;
            }
            if (x >= (251*dx) && x <= (400*dx) && !wavFile.isEmpty())
            {
                start_point = start_point + 1*(zoom_point/4);
                zoom_point = zoom_point/4;
                range_freq = (freq3-freq2)/4;
                freq1 = freq2 + 0*(range_freq);
                freq3 = freq2 + 2*(range_freq);
                freq4 = freq2 + 3*(range_freq);
                freq5 = freq2 + 4*(range_freq);
                freq2 = freq2 + 1*(range_freq);
                lockFFT = true;
                colLock = Qt::red;
            }
            if (x >= (401*dx) && x <= (550*dx) && !wavFile.isEmpty())
            {
                start_point = start_point + 2*(zoom_point/4);
                zoom_point = zoom_point/4;
                range_freq = (freq4-freq3)/4;
                freq1 = freq3 + 0*(range_freq);
                freq2 = freq3 + 1*(range_freq);
                freq4 = freq3 + 3*(range_freq);
                freq5 = freq3 + 4*(range_freq);
                freq3 = freq3 + 2*(range_freq);
                lockFFT = true;
                colLock = Qt::red;
            }
            if (x >= (551*dx) && x <= (700*dx) && !wavFile.isEmpty())
            {
                start_point = start_point + 3*(zoom_point/4);
                zoom_point = zoom_point/4;
                range_freq = (freq5-freq4)/4;
                freq1 = freq4 + 0*(range_freq);
                freq2 = freq4 + 1*(range_freq);
                freq3 = freq4 + 2*(range_freq);
                freq5 = freq4 + 4*(range_freq);
                freq4 = freq4 + 3*(range_freq);
                lockFFT = true;
                colLock = Qt::red;
            }

        }
        // lin-log event
        if (y >= (300*dy) && y <= (310*dy))
        {
            if (x >= (100*dx) && x <= (110*dx) && !wavFile.isEmpty())
            {
                colLin = Qt::red;
                colLog = Qt::black;
                typeFFT = 0;
                update();
            }
            if (x >= (145*dx) && x <= (155*dx) && !wavFile.isEmpty())
            {
                colLin = Qt::black;
                colLog = Qt::red;
                typeFFT = 1;
                update();
            }

        }
        // update FFT il lockFFT in false

        if (x >= (705*dx) && x <= (715*dx) && lockFFT == false)
        {	// 512 points
            if (y >= (60*dy) && y <= (70*dy) && !wavFile.isEmpty())
            {
                col512 = Qt::red;
                col1024 = Qt::black;
                col2048 = Qt::black;
                col4096 = Qt::black;
                setZoomPoint(512);
                calculateFFT(512);
                update();
            }
            // 1024 points
            if (y >= (80*dy) && y <= (90*dy) && !wavFile.isEmpty())
            {
                col512 = Qt::black;
                col1024 = Qt::red;
                col2048 = Qt::black;
                col4096 = Qt::black;
                setZoomPoint(1024);
                calculateFFT(1024);
                update();
            }
            // 2048 points
            if (y >= (100*dy) && y <= (110*dy) && !wavFile.isEmpty())
            {
                col512 = Qt::black;
                col1024 = Qt::black;
                col2048 = Qt::red;
                col4096 = Qt::black;
                setZoomPoint(2048);
                calculateFFT(2048);
                update();
            }
            // 4096 points
            if (y >= (120*dy) && y <= (130*dy) && !wavFile.isEmpty())
            {
                col512 = Qt::black;
                col1024 = Qt::black;
                col2048 = Qt::black;
                col4096 = Qt::red;
                setZoomPoint(4096);
                calculateFFT(4096);
                update();
            }

        }
        // file folder event

        if (x >= (710*dx) && x <= (740*dx) && lockFFT == false)
        {
            if (y >= (170*dy) && y <= (185*dy))
            {
                wavFile = QFileDialog::getOpenFileName(this, tr("Select .wav file to open"), ".", tr("Audio file (*.wav)"));
                if (wavFile.isEmpty())
                {	
                	initValue();
                	initColor();
                	wavFile = "";
                    colLock = Qt::red;
                    return;
                }
                else
                {
                    colLock = Qt::green;
                	colLin = Qt::red;
                	colLog = Qt::black;
                	col512 = Qt::black;
                	col1024 = Qt::red;
                	col2048 = Qt::black;
                	col4096 = Qt::black;
                    calculateFFT(1024);
                }
            }
        }

    }
    // set default value

    if (e->button() == Qt::RightButton)
    {
        if (y >= (100*dy) && y <= (270*dy))
        {
            if (x >= (100*dx) && x <= (700*dx) && !wavFile.isEmpty())
            {
                initValue();
                colLock = Qt::green;
                colLin = Qt::red;
                colLog = Qt::black;
                col512 = Qt::black;
                col1024 = Qt::red;
                col2048 = Qt::black;
                col4096 = Qt::black;
                calculateFFT(1024);
            }
        }

    }

    update();
}



QSize QFreq::minimumSizeHint() const
{
    return QSize(80, 36);
}

QSize QFreq::sizeHint() const
{
    return QSize(800, 360);
}


