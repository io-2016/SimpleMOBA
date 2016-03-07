#include "World.hpp"
#include "Entities/Game.hpp"
#include "Lighting/LightSystem.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QFixture.hpp"
#include "SceneGraph/Window.hpp"
#include "Utility/Factory.hpp"
#include "Utility/Utility.hpp"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <unordered_set>

World::World(ViewWorld *viewWorld)
    : QWorld(viewWorld), m_viewWorld(viewWorld),
      m_mainAction(this, std::unique_ptr<FileActionResolver>(
                             new WorldFileActionResolver(this))),
      m_worldObject(this) {}

World::~World() { clear(); }

void World::step() {
  QWorld::step();

  particleSystem()->step();
}

void World::clear() { itemSet()->clear(); }

void World::onBodyAdded(QBody *body) {
  QWorld::onBodyAdded(body);

  assert(lightSystem());
  lightSystem()->addBody(body);
}

void World::onFixtureDestroyed(QFixture *f) {
  lightSystem()->onFixtureDestroyed(f);
}

LightSystem *World::lightSystem() const {
  return view()->game()->lightSystem();
}

ParticleSystem *World::particleSystem() const {
  return view()->game()->particleSystem();
}

void World::setPaused(bool p) {
  if (paused() == p)
    return;
  setRunning(!p);
  setFocus(!p);
}

WorldObject::WorldObject(World *world) : m_world(world), m_fps() {
  m_fpscounter.restart();
  connect(world->window(), &SceneGraph::Window::beforeRendering, this,
          &WorldObject::updateFps);
}

void WorldObject::updateFps() {
  qreal t = m_fpscounter.restart();

  if (!qFuzzyIsNull(t))
    setFps(1000.0 / t);
}

void WorldObject::setFps(qreal f) {
  m_fps = f;
  emit fpsChanged();
}

WorldFileActionResolver::WorldFileActionResolver(World *w) : m_world(w) {}

void WorldFileActionResolver::load(QString path) const {
  m_world->view()->game()->load(path);
}

void WorldFileActionResolver::dump(QString path) const {
  m_world->view()->game()->dump(path);
}
