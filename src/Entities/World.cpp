#include "World.hpp"
#include "Entities/Game.hpp"
#include "Lighting/LightSystem.hpp"
#include "SceneGraph/Window.hpp"
#include "Utility/Factory.hpp"
#include "Utility/Utility.hpp"

#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/Fixture/Box2DChain.hpp"
#include "QBox2D/Fixture/Box2DCircle.hpp"
#include "QBox2D/Fixture/Box2DEdge.hpp"
#include "QBox2D/Fixture/Box2DPolygon.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QChain.hpp"

#include "Entities/Player.hpp"

#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <memory>
#include <unordered_set>

World::World(ViewWorld *viewWorld)
    : QWorld(viewWorld),
      m_viewWorld(viewWorld),
      m_mainAction(this, std::make_unique<WorldFileActionResolver>(this),
                   nullptr),
      m_player(),
      m_worldObject(this) {
  factory()->registerType<Box2DBox>("Box2DBox");
  factory()->registerType<Box2DChain>("Box2DChain");
  factory()->registerType<Box2DCircle>("Box2DCircle");
  factory()->registerType<Box2DEdge>("Box2DEdge");
  factory()->registerType<Box2DPolygon>("Box2DPolygon");

  factory()->registerType<QBody>("QBody");
  factory()->registerType<QChain>("QChain");
}

World::~World() { clear(); }

void World::step() {
  QWorld::step();

  particleSystem()->step();
  lightSystem()->step();
  mainAction()->step();
}

void World::clear() { itemSet()->clear(); }

void World::onBodyAdded(QBody *body) {
  QWorld::onBodyAdded(body);

  assert(lightSystem());
  lightSystem()->addBody(body);
}

void World::onBodyDestroyed(QBody *body) {
  if (body == player()) {
    m_player = nullptr;
  }

  QWorld::onBodyDestroyed(body);
}

void World::onFixtureDestroyed(QFixture *f) {
  lightSystem()->onFixtureDestroyed(f);
}

void World::focusChanged() {
  QWorld::focusChanged();
  if (focus() && player()) player()->setFocus(true);
}

LightSystem *World::lightSystem() const {
  return view()->game()->lightSystem();
}

ParticleSystem *World::particleSystem() const {
  return view()->game()->particleSystem();
}

void World::setPaused(bool p) {
  if (paused() == p) return;
  setRunning(!p);
  setFocus(!p);
}

void World::read(const QJsonObject &obj) {
  QWorld::read(obj);

  auto player = std::make_unique<Player>(itemSet());
  player->initialize(this);
  player->setFocus(true);

  m_player = player.get();

  itemSet()->addBody(std::move(player));
}

WorldObject::WorldObject(World *world)
    : m_world(world),
      m_fps(),
      m_minimapOnLeft(true),
      m_playerIndicatorColor(QColor::fromRgb(255, 0, 0)) {
  m_fpscounter.restart();
  connect(world->window(), &SceneGraph::Window::beforeRendering, this,
          &WorldObject::updateFps);
  m_playerIndicatorTimer.setInterval(50);
  connect(&m_playerIndicatorTimer, &QTimer::timeout, this,
          &WorldObject::playerLocationChanged);
  m_playerIndicatorTimer.start();
}

void WorldObject::updateFps() {
  qreal t = m_fpscounter.restart();

  if (!qFuzzyIsNull(t)) setFps(1000.0 / t);
}

void WorldObject::setFps(qreal f) {
  m_fps = f;
  emit fpsChanged();
}

QPointF WorldObject::playerLocation() const {
  if (!world()->player()) return QPointF();
  return world()->player()->position();
}

WorldFileActionResolver::WorldFileActionResolver(World *w) : m_world(w) {}

void WorldFileActionResolver::load(QString path) const {
  m_world->view()->game()->load(QUrl(path).toLocalFile());
}

void WorldFileActionResolver::dump(QString path) const {
  m_world->view()->game()->dump(QUrl(path).toLocalFile());
}
