#include "scene.h"

#include "figures/rectfigure.h"
#include "figures/rhombfigure.h"

#include "tools.h"

#include <QApplication>
#include <QGraphicsView>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  Scene scene;
  QGraphicsView view(&scene);

  Tools tools;

  scene.setSceneRect(-10, -10, 110, 110);

  QObject::connect(&tools, SIGNAL(tool_changed(FigureType)),
                   &scene, SLOT(on_tool_changed(FigureType)));

  view.show();
  tools.show();

  a.exec();
}
