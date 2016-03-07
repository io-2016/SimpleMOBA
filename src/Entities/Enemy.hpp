#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Creature.hpp"
#include "Pony.hpp"

class Enemy : public Pony {
 private:
  QBody* m_target;

 protected:
  void timerEvent(QTimerEvent*);
  void beginContact(QFixture* other, b2Contact*);
  void healthChanged();

 public:
  explicit Enemy(SceneGraph::Item* = nullptr);

  bool write(QJsonObject&) const;

  inline QBody* target() const { return m_target; }
  inline void setTarget(QBody* body) { m_target = body; }
};

#endif  // ENEMY_HPP
