#include "Player.hpp"

#include "QBox2D/Fixture/Box2DBox.hpp"
#include "QBox2D/QWorld.hpp"

void Player::onStepped() {
  enqueueFunction(std::bind(&Player::onStepped, this));

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
  }
}

void Player::mousePressEvent(QMouseEvent* e) {
  QBody::mousePressEvent(e);
  if (e->buttons() & Qt::RightButton) {
    e->accept();
    m_target = world()->mapFromScreen(e->pos());
    m_going = true;
  }
}

Player::Player(Item* parent) : QBody(parent), m_going(false) {
  setBodyType(QBody::BodyType::Dynamic);

  auto box = std::make_unique<Box2DBox>();
  box->setSize(QSizeF(5, 5));
  box->setTextureSource(":/resources/crate.jpg");
  box->setDensity(0.5);

  addFixture(std::move(box));
  setPosition(QPointF(70, 950));
  setLinearDamping(5);
  setAngularDamping(5);
}

void Player::initialize(QWorld* w) {
  QBody::initialize(w);
  enqueueFunction(std::bind(&Player::onStepped, this));
}
