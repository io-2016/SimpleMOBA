#ifndef STATICLIGHT_HPP
#define STATICLIGHT_HPP
#include "Light.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"

class DynamicLight;

class StaticLight : public Light {
 private:
  friend class DynamicLight;

  bool m_dynamicShadows;
  Box2DBox m_fixture;
  DynamicLight* m_dynamicLight;

 protected:
  void synchronize();
  void visibleChanged();

 public:
  StaticLight(SceneGraph::Item* = nullptr);
  ~StaticLight();

  void initialize(QWorld*);
  void destroyBody();

  inline bool dynamicShadows() const { return m_dynamicShadows; }
  inline void setDynamicShadows(bool d) { m_dynamicShadows = d; }

  inline DynamicLight* dynamicLight() const { return m_dynamicLight; }

  bool read(const QJsonObject&);
  bool write(QJsonObject&) const;
};

#endif  // STATICLIGHT_HPP
