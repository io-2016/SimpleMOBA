#include "Enemy.hpp"

#include <QJsonObject>
#include "Player.hpp"
#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/QWorld.hpp"

void Enemy::onStepped() {
  enqueueFunction(std::bind(&Enemy::onStepped, this));
  synchronize();

  if (m_going) {
    QPointF d = (m_target - position());
    QVector2D vector(d.x(), d.y());
    if (vector.length() <= 1) {
      setLinearVelocity(QPointF(0, 0));
      m_going = false;
      return;
    }
    vector.normalize();
    vector *= 40;
    setLinearVelocity(vector.toPointF());

    QVector2D diff(m_prev - position());
    if (diff.length() <= 0.01) {
      m_going = false;
    }

    m_prev = QPointF(position().x(), position().y());
  } else {
    if (m_initialized) {
      randomWalk();
    }
  }
}

Enemy::Enemy(Item* parent) : QBody(parent), m_going(false), m_hitpoints(100) {
  setBodyType(QBody::BodyType::Dynamic);

  auto box = std::make_unique<Box2DBox>();
  box->setSize(QSizeF(5, 5));
  box->setPosition(QPointF(-2.5, -2.5));
  box->setTextureSource(":/resources/crate.jpg");

  addFixture(std::move(box));
  setLinearDamping(5);
  setAngularDamping(5);

  m_initialized = false;
}

void Enemy::randomWalk() {
  if (!m_going) {
    int move_x = (rand() % 2 ? 1 : -1) * (rand() % int(m_radius + 1));
    int move_y = (rand() % 2 ? 1 : -1) * (rand() % int(m_radius + 1));
    qreal x = m_center.x();
    qreal y = m_center.y();

    m_target = QPointF(x + move_x, y + move_y);
    m_going = true;
  }
}

void Enemy::mousePressEvent(QMouseEvent* e) {
  QBody::mousePressEvent(e);
  if (e->buttons() & Qt::RightButton) {
    e->accept();
    m_target = world()->mapFromScreen(e->pos());
    m_going = true;
  }
}

void Enemy::beginContact(QFixture* other, b2Contact*) {
  if (Bullet* bullet = dynamic_cast<Bullet*>(other->parent())) {
    if (bullet->destroyed()) return;
    bullet->setDestroyed(true);
    bullet->destroyLater();
    m_hitpoints -= 40;
    if (m_hitpoints <= 0) destroyLater();
  } /*else if (Player* player = dynamic_cast<Player*>(other->parent())) {
    qDebug() << "player hit";
  } */
}

void Enemy::initialize(QWorld* w) {
  QBody::initialize(w);
  enqueueFunction(std::bind(&Enemy::onStepped, this));
}

bool Enemy::read(const QJsonObject& obj) {
  QBody::read(obj);
  m_radius = obj["radius"].toDouble();
  m_center = QPoint(position().x(), position().y());
  m_initialized = true;
  return true;
}

void Enemy::setRadius(qreal radius) { m_radius = radius; }

bool Enemy::write(QJsonObject& obj) const {
  QBody::write(obj);
  obj["radius"] = m_radius;
  obj["class"] = "Enemy";
  return true;
}
