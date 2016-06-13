#include "Player.hpp"

#include "Enemy.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/Fixture/Box2DCircle.hpp"
#include "QBox2D/QWorld.hpp"

#include "Utility/Path.hpp"

void Player::onStepped() {
  enqueueFunction(std::bind(&Player::onStepped, this));
  synchronize();

  if (m_going && m_currentPath) {
    assert(m_currentPathPoint < m_currentPath->points().size());
    QPointF p = m_currentPath->points()[m_currentPathPoint];
    QPointF d = (p - position());
    QVector2D vector(d.x(), d.y());
    if (vector.length() <= 1) {
      if (m_currentPathPoint + 1 >= m_currentPath->points().size()) {
        setLinearVelocity(QPointF(0, 0));
        m_going = false;
      } else {
        m_currentPathPoint++;
      }
      return;
    }
    vector.normalize();
    vector *= 40;
    setLinearVelocity(vector.toPointF());
  }
}

void Player::castSpell(const QPointF& location, int) {
  auto t = std::make_unique<Bullet>(world()->itemSet());
  t->setPosition(position());
  QPointF direction = location - position();
  direction = QVector2D(direction).normalized().toPointF();
  t->setDirection(direction);
  t->setSound(m_punchSound);
  t->setOwner(this);
  t->initialize(world());
  world()->itemSet()->addBody(std::move(t));
}

void Player::addPoints(int pts) { m_score += pts; }

void Player::mousePressEvent(QMouseEvent* e) {
  QBody::mousePressEvent(e);
  if (e->buttons() & Qt::RightButton) {
    e->accept();
    move(world()->mapFromScreen(e->pos()));
  }
}

Player::Player(Item* parent)
    : QBody(parent),
      m_currentPathPoint(),
      m_going(false),
      m_punchSound(std::make_shared<QSound>(":/resources/punch_sound.wav")),
      m_object(this),
      m_score() {
  setBodyType(QBody::BodyType::Dynamic);

  auto circle = std::make_unique<Box2DCircle>();
  circle->setRadius(2.5);
  circle->setFriction(0.0);
  circle->setDensity(1.0);
  circle->setPosition(QPointF(-2.5, -2.5));
  circle->setCategories(QFixture::Category2);
  circle->setCollidesWith(QFixture::Category1);
  circle->setRestitution(0.2);

  addFixture(std::move(circle));

  setPosition(QPointF(70, 950));
  setLinearDamping(5);
  setAngularDamping(5);
}

void Player::initialize(QWorld* w) {
  QBody::initialize(w);
  enqueueFunction(std::bind(&Player::onStepped, this));
}

bool Player::write(QJsonObject&) const { return false; }

void Player::setActiveSpell(std::string str) {
  if (m_activeSpell == str) return;
  m_activeSpell = str;
}

const std::string& Player::activeSpell() const { return m_activeSpell; }

void Bullet::onStepped() {
  setLinearVelocity(40 * m_direction);
  enqueueFunction(std::bind(&Bullet::onStepped, this));
}

void Bullet::beginContact(QFixture* f, b2Contact*) {
  if (f->isSensor()) return;
  if (dynamic_cast<Enemy*>(f->parent()))
    m_punchSound->play();
  else {
    m_destroyed = true;
    destroyLater();
  }
}

void Bullet::initialize(QWorld* w) {
  setBodyType(QBody::BodyType::Dynamic);

  auto f = std::make_unique<Box2DBox>();
  QSizeF size(10, 10);
  f->setSize(size);
  f->setPosition(-QPointF(0.5 * size.width(), 0.5 * size.height()));
  f->setTextureSource(":/resources/punch.png");
  f->setSensor(true);
  f->setCollidesWith(
      QFixture::CategoryFlag(QFixture::All & ~QFixture::Category2));
  addFixture(std::move(f));
  setRotation(180.0 / M_PI * atan2(-m_direction.x(), m_direction.y()) + 90.0);
  QBody::initialize(w);
  enqueueFunction(std::bind(&Bullet::onStepped, this));
}

void Player::move(QPointF p) {
  m_target = p;
  const qreal playerHalfRadius = 2.5;
  const qreal collisionMargin = sqrt(2.0) * playerHalfRadius;
  m_currentPath =
      std::make_unique<Path>(p, position(), this, collisionMargin, world());
  if (m_currentPath->points().empty()) m_currentPath = nullptr;
  m_currentPathPoint = 0;
  m_going = true;
}

void Bullet::setDirection(QPointF d) { m_direction = d; }

void Bullet::setSound(std::shared_ptr<QSound> sound) { m_punchSound = sound; }

<<<<<<< 25630cff855aff2e158c0c311783a600aff4062a
void PlayerObject::regen() {
  setHealth(health() + healthRegen());
  setMana(mana() + manaRegen());
}

PlayerObject::PlayerObject(Player* player)
    : m_player(player),
      m_health(42),
      m_maxHealth(100),
      m_healthRegen(3),
      m_mana(66),
      m_maxMana(100),
      m_manaRegen(5) {
  m_regenTimer.setInterval(1000);
  connect(&m_regenTimer, &QTimer::timeout, this, &PlayerObject::regen);
  m_regenTimer.start();
}

uint PlayerObject::health() const { return m_health; }

uint PlayerObject::maxHealth() const { return m_maxHealth; }

uint PlayerObject::healthRegen() const { return m_healthRegen; }

uint PlayerObject::mana() const { return m_mana; }

uint PlayerObject::maxMana() const { return m_maxMana; }

uint PlayerObject::manaRegen() const { return m_manaRegen; }

void PlayerObject::setHealth(uint h) {
  if (h < 0) {
    m_health = 0;
  } else if (h > maxHealth()) {
    m_health = maxHealth();
  } else {
    m_health = h;
  }
  emit healthChanged();
}

void PlayerObject::setMana(uint m) {
  if (m < 0) {
    m_mana = 0;
  } else if (m > maxMana()) {
    m_mana = maxMana();
  } else {
    m_mana = m;
  }
  emit manaChanged();
}

void Bullet::setOwner(Player* player) { m_owner = player; }
