#ifndef QWORLD_H
#define QWORLD_H

#include <QQuickItem>
#include <QBasicTimer>
#include <QOpenGLFunctions>
#include <Box2D/Box2D.h>
#include <functional>
#include <deque>
#include "QBody.hpp"

class QWorld;
class QFixture;

class QContactListener : public b2ContactListener {
 public:
  void BeginContact(b2Contact* contact);
  void EndContact(b2Contact* contact);
  void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
  void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

class QDestructionListener : public b2DestructionListener {
 public:
  void SayGoodbye(b2Joint* joint);
  void SayGoodbye(b2Fixture* fixture);
};

class QWorld : public SceneGraph::Item, public QOpenGLFunctions {
 private:
  friend class QBody;
  friend class QFixture;

  bool m_running;
  bool m_locked;
  float m_timeStep;
  int m_velocityIterations;
  int m_positionIterations;
  int m_frameTime;

  b2World m_world;
  QBody m_groundBody;
  QContactListener m_contactListener;
  QDestructionListener m_destructionListener;
  int m_timer;
  QRectF m_visibleRect;
  std::vector<QBody*> m_visibleBodies;
  std::vector<QBody*> m_destroyed;
  std::vector<QBody*> m_enqueued;
  bool m_glinitialized;

  void updateVisibleBodies();
  void destroyBodies();

 protected:
  struct BodyFinder : public b2QueryCallback {
    std::vector<QBody*> m_foundBodies;
    bool ReportFixture(b2Fixture* fixture);
  };

  struct FixtureFinder : public b2QueryCallback {
    std::vector<QFixture*> m_foundFixtures;
    bool ReportFixture(b2Fixture* fixture);
  };

  void timerEvent(QTimerEvent*);

  virtual void onBodyDestroyed(QBody*);
  virtual void onBodyAdded(QBody*);

  virtual void onFixtureDestroyed(QFixture*);
  virtual void onFixtureAdded(QFixture*);

  inline bool isRunning() const { return m_running; }
  void setRunning(bool running);

  virtual void releaseResource(QBody*);

 public:
  explicit QWorld(SceneGraph::Item* parent = 0);
  ~QWorld();

  virtual void initialize();
  virtual void step();

  inline QBody* groundBody() { return &m_groundBody; }
  inline bool locked() const { return m_locked; }

  inline float timeStep() const { return m_timeStep; }
  inline void setTimeStep(float t) { m_timeStep = t; }

  inline int velocityIterations() const { return m_velocityIterations; }
  inline void setVelocityIterations(int i) { m_velocityIterations = i; }

  inline int positionIterations() const { return m_positionIterations; }
  inline void setPositionIterations(int i) { m_positionIterations = i; }

  inline int frameTime() const { return m_frameTime; }
  inline void setFrameTime(int frameTime) { m_frameTime = frameTime; }

  QPointF gravity() const;
  void setGravity(const QPointF& gravity);

  inline QRectF visibleRect() const { return m_visibleRect; }
  inline void setVisibleRect(QRectF rect) { m_visibleRect = rect; }

  inline b2World* world() { return &m_world; }
  inline const b2World* world() const { return &m_world; }

  void destroyBody(QBody*);

  std::vector<QBody*> bodies();
  std::vector<const QBody*> bodies() const;

  std::vector<QBody*> bodies(const QRectF& rect) const;

  inline const std::vector<QBody*>& visibleBodies() const {
    return m_visibleBodies;
  }

  QBody* bodyUnderPoint(const QPointF& p,
                        std::function<bool(QBody*)> ok = nullptr) const;
  QFixture* fixtureUnderPoint(const QPointF& p) const;

  std::vector<QFixture*> fixtures(const QRectF& rect) const;

  void rayCast(b2RayCastCallback* raycast, QPointF p1, QPointF p2) const;
};

#endif  // BOX2DWORLD_H
