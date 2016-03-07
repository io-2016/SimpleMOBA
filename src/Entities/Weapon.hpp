#ifndef WEAPON_HPP
#define WEAPON_HPP

#include "QBox2D/QBody.hpp"
#include "Graphics/TexturedRectangle.hpp"

class Hand;

class Weapon : public QBody {
 private:
  QPointF m_shootPoint;
  int m_bulletCount;
  int m_flip;

  QPointF m_effectiveShootPoint;
  Hand* m_grabber;

  TexturedRectangle m_texture;
  QSizeF m_size;

  void update();
  void updateEffectiveShootPoint();

  inline void setEffectiveShootPoint(QPointF p) { m_effectiveShootPoint = p; }

 protected:
  void preSolve(QFixture* other, b2Contact*, const b2Manifold*);

 public:
  Weapon(SceneGraph::Item* parent = nullptr);

  void initialize(QWorld*);
  void destroyBody();

  virtual void shoot() = 0;

  inline QSizeF size() const { return m_size; }
  void setSize(QSizeF);

  inline QPointF effectiveShootPoint() const { return m_effectiveShootPoint; }

  inline QPointF shootPoint() const { return m_shootPoint; }
  void setShootPoint(QPointF p);

  inline int bulletCount() const { return m_bulletCount; }
  void setBulletCount(int);

  inline Hand* grabber() const { return m_grabber; }
  inline void setGrabber(Hand* grabber) { m_grabber = grabber, update(); }

  inline int flip() const { return m_flip; }
  void setFlip(int);

  bool read(const QJsonObject&);
  bool write(QJsonObject&) const;

  inline static Type* key() {
    static Type t;
    return &t;
  }
  inline Type* type() const { return key(); }
};

#endif  // WEAPON_HPP
