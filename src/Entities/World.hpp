#ifndef WORLD_HPP
#define WORLD_HPP

#include <QElapsedTimer>

#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QWorld.hpp"

#include "Actions/MainAction.hpp"

#include "Graphics/ParticleSystem.hpp"

class Game;
class LightSystem;
class ViewWorld;
class World;

class WorldObject : public QObject {
 private:
  Q_OBJECT

  Q_PROPERTY(qreal fps READ fps NOTIFY fpsChanged)

  World *m_world;

  qreal m_fps;
  QElapsedTimer m_fpscounter;
  int var;

  void updateFps();
  void setFps(qreal);

 public:
  WorldObject(World *);

  uint playerHealth() const;
  bool equippedWeapon() const;
  uint bulletCount() const;

  inline qreal fps() const { return m_fps; }

 signals:
  void fpsChanged();
};

class WorldFileActionResolver : public FileActionResolver {
 private:
  World *m_world;

 public:
  WorldFileActionResolver(World *);

  void load(QString) const;
  void dump(QString) const;
};

class World : public QWorld {
 private:
  friend class ViewWorld;

  ViewWorld *m_viewWorld;
  MainAction m_mainAction;

  WorldObject m_worldObject;

 protected:
  void onBodyAdded(QBody *);
  void onFixtureDestroyed(QFixture *);

 public:
  explicit World(ViewWorld *);
  ~World();

  void step();
  void clear();

  inline ViewWorld *view() const { return m_viewWorld; }

  LightSystem *lightSystem() const;
  ParticleSystem *particleSystem() const;

  inline bool paused() const { return !isRunning(); }
  void setPaused(bool);

  inline MainAction *mainAction() { return &m_mainAction; }
  inline const MainAction *mainAction() const { return &m_mainAction; }

  inline WorldObject *object() { return &m_worldObject; }
  inline const WorldObject *object() const { return &m_worldObject; }
};

#endif  // WORLD_HPP
