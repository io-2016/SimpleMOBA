#ifndef GAME_HPP
#define GAME_HPP

#include "Entities/ViewWorld.hpp"
#include "Lighting/LightSystem.hpp"

class Game : public DisplayItemFrame {
 private:
  ViewWorld m_viewWorld;
  LightSystem m_lightSystem;
  ParticleSystem m_particleSystem;

 protected:
  void sizeChanged();

 public:
  explicit Game(SceneGraph::Item* parent = 0);
  ~Game();

  inline ViewWorld* view() { return &m_viewWorld; }
  inline const ViewWorld* view() const { return &m_viewWorld; }

  inline LightSystem* lightSystem() { return &m_lightSystem; }
  inline const LightSystem* lightSystem() const { return &m_lightSystem; }

  inline ParticleSystem* particleSystem() { return &m_particleSystem; }
  inline const ParticleSystem* particleSystem() const {
    return &m_particleSystem;
  }

  bool read(const QJsonObject& obj);
  bool write(QJsonObject& obj) const;

  void clear();

  void load(QString path);
  void dump(QString path) const;
};

#endif  // GAME_HPP
