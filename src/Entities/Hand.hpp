#ifndef HAND_HPP
#define HAND_HPP
#include "Weapon.hpp"
#include "QBox2D/QJoint.hpp"

class Player;

class Hand : public SceneGraph::Item {
 private:
  friend class Weapon;
  friend class Player;

  Player* m_owner;
  Weapon* m_grabbedWeapon;
  QMouseJoint* m_mouseJoint;
  QPointF m_position;
  float m_range;

  void updateGrabbedWeapon();

 public:
  Hand(Player* owner = nullptr);
  ~Hand();

  void setHandPosition(QPointF pos);
  inline QPointF position() const { return m_position; }

  inline Player* owner() const { return m_owner; }

  inline Weapon* grabbedWeapon() const { return m_grabbedWeapon; }
  void setGrabbedWeapon(Weapon* weapon);

  inline float range() const { return m_range; }
  inline void setRange(float range) { m_range = range; }

  void grabWeapon(Weapon* item);
  void dropWeapon();
};
#endif  // HAND_HPP
