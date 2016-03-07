#ifndef GUN_HPP
#define GUN_HPP
#include "Weapon.hpp"
#include "Geometry/Circle.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/Fixture/Box2DCircle.hpp"

class Box2DBox;

class Gun : public Weapon {
 public:
  Gun(Item* = nullptr);

  bool write(QJsonObject&) const;

  void shoot();
};

class Bullet : public QBody {
 protected:
  Box2DCircle m_circle;

  void beginContact(QFixture* other, b2Contact*);

 public:
  Bullet(Circle circle, SceneGraph::Item* = nullptr);
  void destroyBody();

  inline static Type* key() {
    static Type t;
    return &t;
  }
  inline Type* type() const { return key(); }
};

#endif  // GUN_HPP
