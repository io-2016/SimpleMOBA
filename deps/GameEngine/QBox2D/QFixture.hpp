#ifndef QFIXTURE_HPP
#define QFIXTURE_HPP

#include <QQuickItem>
#include <QFlags>
#include <Box2D/Box2D.h>
#include "Utility/List.hpp"
#include "SceneGraph/Item.hpp"

class b2Fixture;
struct b2FixtureDef;
class b2Shape;
class QBody;
class TexturedItem;
class LightSystem;

class QFixture : public SceneGraph::Item {
 private:
  friend class QBody;
  friend class QDestructionListener;

  QBody* m_body;
  b2Fixture* m_fixture;
  b2FixtureDef m_fixtureDef;
  List<QFixture*>::Node m_node;

  SceneGraph::Item m_content;

  QPointF m_position;
  bool m_shadowCaster;

 protected:
  virtual void initialize(QBody* item);
  virtual b2Shape* createShape() const = 0;
  virtual bool read(const QJsonObject&);

  void matrixChanged();

 public:
  explicit QFixture(SceneGraph::Item* parent = nullptr);
  ~QFixture();

  void destroyFixture();

  enum CategoryFlag {
    Category1 = 0x0001,
    Category2 = 0x0002,
    Category3 = 0x0004,
    Category4 = 0x0008,
    Category5 = 0x0010,
    Category6 = 0x0020,
    Category7 = 0x0040,
    Category8 = 0x0080,
    Category9 = 0x0100,
    Category10 = 0x0200,
    Category11 = 0x0400,
    Category12 = 0x0800,
    Category13 = 0x1000,
    Category14 = 0x2000,
    Category15 = 0x4000,
    Category16 = 0x8000,
    All = 0xFFFF,
    None = 0x0000
  };

  virtual std::vector<QPointF> vertices() const = 0;

  inline QPointF position() const { return m_position; }
  void setPosition(QPointF p);

  inline const b2Fixture* fixture() const { return m_fixture; }

  inline QBody* body() const { return m_body; }

  inline float density() const { return m_fixtureDef.density; }
  void setDensity(float density);

  inline float friction() const { return m_fixtureDef.friction; }
  void setFriction(float friction);

  inline float restitution() const { return m_fixtureDef.restitution; }
  void setRestitution(float restitution);

  inline bool isSensor() const { return m_fixtureDef.isSensor; }
  void setSensor(bool sensor);

  CategoryFlag categories() const;
  void setCategories(CategoryFlag layers);

  CategoryFlag collidesWith() const;
  void setCollidesWith(CategoryFlag layers);

  inline int groupIndex() const { return m_fixtureDef.filter.groupIndex; }
  void setGroupIndex(int groupIndex);

  inline bool shadowCaster() const { return m_shadowCaster; }
  void setShadowCaster(bool);

  inline const SceneGraph::Item* content() const { return &m_content; }
  inline SceneGraph::Item* content() { return &m_content; }

  virtual bool write(QJsonObject&) const;

  QFixture* next() const;

  bool testOverlap(QFixture*);

  static QFixture* toQFixture(b2Fixture* fixture);
  static const QFixture* toQFixture(const b2Fixture* fixture);
};

#endif  // QFIXTURE_HPP
