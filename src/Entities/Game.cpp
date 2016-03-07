#include "Game.hpp"
#include <fstream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>

Game::Game(Item* parent)
    : DisplayItemFrame(parent),
      m_viewWorld(this),
      m_lightSystem(this),
      m_particleSystem(m_lightSystem.lightTexture()->sourceItem()) {
  view()->initialize();
  lightSystem()->initialize();

  particleSystem()->setLightSystem(lightSystem());

  load(":/json/map00.json");
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
  view()->world()->clear();
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
