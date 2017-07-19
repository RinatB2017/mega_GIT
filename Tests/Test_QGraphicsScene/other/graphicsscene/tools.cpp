#include "tools.h"
#include "figures/figure.h"

#include <QGridLayout>
#include <QPushButton>

Tools::Tools(QWidget *parent) : QWidget(parent) {
  QPushButton *but_1, *but_2;

  QGridLayout *grid = new QGridLayout(this);

  but_1 = new QPushButton("rect", this);
  but_2 = new QPushButton("rhomb", this);

  grid->addWidget(but_1, 0, 0, 1, 1);
  grid->addWidget(but_2, 1, 0, 1, 1);

  connect(but_1, SIGNAL(clicked()), SLOT(on_process_clicked()));
  connect(but_2, SIGNAL(clicked()), SLOT(on_switch_clicked()));
}

void Tools::on_process_clicked() { emit tool_changed(FigureType::Rect); }

void Tools::on_switch_clicked() { emit tool_changed(FigureType::Rhomb); }
