#include "World.hpp"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <unordered_set>
#include "QBox2D/QFixture.hpp"
#include "QBox2D/QBody.hpp"
#include "Lighting/LightSystem.hpp"
#include "Entities/Player.hpp"
#include "Entities/Gun.hpp"
#include "Entities/ItemSet.hpp"
#include "Entities/Game.hpp"
#include "Utility/Utility.hpp"
#include "Utility/Factory.hpp"
#include "SceneGraph/Window.hpp"

World::World(ViewWorld* viewWorld)
    : QWorld(viewWorld),
      m_viewWorld(viewWorld),
      m_player(),
      m_itemSet(this),
      m_mainAction(this),
      m_worldObject(this) {}

World::~World() { clear(); }

void World::step() {
  QWorld::step();

  particleSystem()->step();
}

void World::clear() {
  Player* item = player();
  setPlayer(nullptr);

  if (item) {
    item->destroyBody();
    delete item;
  }

  m_itemSet.clear();
}

void World::onBodyDestroyed(QBody* body) {
  if (player() == body) setPlayer(nullptr);
  if (mainAction()->mapEditor()->grabItem()->m_grabbedBody == body)
    mainAction()->mapEditor()->grabItem()->releaseItem();

  QWorld::onBodyDestroyed(body);
}

void World::onBodyAdded(QBody* body) {
  QWorld::onBodyAdded(body);

  assert(lightSystem());
  lightSystem()->addBody(body);
}

void World::onFixtureDestroyed(QFixture* f) {
  lightSystem()->onFixtureDestroyed(f);
}

void World::releaseResource(QBody* body) {
  if (m_itemSet.contains(body)) m_itemSet.removeBody(body);
}

void World::setPlayer(Player* player) {
  if (m_player == player) return;

  m_player = player;

  if (player) player->setFocus(true);

  view()->setFocusedObject(player);

  emit object()->playerChanged();
  emit object()->equippedWeaponChanged();
}

LightSystem* World::lightSystem() const {
  return view()->game()->lightSystem();
}

ParticleSystem* World::particleSystem() const {
  return view()->game()->particleSystem();
}

void World::setPaused(bool p) {
  if (paused() == p) return;
  setRunning(!p);
  setFocus(!p);
}

void World::read(const QJsonObject& obj) {
  m_itemSet.read(obj);

  QJsonObject p = obj["player"].toObject();
  Player* player = Utility::create<Player>(p["class"].toString().toLocal8Bit());
  assert(player);

  player->setParent(this);
  player->read(p);
  player->initialize(this);

  setPlayer(player);
}

void World::write(QJsonObject& obj) const {
  m_itemSet.write(obj);

  if (player()) {
    QJsonObject p;
    player()->write(p);
    obj["player"] = p;
  }
}

WorldObject::WorldObject(World* world) : m_world(world), m_fps() {
  m_fpscounter.restart();
  connect(world->window(), &SceneGraph::Window::beforeRendering, this,
          &WorldObject::updateFps);
}

bool WorldObject::player() {
  return m_world->player() ? m_world->player()->focus() : false;
}

void WorldObject::updateFps() {
  qreal t = m_fpscounter.restart();

  if (!qFuzzyIsNull(t)) setFps(1000.0 / t);
}

void WorldObject::setFps(qreal f) {
  m_fps = f;
  emit fpsChanged();
}

uint WorldObject::playerHealth() const {
  if (m_world->player() == nullptr) return 0;

  return m_world->player()->health();
}

bool WorldObject::equippedWeapon() const {
  if (m_world->player() == nullptr) return false;

  return m_world->player()->hand()->grabbedWeapon() != nullptr;
}

uint WorldObject::bulletCount() const {
  assert(equippedWeapon());
  return m_world->player()->hand()->grabbedWeapon()->bulletCount();
}

void WorldObject::playerEnableGoLeft() { m_world->player()->enableGoLeft(); }

void WorldObject::playerDisableGoLeft() { m_world->player()->disableGoLeft(); }

void WorldObject::playerEnableGoRight() { m_world->player()->enableGoRight(); }

void WorldObject::playerDisableGoRight() {
  m_world->player()->disableGoRight();
}

void WorldObject::playerJumpRequested() {
  if (m_world->player()->currentState() & Player::OnGround)
    m_world->player()->jumpRequested();
}

void WorldObject::playerPunchRequested() {
  m_world->player()->punchRequested();
}

void WorldObject::playerDropWeapon() {
  m_world->player()->hand()->dropWeapon();
}
