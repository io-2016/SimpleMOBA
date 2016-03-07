#include "ViewWorld.hpp"
#include "Player.hpp"
#include "Game.hpp"
#include <QJsonObject>

ViewWorld::ViewWorld(Game* game)
    : DisplayItem(game), m_world(this), m_game(game) {}

void ViewWorld::initialize() {
  m_world.initialize();
  m_world.setFocus(true);
}

bool ViewWorld::read(const QJsonObject& obj) {
  m_world.read(obj["world"].toObject());

  setSize(QSizeF(obj["width"].toDouble(), obj["height"].toDouble()));
  setFactor(obj["scale"].toDouble());

  return true;
}

bool ViewWorld::write(QJsonObject& obj) const {
  QJsonObject world;
  m_world.write(world);
  obj["world"] = world;

  obj["width"] = size().width();
  obj["height"] = size().height();
  obj["scale"] = factor();

  return true;
}

void ViewWorld::visibleAreaChanged() {
  DisplayItem::visibleAreaChanged();

  game()->lightSystem()->visibleAreaChanged(visibleArea());
  world()->setVisibleRect(visibleArea());
}
