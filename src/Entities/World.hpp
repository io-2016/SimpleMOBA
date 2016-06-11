#ifndef WORLD_HPP
#define WORLD_HPP

#include <QElapsedTimer>
#include <QTimer>
#include <memory>

#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QWorld.hpp"

#include "Actions/MainAction.hpp"

#include "Graphics/ParticleSystem.hpp"

class Game;
class LightSystem;
class ViewWorld;
class World;
class Player;

class WorldObject : public QObject {
 private:
  Q_OBJECT

  Q_PROPERTY(qreal fps READ fps NOTIFY fpsChanged)
  Q_PROPERTY(
      bool minimapOnLeft MEMBER m_minimapOnLeft NOTIFY minimapOnLeftChanged)
  Q_PROPERTY(QColor playerIndicatorColor MEMBER m_playerIndicatorColor NOTIFY
                 playerIndicatorColorChanged)
  Q_PROPERTY(
      QPointF playerLocation READ playerLocation NOTIFY playerLocationChanged)
  Q_PROPERTY(QRectF cameraLocation READ cameraLocation NOTIFY cameraLocationChanged)
  Q_PROPERTY(QSizeF mapSize READ mapSize CONSTANT)

  World *m_world;

  bool m_minimapOnLeft;
  QColor m_playerIndicatorColor;
  QTimer m_playerIndicatorTimer;

  QPointF playerLocation() const;
  QRectF cameraLocation() const;
  QSizeF mapSize() const;

 public:
  WorldObject(World *);

  uint playerHealth() const;
  bool equippedWeapon() const;
  uint bulletCount() const;

  qreal fps() const;

  inline World *world() { return m_world; }
  inline const World *world() const { return m_world; }

  Q_INVOKABLE void minimapMove(QPointF);
  Q_INVOKABLE void playerMove(QPointF);

 signals:
  void fpsChanged();
  void minimapOnLeftChanged();
  void playerIndicatorColorChanged();
  void playerLocationChanged();
  void cameraLocationChanged();
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
  Player *m_player;

  WorldObject m_worldObject;

 protected:
  void onBodyAdded(QBody *);
  void onBodyDestroyed(QBody *);
  void onFixtureDestroyed(QFixture *);
  void focusChanged();

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

  inline Player *player() { return m_player; }
  inline const Player *player() const { return m_player; }

  void read(const QJsonObject &);
};

#endif  // WORLD_HPP
