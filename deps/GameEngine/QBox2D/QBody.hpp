#ifndef QBODY_HPP
#define QBODY_HPP

#include <QQuickItem>
#include <Box2D/Box2D.h>
#include <queue>
#include <functional>
#include "Utility/List.hpp"
#include "Utility/BaseItem.hpp"
#include "Geometry/Circle.hpp"

class QFixture;
class QWorld;

class QBody : public BaseItem {
 private:
  friend class QWorld;
  friend class QFixture;
  friend class ViewWorld;
  friend class QContactListener;

  b2Body* m_body;
  b2BodyDef m_bodyDef;

  List<QFixture*> m_fixtureList;
  QWorld* m_world;

  SceneGraph::Item m_content;
  std::queue<std::function<void()> > m_work;
  uint m_id;
  bool m_pendingDestroy;

  void removeFixture(QFixture*);

 protected:
  virtual void synchronize();
  virtual void fixtureAdded(QFixture*);
  virtual void fixtureDestroyed(QFixture*);
  virtual void releaseResource(QFixture*);

  virtual void beginContact(QFixture* other, b2Contact*);
  virtual void endContact(QFixture* other, b2Contact*);
  virtual void preSolve(QFixture* other, b2Contact*, const b2Manifold*);
  virtual void postSolve(QFixture* other, b2Contact*, const b2ContactImpulse*);

  virtual bool read(const QJsonObject&);

  void visibleChanged();

 public:
  struct Type {};

  enum BodyType { Static, Kinematic, Dynamic };

  explicit QBody(SceneGraph::Item* parent = nullptr);
  ~QBody();

  virtual void initialize(QWorld*);
  virtual void destroyBody();

  QPointF position() const;
  void setPosition(QPointF);

  inline qreal rotation() const { return m_bodyDef.angle * 180.0 / M_PI; }
  void setRotation(qreal);

  inline qreal linearDamping() const { return m_bodyDef.linearDamping; }
  void setLinearDamping(qreal linearDamping);

  inline qreal angularDamping() const { return m_bodyDef.angularDamping; }
  void setAngularDamping(qreal angularDamping);

  inline qreal gravityScale() const { return m_bodyDef.gravityScale; }
  void setGravityScale(qreal gravityScale);

  BodyType bodyType() const;
  void setBodyType(BodyType bodyType);

  inline bool isBullet() const { return m_bodyDef.bullet; }
  void setBullet(bool bullet);

  inline bool sleepingAllowed() const { return m_bodyDef.allowSleep; }
  void setSleepingAllowed(bool allowed);

  inline bool fixedRotation() const { return m_bodyDef.fixedRotation; }
  void setFixedRotation(bool fixedRotation);

  inline bool active() const { return m_bodyDef.active; }
  void setActive(bool active);

  QPointF linearVelocity() const;
  void setLinearVelocity(const QPointF& linearVelocity);

  inline qreal angularVelocity() const { return m_bodyDef.angularVelocity; }
  void setAngularVelocity(qreal velocity);

  inline SceneGraph::Item* content() { return &m_content; }
  inline const SceneGraph::Item* content() const { return &m_content; }

  void setTransform(QPointF position, qreal rotation);

  void addFixture(QFixture* f);
  bool addFixture(const QJsonObject&, QFixture* f);

  QFixture* firstFixture() const;

  void applyLinearImpulse(const QPointF& impulse, const QPointF& point);
  void applyTorque(qreal torque);
  void applyForce(const QPointF& force);
  QPointF worldCenter() const;

  inline const b2Body* body() const { return m_body; }
  inline b2Body* body() { return m_body; }

  inline QWorld* world() const { return m_world; }

  QRectF boundingRect() const;

  virtual bool testPoint(const QPointF& point) const;
  bool testOverlap(const QRectF& rect) const;
  bool testOverlap(const QBody* other) const;

  void enqueueFunction(std::function<void()>);

  void initializeLater(QWorld*);
  void destroyLater();

  bool initialize(const QJsonObject&, QWorld* world);
  virtual bool write(QJsonObject&) const;

  virtual Type* type() const { return nullptr; }

  inline uint id() const { return m_id; }

  static QBody* toQBody(b2Body* body);
  static const QBody* toQBody(const b2Body* body);
};

#endif  // QBODY_HPP
