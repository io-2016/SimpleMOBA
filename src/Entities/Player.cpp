#include "Player.hpp"
#include "World.hpp"
#include "Weapon.hpp"
#include "Gun.hpp"
#include "Enemy.hpp"
#include "Graphics/Primitives.hpp"
#include "Geometry/Functions.hpp"
#include "Utility/DisplayItem.hpp"
#include "Lighting/LightSystem.hpp"
#include <QJsonObject>

Player::Player(Item* parent) : Pony(parent), m_hand(this) {}

void Player::keyPressEvent(QKeyEvent* event) {
  if (event->isAutoRepeat()) {
    event->ignore();
    return;
  }

  if (event->key() == Qt::Key_A)
    enableGoLeft();
  else if (event->key() == Qt::Key_D)
    enableGoRight();
  else if (event->key() == Qt::Key_Space) {
    if (currentState() & OnGround) jumpRequested();
  } else if (event->key() == Qt::Key_Shift) {
    if (currentState() & OnGround) punchRequested();
  } else
    event->ignore();
}

void Player::keyReleaseEvent(QKeyEvent* event) {
  if (event->isAutoRepeat()) {
    event->ignore();
    return;
  }

  if (event->key() == Qt::Key_A)
    disableGoLeft();
  else if (event->key() == Qt::Key_D)
    disableGoRight();
  else if (event->key() == Qt::Key_F)
    hand()->dropWeapon();
  else
    event->ignore();
}

void Player::mousePressEvent(QMouseEvent* e) {
  hand()->setHandPosition(mapFromScreen(e->pos()));

  if (hand()->grabbedWeapon()) return hand()->grabbedWeapon()->shoot();

  QPointF pos = matrix() * hand()->position();
  QBody* body = world()->bodyUnderPoint(
      pos, [](QBody* body) { return body->type() == Weapon::key(); });

  if (body) hand()->grabWeapon(static_cast<Weapon*>(body));
}

void Player::mouseMoveEvent(QMouseEvent* e) {
  hand()->setHandPosition(mapFromScreen(e->pos()));
}

void Player::beginContact(QFixture*, b2Contact*) {
  // if (qobject_cast<Enemy*>(other->body())) {
  //    setHealth(health()-10);
  //}
}

void Player::synchronize() {
  Pony::synchronize();

  m_light.setPosition(position());
  m_hand.updateGrabbedWeapon();

  QPointF position = matrix() * m_hand.position();
  Particle particle;
  particle.x = position.x();
  particle.y = position.y();
  particle.dx = 0.0;
  particle.dy = 0.0;
  particle.r = 2.0;
  particle.xvariation = 0.1;
  particle.yvariation = 0.1;
  particle.lifespan = 50;
  particle.time = world()->particleSystem()->time();

  world()->particleSystem()->addParticle(particle);
}

void Player::initialize(QWorld* w) {
  Pony::initialize(w);

  for (QFixture* f = firstFixture(); f; f = f->next()) f->setGroupIndex(-1);
  hand()->setRange(10);

  m_light.setLightSystem(world()->lightSystem());
  m_light.initialize(world());
}

void Player::destroyBody() {
  m_light.destroyBody();
  m_hand.dropWeapon();

  Pony::destroyBody();
}

World* Player::world() const { return static_cast<World*>(Creature::world()); }

void Player::healthChanged() {
  if (world() && world()->player() == this)
    world()->object()->playerHealthChanged();

  if (health() <= 0) {
    // emit world()->gameOver();

    enqueueFunction([this]() {
      world()->world()->ClearForces();
      setHealth(20);
      setTransform({450, 600}, 0);
      hand()->dropWeapon();
    });
  }
}

void Player::focusChanged() {
  if (!focus()) disableState(GoingLeft | GoingRight);

  if (world()->player() == this) emit world()->object()->playerChanged();
}

bool Player::read(const QJsonObject& obj) {
  Pony::read(obj);

  m_light.read(obj["light"].toObject());

  return true;
}

bool Player::write(QJsonObject& obj) const {
  Pony::write(obj);

  obj["class"] = QString("Player");

  QJsonObject light;
  m_light.write(light);
  obj["light"] = light;

  return true;
}
