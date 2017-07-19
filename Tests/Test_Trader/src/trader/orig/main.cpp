#include <stdlib.h>
#include <qapplication.h>
#include <qpen.h>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_price.h>
#include <qwt_price_move.h>
#include <qwt_series_data.h>

int main(int argc, char **argv)
{
    QApplication a(argc, argv);

    QwtPlot plot;
    plot.setCanvasBackground(QColor(Qt::gray));
    plot.setTitle("Demo : Google Inc. (GOOG)");

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableXMin(true);
    grid->enableYMin(true);
    grid->setMajPen(QPen(Qt::black, 0, Qt::DotLine));
    grid->setMinPen(QPen(Qt::gray, 0 , Qt::DotLine));
    grid->attach(&plot);

    QwtPlotPrice *price = new QwtPlotPrice();

    // Set the style of the plot :
    //  * QwtPlotPrice::Candlestick for Japaneses candlestick
    //  * QwtPlotPrice::OHLC for open-high-low-close barchart
    price->setStyle(QwtPlotPrice::Candlestick);

    // Outline color of the price items
    price->setPen(QPen(Qt::black));

    // Set the colors of the moves
    // By default : transparent when the price goes up, black when it goes down
    price->setUpBrush(Qt::green);
    price->setDownBrush(Qt::red);


    // Sample data : 'GOOG' (Google Inc.), one month from 2009/02/01
    // Thanks Yahoo! finance.
    QwtArray<QwtPriceMove> sample(30);
    sample[0].interval = QwtDoubleInterval(1237302000,1237332600);
    sample[0].open = 334.81;
    sample[0].close = 333.10;
    sample[0].high = 340.00;
    sample[0].low = 328.05;

    sample[1].interval = QwtDoubleInterval(1237215600,1237246200);
    sample[1].open = 320.18;
    sample[1].close = 335.34;
    sample[1].high = 335.34;
    sample[1].low = 319.09;

    sample[2].interval = QwtDoubleInterval(1237129200,1237159800);
    sample[2].open = 325.99;
    sample[2].close = 319.69;
    sample[2].high = 329.73;
    sample[2].low = 318.59;

    sample[3].interval = QwtDoubleInterval(1236870000,1236900600);
    sample[3].open = 326.10;
    sample[3].close = 324.42;
    sample[3].high = 327.46;
    sample[3].low = 319.03;

    sample[4].interval = QwtDoubleInterval(1236783600,1236814200);
    sample[4].open = 317.54;
    sample[4].close = 323.53;
    sample[4].high = 325.00;
    sample[4].low = 313.65;

    sample[5].interval = QwtDoubleInterval(1236697200,1236727800);
    sample[5].open = 310.10;
    sample[5].close = 317.91;
    sample[5].high = 320.00;
    sample[5].low = 305.71;

    sample[6].interval = QwtDoubleInterval(1236610800,1236641400);
    sample[6].open = 298.25;
    sample[6].close = 308.17;
    sample[6].high = 310.50;
    sample[6].low = 294.25;

    sample[7].interval = QwtDoubleInterval(1236524400,1236555000);
    sample[7].open = 299.98;
    sample[7].close = 290.89;
    sample[7].high = 306.57;
    sample[7].low = 289.45;

    sample[8].interval = QwtDoubleInterval(1236265200,1236295800);
    sample[8].open = 307.22;
    sample[8].close = 308.57;
    sample[8].high = 310.19;
    sample[8].low = 294.25;

    sample[9].interval = QwtDoubleInterval(1236178800,1236209400);
    sample[9].open = 316.48;
    sample[9].close = 305.64;
    sample[9].high = 319.08;
    sample[9].low = 302.64;

    sample[10].interval = QwtDoubleInterval(1236092400,1236123000);
    sample[10].open = 323.16;
    sample[10].close = 318.92;
    sample[10].high = 329.00;
    sample[10].low = 315.38;

    sample[11].interval = QwtDoubleInterval(1236006000,1236036600);
    sample[11].open = 330.07;
    sample[11].close = 325.48;
    sample[11].high = 333.69;
    sample[11].low = 322.35;

    sample[12].interval = QwtDoubleInterval(1235919600,1235950200);
    sample[12].open = 333.33;
    sample[12].close = 327.16;
    sample[12].high = 340.70;
    sample[12].low = 326.00;

    sample[13].interval = QwtDoubleInterval(1235660400,1235691000);
    sample[13].open = 332.95;
    sample[13].close = 337.99;
    sample[13].high = 343.82;
    sample[13].low = 331.11;

    sample[14].interval = QwtDoubleInterval(1235574000,1235604600);
    sample[14].open = 345.96;
    sample[14].close = 337.18;
    sample[14].high = 352.49;
    sample[14].low = 337.16;

    sample[15].interval = QwtDoubleInterval(1235487600,1235518200);
    sample[15].open = 342.15;
    sample[15].close = 341.64;
    sample[15].high = 352.30;
    sample[15].low = 338.92;

    sample[16].interval = QwtDoubleInterval(1235401200,1235431800);
    sample[16].open = 331.02;
    sample[16].close = 345.45;
    sample[16].high = 349.62;
    sample[16].low = 330.89;

    sample[17].interval = QwtDoubleInterval(1235314800,1235345400);
    sample[17].open = 347.00;
    sample[17].close = 330.06;
    sample[17].high = 349.80;
    sample[17].low = 329.55;

    sample[18].interval = QwtDoubleInterval(1235055600,1235086200);
    sample[18].open = 338.05;
    sample[18].close = 346.45;
    sample[18].high = 348.92;
    sample[18].low = 335.00;

    sample[19].interval = QwtDoubleInterval(1234969200,1234999800);
    sample[19].open = 357.47;
    sample[19].close = 342.64;
    sample[19].high = 359.80;
    sample[19].low = 341.41;

    sample[20].interval = QwtDoubleInterval(1234882800,1234913400);
    sample[20].open = 347.24;
    sample[20].close = 353.11;
    sample[20].high = 353.38;
    sample[20].low = 340.52;

    sample[21].interval = QwtDoubleInterval(1234796400,1234827000);
    sample[21].open = 346.51;
    sample[21].close = 342.66;
    sample[21].high = 347.09;
    sample[21].low = 339.69;

    sample[22].interval = QwtDoubleInterval(1234450800,1234481400);
    sample[22].open = 362.19;
    sample[22].close = 357.68;
    sample[22].high = 362.99;
    sample[22].low = 355.23;

    sample[23].interval = QwtDoubleInterval(1234364400,1234395000);
    sample[23].open = 353.16;
    sample[23].close = 363.05;
    sample[23].high = 363.62;
    sample[23].low = 351.48;

    sample[24].interval = QwtDoubleInterval(1234278000,1234308600);
    sample[24].open = 358.95;
    sample[24].close = 358.04;
    sample[24].high = 365.00;
    sample[24].low = 353.00;

    sample[25].interval = QwtDoubleInterval(1234191600,1234222200);
    sample[25].open = 375.98;
    sample[25].close = 358.51;
    sample[25].high = 377.50;
    sample[25].low = 357.89;

    sample[26].interval = QwtDoubleInterval(1234105200,1234135800);
    sample[26].open = 371.28;
    sample[26].close = 378.77;
    sample[26].high = 381.00;
    sample[26].low = 367.30;

    sample[27].interval = QwtDoubleInterval(1233846000,1233876600);
    sample[27].open = 356.46;
    sample[27].close = 371.28;
    sample[27].high = 373.81;
    sample[27].low = 355.44;

    sample[28].interval = QwtDoubleInterval(1233759600,1233790200);
    sample[28].open = 340.91;
    sample[28].close = 353.72;
    sample[28].high = 355.38;
    sample[28].low = 337.00;

    sample[29].interval = QwtDoubleInterval(1233673200,1233703800);
    sample[29].open = 340.07;
    sample[29].close = 343.00;
    sample[29].high = 354.44;
    sample[29].low = 339.17;


    price->setData(QwtPriceMoveSeriesData(sample));
    price->attach(&plot);

    plot.replot();

    plot.resize(600,400);
    plot.show();

    return a.exec(); 
}