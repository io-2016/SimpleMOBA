#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QSound>
#include <QTimer>
#include <memory>
#include "QBox2D/QBody.hpp"
#include "Utility/Path.hpp"

class Player;

class Bullet : public QBody {
 private:
  QPointF m_direction;
  bool m_destroyed = false;
  std::shared_ptr<QSound> m_punchSound;
  Player* m_owner = nullptr;

  void onStepped();

 protected:
  void beginContact(QFixture* other, b2Contact*);

 public:
  using QBody::QBody;

  void initialize(QWorld*);
  void setDirection(QPointF);
  void setSound(std::shared_ptr<QSound>);

  void setOwner(Player*);
  Player* owner() const { return m_owner; };

  bool destroyed() const { return m_destroyed; }
  void setDestroyed(bool f) { m_destroyed = f; }
};

class Player;

class PlayerObject : public QObject {
 private:
  Q_OBJECT

  Q_PROPERTY(uint health READ health NOTIFY healthChanged)
  Q_PROPERTY(uint maxHealth READ maxHealth NOTIFY maxHealthChanged)
  Q_PROPERTY(uint healthRegen READ healthRegen NOTIFY healthRegenChanged)
  Q_PROPERTY(uint mana READ mana NOTIFY manaChanged)
  Q_PROPERTY(uint maxMana READ maxMana NOTIFY maxManaChanged)
  Q_PROPERTY(uint manaRegen READ manaRegen NOTIFY manaRegenChanged)

  QTimer m_regenTimer;
  Player* m_player;

  uint m_health;
  uint m_maxHealth;
  uint m_healthRegen;
  uint m_mana;
  uint m_maxMana;
  uint m_manaRegen;

  void regen();

 public:
  PlayerObject(Player*);

  uint health() const;
  uint maxHealth() const;
  uint healthRegen() const;
  uint mana() const;
  uint maxMana() const;
  uint manaRegen() const;

  void setHealth(uint);
  void setMana(uint);

 signals:
  void healthChanged();
  void maxHealthChanged();
  void healthRegenChanged();
  void manaChanged();
  void maxManaChanged();
  void manaRegenChanged();
};

class Player : public QBody {
 private:
  QPointF m_target;
  std::unique_ptr<Path> m_currentPath;
  size_t m_currentPathPoint;
  bool m_going;
  std::string m_activeSpell;
  std::shared_ptr<QSound> m_punchSound;
<<<<<<< 25630cff855aff2e158c0c311783a600aff4062a
  PlayerObject m_object;
=======
  int m_score;
>>>>>>> Added winscreen.

  void onStepped();

 protected:
  void mousePressEvent(QMouseEvent*);

 public:
  Player(SceneGraph::Item* = nullptr);
  void initialize(QWorld* w);

  bool write(QJsonObject&) const;

  void move(QPointF);
  void setActiveSpell(std::string str);
  const std::string& activeSpell() const;
  void castSpell(const QPointF& location, int spellId);

  inline PlayerObject* object() { return &m_object; }
  inline const PlayerObject* object() const { return &m_object; }
  
  void addPoints(int pts);
  int score() const { return m_score; }
};

#endif  // PLAYER_HPP
