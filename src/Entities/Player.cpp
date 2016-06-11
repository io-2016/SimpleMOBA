#include "Player.hpp"

#include "QBox2D/Fixture/Box2DBox.hpp"
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

void Player::mousePressEvent(QMouseEvent* e) {
  QBody::mousePressEvent(e);
  if (e->buttons() & Qt::RightButton) {
    e->accept();
    m_target = world()->mapFromScreen(e->pos());
    m_currentPath = std::make_unique<Path>(
        m_target, position(), this, 2.6, world());
    if (m_currentPath->points().empty()) m_currentPath = nullptr;
    m_currentPathPoint = 0;
    m_going = true;
  }
}

Player::Player(Item* parent)
    : QBody(parent), m_currentPathPoint(0), m_going(false) {
  setBodyType(QBody::BodyType::Dynamic);

  auto box = std::make_unique<Box2DBox>();
  box->setPosition(QPointF(-2.5, -2.5));
  box->setSize(QSizeF(5, 5));
  box->setPosition(QPointF(-2.5, -2.5));
  box->setTextureSource(":/resources/crate.jpg");

  addFixture(std::move(box));
  setPosition(QPointF(70, 950));
  setLinearDamping(5);
  setAngularDamping(5);
}

void Player::initialize(QWorld* w) {
  QBody::initialize(w);
  enqueueFunction(std::bind(&Player::onStepped, this));
}

bool Player::write(QJsonObject&) const {
  return false;
}
