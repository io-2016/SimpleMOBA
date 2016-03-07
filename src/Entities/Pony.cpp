#include "Pony.hpp"
#include "World.hpp"
#include <QJsonObject>

Pony::Pony(SceneGraph::Item* parent)
    : Creature(parent), m_body(this), m_wings(this) {}

void Pony::initialize(QWorld* w) {
  Creature::initialize(w);

  m_body.initialize(m_bodySource.c_str());
  m_body.translate(-0.5 * size().width(), -0.5 * size().height());
  m_body.scale(size().width(), size().height());

  m_wings.initialize(m_wingsSource.c_str());
  m_wings.translate(-0.5 * size().width(), -0.5 * size().height());
  m_wings.scale(size().width(), size().height());

  setCurrentSprite("standStill");
}

void Pony::setCurrentSprite(const char* sprite) {
  m_body.setCurrentSprite(sprite);
  m_wings.setCurrentSprite(sprite);
}

void Pony::jumpRequested() { setCurrentSprite("prepareJump"); }

void Pony::punchRequested() { setCurrentSprite("preparePunch"); }

bool Pony::read(const QJsonObject& obj) {
  Creature::read(obj);

  m_size.setWidth(obj["width"].toDouble());
  m_size.setHeight(obj["height"].toDouble());
  m_bodySource = obj["bodySource"].toString().toStdString();
  m_wingsSource = obj["wingsSource"].toString().toStdString();

  for (QFixture* f = firstFixture(); f != nullptr; f = f->next()) {
    Box2DBox* box = static_cast<Box2DBox*>(f);
    //box->setTextureSource(":/resources/crate.jpg");
    //box->setDensity(0.025);
  }

  return true;
}

bool Pony::write(QJsonObject& obj) const {
  Creature::write(obj);

  obj["class"] = QString("Pony");
  obj["bodySource"] = QString(m_bodySource.c_str());
  obj["wingsSource"] = QString(m_wingsSource.c_str());
  obj["width"] = m_size.width();
  obj["height"] = m_size.height();

  return true;
}

void Pony::currentStateChanged(unsigned added, unsigned removed) {
  Creature::currentStateChanged(added, removed);

  m_body.updateState(added, removed);
  m_wings.updateState(added, removed);
}
