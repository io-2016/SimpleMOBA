#include "Utility/Window.hpp"
#include <QGuiApplication>
#include <QScreen>

int main(int argc, char **argv) {
  QGuiApplication app(argc, argv);
  // app.setOverrideCursor(Qt::BlankCursor);
  // qputenv("QSG_RENDER_LOOP", "threaded");

  Utility::Window window;
  window.setFlags(Qt::Window | Qt::FramelessWindowHint);
  QSize screenGeometry = window.screen()->size();
  int x = (screenGeometry.width() - window.width()) / 2;
  int y = (screenGeometry.height() - window.height()) / 2;
  window.setPosition(QPoint(x, y));
  window.show();

  window.game()->view()->setLookAt(QPointF(70, 950));
  window.game()->view()->setFactor(7.5);
  window.game()->view()->setFlickable(false);
  window.game()->lightSystem()->setVisible(false);

  return app.exec();
}
