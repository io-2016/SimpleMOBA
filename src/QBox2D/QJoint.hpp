#ifndef QJOINT_HPP
#define QJOINT_HPP
#include <Box2D/Box2D.h>
#include <QPointF>

class QWorld;
class QBody;

class QJoint {
 private:
  friend class QDestructionListener;

  b2Joint* m_joint;
  b2JointDef* m_jointDef;
  QWorld* m_world;
  QBody* m_bodyA;
  QBody* m_bodyB;

 protected:
  inline b2JointDef* jointDef() const { return m_jointDef; }
  inline void setJointDef(b2JointDef* def) { m_jointDef = def; }

 public:
  QJoint(QBody* b1, QBody* b2);
  virtual ~QJoint();

  virtual void initialize();

  void destroyJoint();

  inline b2Joint* joint() const { return m_joint; }
  inline QWorld* world() const { return m_world; }

  inline QBody* bodyA() const { return m_bodyA; }
  inline QBody* bodyB() const { return m_bodyB; }

  QPointF anchorA() const;
  QPointF anchorB() const;

  static QJoint* toQJoint(b2Joint*);
};

class QMouseJoint : public QJoint {
 private:
  b2MouseJointDef m_jointDef;

 public:
  QMouseJoint(QBody* body);
  void initialize();

  void setTarget(QPointF p);
  void setMaxForce(qreal f);
};

#endif  // QJOINT_HPP
