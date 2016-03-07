#ifndef WORLD_HPP
#define WORLD_HPP

#include <QElapsedTimer>

#include "QBox2D/QWorld.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"

#include "Actions/MainAction.hpp"

#include "Graphics/ParticleSystem.hpp"

#include "ItemSet.hpp"

class Game;
class Player;
class LightSystem;
class ViewWorld;

class WorldObject : public QObject {
 private:
  Q_OBJECT

  Q_PROPERTY(bool player READ player NOTIFY playerChanged)
  Q_PROPERTY(uint playerHealth READ playerHealth NOTIFY playerHealthChanged)
  Q_PROPERTY(
      bool equippedWeapon READ equippedWeapon NOTIFY equippedWeaponChanged)
  Q_PROPERTY(uint bulletCount READ bulletCount NOTIFY bulletCountChanged)
  Q_PROPERTY(qreal fps READ fps NOTIFY fpsChanged)

  World* m_world;

  qreal m_fps;
  QElapsedTimer m_fpscounter;
  int var;

  void updateFps();
  void setFps(qreal);

 public:
  WorldObject(World*);

  bool player();

  uint playerHealth() const;
  bool equippedWeapon() const;
  uint bulletCount() const;

  inline qreal fps() const { return m_fps; }

  Q_INVOKABLE void playerEnableGoLeft();
  Q_INVOKABLE void playerDisableGoLeft();

  Q_INVOKABLE void playerEnableGoRight();
  Q_INVOKABLE void playerDisableGoRight();

  Q_INVOKABLE void playerJumpRequested();
  Q_INVOKABLE void playerPunchRequested();

  Q_INVOKABLE void playerDropWeapon();

 signals:
  void playerChanged();
  void playerHealthChanged();
  void equippedWeaponChanged();
  void bulletCountChanged();
  void fpsChanged();
};

class World : public QWorld {
 private:
  friend class ViewWorld;

  ViewWorld* m_viewWorld;

  Player* m_player;

  ItemSet m_itemSet;
  MainAction m_mainAction;

  WorldObject m_worldObject;

 protected:
  void onBodyDestroyed(QBody*);
  void onBodyAdded(QBody*);
  void onFixtureDestroyed(QFixture*);
  void releaseResource(QBody*);

 public:
  explicit World(ViewWorld*);
  ~World();

  void step();
  void clear();

  inline Player* player() const { return m_player; }
  void setPlayer(Player* p);

  inline ViewWorld* view() const { return m_viewWorld; }

  LightSystem* lightSystem() const;
  ParticleSystem* particleSystem() const;

  inline bool paused() const { return !isRunning(); }
  void setPaused(bool);

  inline MainAction* mainAction() { return &m_mainAction; }
  inline const MainAction* mainAction() const { return &m_mainAction; }

  inline WorldObject* object() { return &m_worldObject; }
  inline const WorldObject* object() const { return &m_worldObject; }

  ItemSet* itemSet() { return &m_itemSet; }
  const ItemSet* itemSet() const { return &m_itemSet; }

  void read(const QJsonObject&);
  void write(QJsonObject&) const;
};

#endif  // WORLD_HPP
