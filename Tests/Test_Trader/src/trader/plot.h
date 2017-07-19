//--------------------------------------------------------------------------------
#ifndef _PLOT_H_
#define _PLOT_H_
//--------------------------------------------------------------------------------
#include <qwt_samples.h>
#include <qwt_plot.h>
//--------------------------------------------------------------------------------
class QwtPlotTradingCurve;
//--------------------------------------------------------------------------------
class Plot: public QwtPlot
{
    Q_OBJECT

public:
    Plot(const QString &ticket_name, QWidget * = 0);
    QString get_ticket_name(void);
    void update_ticket(void);

public slots:
    void setMode(int);
    void exportPlot(void);
    bool append(int time,
                float open,
                float close,
                float low,
                float high);

private slots:
    void showItem(QwtPlotItem *, bool on);

private:
    QwtPlotTradingCurve *curve;
    QVector<QwtOHLCSample> tickets;
    QString ticket_name;

    void populate(void);
};
//--------------------------------------------------------------------------------
#endif
