#include "Game.hpp"
#include <fstream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

#include "Path.hpp"

Game::Game(Item* parent)
    : DisplayItemFrame(parent),
      m_viewWorld(this),
      m_lightSystem(this, view()->world()),
      m_particleSystem(lightSystem()->lightTexture()->sourceItem()) {
  view()->initialize();
  lightSystem()->initialize();

  particleSystem()->setLightSystem(lightSystem());

  load(":/json/map00.json");

  Path path(QPointF(712, 615), QPointF(743, 647), view()->world());
  for (const auto &p : path.points()) {
    TexturedRectangle *rect = new TexturedRectangle(view()->world());
    rect->translate(p.x(), p.y());
    rect->scale(1, 1);
    rect->setSource(":/resources/crate.jpg");
  }
}

Game::~Game() { clear(); }

void Game::sizeChanged() {
  DisplayItemFrame::sizeChanged();

#ifdef Q_OS_ANDROID
  const QSize maxres(800, 600);
#else
  const QSize maxres(1920, 1080);
#endif

  QSize resolution(std::min(int(size().width()), maxres.width()),
                   std::min(int(size().height()), maxres.height()));

  lightSystem()->setResolution(resolution);
  lightSystem()->setSize(size());
}

bool Game::read(const QJsonObject& obj) {
  view()->read(obj["viewWorld"].toObject());
  lightSystem()->read(obj["lightSystem"].toObject());

  return true;
}

bool Game::write(QJsonObject& obj) const {
  QJsonObject viewWorld;
  view()->write(viewWorld);
  obj["viewWorld"] = viewWorld;

  QJsonObject lights;
  lightSystem()->write(lights);
  obj["lightSystem"] = lights;

  return true;
}

void Game::clear() {
  lightSystem()->clear();
  view()->world()->itemSet()->clear();
}

void Game::load(QString path) {
  clear();

  QFile file(path);
  if (!file.open(QFile::ReadOnly)) qDebug() << "failed to open map file";

  QByteArray data = file.readAll();

  QJsonDocument document(QJsonDocument::fromJson(data));
  read(document.object());
}

void Game::dump(QString path) const {
  QJsonObject obj;
  write(obj);

  QJsonDocument document(obj);
  std::fstream file(path.toStdString(), std::ios::out);
  assert(file.is_open());
  file << document.toJson().constData();
}
