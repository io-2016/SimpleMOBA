#ifndef DEAGLE_HPP
#define DEAGLE_HPP

#include "Weapon.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"

class Deagle : public Weapon {
 private:
 public:
  Deagle(SceneGraph::Item* = nullptr);

  bool write(QJsonObject&) const;

  void shoot();
};

class Laser : public SceneGraph::Item {
 private:
  friend class Deagle;

  QPointF m_start, m_end;
  QPointF m_currentStart, m_currentEnd;
  QPointF m_unitVec;

  void advanceAnimation();

  struct RayCastCallback : public b2RayCastCallback {
    b2Fixture* m_fixture;
    b2Vec2 m_point;
    b2Vec2 m_normal;
    float32 m_fraction;

    float32 ReportFixture(b2Fixture* fixture, const b2Vec2& point,
                          const b2Vec2& normal, float32 fraction);
  };

 protected:
  // QSGNode* updatePaintNode(QSGNode *, UpdatePaintNodeData *);
  void timerEvent(QTimerEvent*);

 public:
  Laser(QPointF p1, QPointF p2, float speedFactor,
        SceneGraph::Item* parent = nullptr);
};

#endif  // DEAGLE_HPP
