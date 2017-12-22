#ifndef TOOLS_H
# define TOOLS_H
# include "figures/figure.h"
# include "figures/figuretype.h"
# include <QWidget>

class Tools : public QWidget {
  Q_OBJECT
public:
  explicit Tools(QWidget *parent = 0);
signals:
  void tool_changed(FigureType type);
public slots:
  void on_process_clicked();
  void on_switch_clicked();
};

#endif // TOOLS_H
