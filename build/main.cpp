#include "Graphics/TexturedRectangle.hpp"
#include "Utility/Window.hpp"
#include <QGuiApplication>

int main(int argc, char **argv) {
  QGuiApplication app(argc, argv);
  // app.setOverrideCursor(Qt::BlankCursor);
  // qputenv("QSG_RENDER_LOOP", "threaded");

  Window::registerTypes();

  //Window window;
  //window.show();

  SceneGraph::Window window;
  window.show();

  TexturedRectangle rectangle(window.rootItem());
  rectangle.setSource(":/resources/bricks.jpg");

  return app.exec();
}
