#include "Hand.hpp"
#include "Player.hpp"
#include "World.hpp"

Hand::Hand(Player* owner)
    : SceneGraph::Item(owner),
      m_owner(owner),
      m_grabbedWeapon(),
      m_mouseJoint(),
      m_range() {}

Hand::~Hand() { dropWeapon(); }

void Hand::setGrabbedWeapon(Weapon* weapon) {
  if (m_grabbedWeapon == weapon) return;
  m_grabbedWeapon = weapon;

  if (owner() == owner()->world()->player()) {
    emit owner()->world()->object()->equippedWeaponChanged();
    emit owner()->world()->object()->bulletCountChanged();
  }
}

void Hand::setHandPosition(QPointF pos) {
  qreal len = std::sqrt(pos.x() * pos.x() + pos.y() * pos.y());

  if (len > range())
    pos = QPointF(pos.x() / len * range(), pos.y() / len * range());

  resetTransform();
  translate(pos.x(), pos.y());

  m_position = pos;

  updateGrabbedWeapon();
}

void Hand::grabWeapon(Weapon* weapon) {
  if (m_mouseJoint) return;

  weapon->setGrabber(this);
  setGrabbedWeapon(weapon);

  m_mouseJoint = new QMouseJoint(grabbedWeapon());
  m_mouseJoint->setTarget(grabbedWeapon()->position());
  m_mouseJoint->setMaxForce(1000 * grabbedWeapon()->body()->GetMass());
  m_mouseJoint->initialize();

  updateGrabbedWeapon();
  setRange(0.5 * range());
}

void Hand::dropWeapon() {
  if (m_mouseJoint) {
    delete m_mouseJoint;
    m_mouseJoint = nullptr;

    grabbedWeapon()->setFlip(1);
    grabbedWeapon()->setGrabber(nullptr);
    setGrabbedWeapon(nullptr);
    setRange(range() * 2);
  }
}

void Hand::updateGrabbedWeapon() {
  if (m_mouseJoint) {
    QPointF p = owner()->matrix() * position();
    if (p.x() - owner()->position().x() < 0)
      grabbedWeapon()->setFlip(-1);
    else
      grabbedWeapon()->setFlip(1);

    m_mouseJoint->setTarget(p);
  }
}
