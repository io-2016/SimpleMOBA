#include "Box2DBox.hpp"
#include "Utility/Utility.hpp"
#include <QJsonObject>

Box2DBox::Box2DBox(Item* parent)
    : Fixture(parent), m_texturedRectangle(this), m_normalMapRectangle() {
  setTexture(&m_texturedRectangle);
  setNormalMap(&m_normalMapRectangle);
}

void Box2DBox::setTextureRect(QRectF rect) {
  m_texturedRectangle.setTextureRect(rect);
  m_normalMapRectangle.setTextureRect(rect);
}

std::vector<QPointF> Box2DBox::vertices() const {
  return {QPointF(0, 0), QPointF(size().width(), 0),
          QPointF(size().width(), size().height()), QPointF(0, size().height()),
          QPointF(0, 0)};
}

void Box2DBox::setSize(QSizeF s) {
  if (m_size != s) {
    m_size = s;
    m_texturedRectangle.scale(s.width(), s.height());
    m_normalMapRectangle.scale(s.width(), s.height());
  }
}

bool Box2DBox::read(const QJsonObject& obj) {
  Fixture::read(obj);

  setSize(QSizeF(obj["width"].toDouble(), obj["height"].toDouble()));
  setTextureRect(Utility::Json::toRect(obj["textureRect"].toObject()));
  return true;
}

bool Box2DBox::write(QJsonObject& obj) const {
  Fixture::write(obj);

  obj["class"] = QString("Box2DBox");
  obj["width"] = size().width();
  obj["height"] = size().height();
  obj["textureRect"] = Utility::Json::toObject(textureRect());
  return true;
}

b2Shape* Box2DBox::createShape() const {
  b2Vec2 vertices[4];
  vertices[0].Set(position().x(), position().y());
  vertices[1].Set(position().x() + size().width(), position().y());
  vertices[2].Set(position().x() + size().width(),
                  position().y() + size().height());
  vertices[3].Set(position().x(), position().y() + size().height());

  b2PolygonShape* shape = new b2PolygonShape;
  shape->Set(vertices, 4);

  return shape;
}
