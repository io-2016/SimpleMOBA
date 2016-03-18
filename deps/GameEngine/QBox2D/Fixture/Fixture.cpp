#include "Fixture.hpp"
#include "Graphics/TexturedItem.hpp"
#include "Lighting/LightSystem.hpp"
#include <QJsonObject>

Fixture::Fixture(SceneGraph::Item* parent)
    : QFixture(parent), m_texture(), m_normalMap() {}

void Fixture::initialize(QBody* b) {
  QFixture::initialize(b);

  if (normalMap()) normalMap()->setParent(content());
}

QString Fixture::textureSource() const {
  return texture() ? texture()->source() : "";
}

void Fixture::setTextureSource(QString path) {
  if (texture()) texture()->setSource(path);
}

QString Fixture::normalMapSource() const {
  return normalMap() ? normalMap()->source() : "";
}

void Fixture::setNormalMapSource(QString path) {
  if (normalMap()) normalMap()->setSource(path);
}

bool Fixture::read(const QJsonObject& obj) {
  QFixture::read(obj);

  setTextureSource(obj["textureSource"].toString());
  setNormalMapSource(obj["normalMapSource"].toString());
  return true;
}

bool Fixture::write(QJsonObject& obj) const {
  QFixture::write(obj);

  obj["textureSource"] = textureSource();
  obj["normalMapSource"] = normalMapSource();
  return true;
}
