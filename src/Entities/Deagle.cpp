#include "Deagle.hpp"
#include "QBox2D/QWorld.hpp"
#include "Graphics/Primitives.hpp"
#include <QJsonObject>

Deagle::Deagle(Item* p) : Weapon(p) {}

bool Deagle::write(QJsonObject& obj) const {
  Weapon::write(obj);
  obj["class"] = QString("Deagle");
  return true;
}

void Deagle::shoot() {
  float angle = body()->GetAngle();

  QPointF p1;  // = mapToItem(world(), effectiveShootPoint());
  QPointF p2 = p1 + QPointF(cos(angle) * 5000, sin(angle) * 5000);

  Laser::RayCastCallback callback;
  world()->rayCast(&callback, p1, p2);

  new Laser(p1, QPointF(callback.m_point.x, callback.m_point.y), 10, world());
}

Laser::Laser(QPointF p1, QPointF p2, float speedFactor, Item* parent)
    : SceneGraph::Item(parent),
      m_start(p1),
      m_end(p2),
      m_currentStart(p1),
      m_currentEnd(p1) {
  // setFlag(ItemHasContents);
  // startTimer(20);

  QPointF vec = m_end - m_start;
  qreal len = std::sqrt(vec.x() * vec.x() + vec.y() * vec.y());

  m_unitVec = speedFactor * vec / len;
}

void Laser::advanceAnimation() {
  const qreal maxlen = 100;
  m_currentEnd += m_unitVec;

  qreal l1 = QVector2D(m_currentEnd - m_start).lengthSquared();
  qreal l2 = QVector2D(m_end - m_start).lengthSquared();

  if (l1 > l2) m_currentEnd = m_end;

  if (l1 > maxlen * maxlen || m_currentEnd == m_end) {
    m_currentStart += m_unitVec;

    if (l1 > l2) {
      m_currentStart = m_end;
      // deleteLater();
    }
  }

  update();
}

/*QSGNode* Laser::updatePaintNode(QSGNode* old, UpdatePaintNodeData*) {
    EdgeNode* node = static_cast<EdgeNode*>(old);

    if (node == nullptr) {
        node = new EdgeNode(m_currentStart, m_currentEnd);
        node->setColor("green");
    }
    else {
        node->setP1(m_currentStart);
        node->setP2(m_currentEnd);
    }

    return node;
}*/

void Laser::timerEvent(QTimerEvent*) { advanceAnimation(); }

float32 Laser::RayCastCallback::ReportFixture(b2Fixture* fixture,
                                              const b2Vec2& point,
                                              const b2Vec2& normal,
                                              float32 fraction) {
  m_fixture = fixture;
  m_point = point;
  m_normal = normal;
  m_fraction = fraction;

  return fraction;
}
