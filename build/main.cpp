#include "Utility/Window.hpp"
#include <QGuiApplication>

int main(int argc, char **argv) {
  QGuiApplication app(argc, argv);
  // app.setOverrideCursor(Qt::BlankCursor);
  // qputenv("QSG_RENDER_LOOP", "threaded");

  Window window;
  window.show();

  window.game()->view()->setLookAt(QPointF(800, 600));
  window.game()->view()->setFactor(5);
  window.game()->view()->setFlickable(true);
  window.game()->lightSystem()->setVisible(false);

  return app.exec();
}
