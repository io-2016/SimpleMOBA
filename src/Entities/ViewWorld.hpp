#ifndef VIEWWORLD_HPP
#define VIEWWORLD_HPP
#include "Entities/World.hpp"
#include "Lighting/LightSystem.hpp"
#include "Utility/DisplayItem.hpp"

class ViewWorld : public DisplayItem {
 private:
  friend class Game;

  World m_world;
  Game* m_game;

 protected:
  void visibleAreaChanged();

 public:
  explicit ViewWorld(Game*);
  void initialize();

  inline Game* game() const { return m_game; }

  inline World* world() { return &m_world; }
  inline const World* world() const { return &m_world; }

  bool read(const QJsonObject& obj);
  bool write(QJsonObject& obj) const;
};

#endif  // VIEWWORLD_HPP
