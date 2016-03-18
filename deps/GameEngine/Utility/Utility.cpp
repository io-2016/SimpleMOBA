#include "Utility.hpp"
#include <QJsonObject>
#include <QRectF>

QJsonObject Utility::Json::toObject(QRectF rect) {
  QJsonObject obj;
  obj["x"] = rect.x();
  obj["y"] = rect.y();
  obj["w"] = rect.width();
  obj["h"] = rect.height();

  return obj;
}

QRectF Utility::Json::toRect(const QJsonObject& obj) {
  QRectF rect;
  rect.setX(obj["x"].toDouble());
  rect.setY(obj["y"].toDouble());
  rect.setWidth(obj["w"].toDouble());
  rect.setHeight(obj["h"].toDouble());

  return rect;
}

QJsonObject Utility::Json::toObject(QPointF point) {
  QJsonObject obj;
  obj["x"] = point.x();
  obj["y"] = point.y();

  return obj;
}

QPointF Utility::Json::toPoint(const QJsonObject& obj) {
  return QPointF(obj["x"].toDouble(), obj["y"].toDouble());
}

Utility::Random::Random() : m_rng(m_device()) {}

Utility::Random& Utility::Random::instance() {
  static Random random;
  return random;
}

qreal Utility::Random::real(qreal a, qreal b) {
  std::uniform_real_distribution<qreal> distribution(a, b);
  return distribution(instance().m_rng);
}
