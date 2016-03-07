#ifndef BODYEDIT_HPP
#define BODYEDIT_HPP
#include "SubAction.hpp"

class QBody;
class QMouseEvent;
class BodyEdit;

class BodyEditObject : public ActionObject {
 private:
  Q_OBJECT

  Q_PROPERTY(bool body READ body NOTIFY bodyChanged)

  Q_PROPERTY(qreal density READ density WRITE setDensity NOTIFY update)
  Q_PROPERTY(qreal friction READ friction WRITE setFriction NOTIFY update)
  Q_PROPERTY(
      qreal restitution READ restitution WRITE setRestitution NOTIFY update)
  Q_PROPERTY(
      qreal gravityScale READ gravityScale WRITE setGravityScale NOTIFY update)
  Q_PROPERTY(qreal linearDamping READ linearDamping WRITE setLinearDamping
                 NOTIFY update)
  Q_PROPERTY(qreal angularDamping READ angularDamping WRITE setAngularDamping
                 NOTIFY update)
  Q_PROPERTY(bool dynamic READ dynamic WRITE setDynamic NOTIFY update)

 public:
  BodyEditObject(BodyEdit*);

  BodyEdit* action() const;

  bool body() const;

  qreal density() const;
  void setDensity(qreal);

  qreal friction() const;
  void setFriction(qreal);

  qreal restitution() const;
  void setRestitution(qreal);

  qreal gravityScale() const;
  void setGravityScale(qreal);

  qreal linearDamping() const;
  void setLinearDamping(qreal);

  qreal angularDamping() const;
  void setAngularDamping(qreal);

  bool dynamic() const;
  void setDynamic(bool);

  Q_INVOKABLE void applyChanges();

 signals:
  void bodyChanged();
  void update();
};

class BodyEdit : public SubAction {
 private:
  friend class BodyEditObject;

  QBody* m_body;
  qreal m_density;
  qreal m_friction;
  qreal m_restitution;
  qreal m_gravityScale;
  qreal m_linearDamping;
  qreal m_angularDamping;
  bool m_dynamic;

  BodyEditObject m_object;

 protected:
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);

 public:
  BodyEdit(MapEditor*);
  void reset();

  inline QString name() const { return "BodyEdit"; }

  void applyChanges() const;
};

#endif  // BODYEDIT_HPP
