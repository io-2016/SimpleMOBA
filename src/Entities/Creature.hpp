#ifndef CREATURE_HPP
#define CREATURE_HPP

#include "QBox2D/QBody.hpp"

class Creature : public QBody {
 private:
  struct RayCastCallback : public b2RayCastCallback {
    QBody *m_hit;

    float32 ReportFixture(b2Fixture *fixture, const b2Vec2 &point,
                          const b2Vec2 &normal, float32 fraction);
  };

  unsigned m_state;
  int m_health;
  QPointF m_force;

  void updateOnGround();
  void onWorldStepped();

 protected:
  virtual inline void healthChanged() {}
  virtual void currentStateChanged(unsigned added, unsigned removed);

 public:
  explicit Creature(Item * = nullptr);
  void initialize(QWorld *);

  enum State {
    GoingLeft = 1 << 0,
    GoingRight = 1 << 1,
    OnGround = 1 << 2,
    TurnedLeft = 1 << 3,
    TurnedRight = 1 << 4
  };

  inline void setForce(const QPointF &force) { m_force = force; }
  inline QPointF force() const { return m_force; }
  void addForce(const QPointF &);

  inline unsigned currentState() const { return m_state; }
  void modifyState(unsigned enable, unsigned disable);
  void enableState(unsigned state);
  void disableState(unsigned state);

  inline int health() const { return m_health; }
  void setHealth(int);

  bool read(const QJsonObject &);
  bool write(QJsonObject &) const;

  bool canSee(QBody *);

  void jump();
  void punch();

  void enableGoLeft();
  void enableGoRight();
  void disableGoLeft();
  void disableGoRight();

  virtual void jumpRequested();
  virtual void punchRequested();
};

#endif  // CREATURE_HPP
