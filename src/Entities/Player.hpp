#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <QSound>
#include <memory>
#include "QBox2D/QBody.hpp"
#include "Utility/Path.hpp"

class Bullet : public QBody {
 private:
  QPointF m_direction;
  std::shared_ptr<QSound> m_punchSound;

  void onStepped();

 protected:
  void beginContact(QFixture* other, b2Contact*);

 public:
  using QBody::QBody;

  void initialize(QWorld*);
  void setDirection(QPointF);
  void setSound(std::shared_ptr<QSound>);
};

class Player;

class PlayerObject : public QObject {
 private:
  Q_OBJECT

  friend class Player;

  Q_PROPERTY(uint health READ health NOTIFY healthChanged)
  Q_PROPERTY(uint maxHealth READ maxHealth NOTIFY maxHealthChanged)
  Q_PROPERTY(uint healthRegen READ healthRegen NOTIFY healthRegenChanged)
  Q_PROPERTY(uint mana READ mana NOTIFY manaChanged)
  Q_PROPERTY(uint maxMana READ maxMana NOTIFY maxManaChanged)
  Q_PROPERTY(uint manaRegen READ manaRegen NOTIFY manaRegenChanged)

  Player* m_player;

  uint m_health;
  uint m_maxHealth;
  uint m_healthRegen;
  uint m_mana;
  uint m_maxMana;
  uint m_manaRegen;

 public:
  PlayerObject(Player*);

  uint health() const;
  uint maxHealth() const;
  uint healthRegen() const;
  uint mana() const;
  uint maxMana() const;
  uint manaRegen() const;

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
  friend class PlayerObject;

  QPointF m_target;
  std::unique_ptr<Path> m_currentPath;
  size_t m_currentPathPoint;
  bool m_going;
  std::string m_activeSpell;
  std::shared_ptr<QSound> m_punchSound;
  PlayerObject m_object;

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
};

#endif  // PLAYER_HPP
