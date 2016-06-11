#include "ViewWorld.hpp"
#include "Entities/Player.hpp"
#include "Game.hpp"
#include "Utility/Window.hpp"

#include <QGuiApplication>
#include <QJsonObject>
#include <QThread>
#include <QTime>
#include <QTimer>

ViewWorld::ViewWorld(Game *game)
    : DisplayItem(game),
      m_world(this),
      m_game(game),
      m_background(world()->itemSet()),
      m_scrollRadius(5),
      m_timer(startTimer(1000 / 60)) {}

void ViewWorld::initialize() {
  m_world.initialize();
  m_world.setFocus(true);
}

bool ViewWorld::read(const QJsonObject &obj) {
  world()->read(obj["world"].toObject());

  setSize(QSizeF(obj["width"].toDouble(), obj["height"].toDouble()));
  setFactor(obj["scale"].toDouble());

  m_background.resetTransform();
  m_background.scale(size().width(), size().height());
  m_background.setSource(obj["background"].toString());

  int backgroundRectX = obj["backgroundRectX"].toInt();
  int backgroundRectY = obj["backgroundRectY"].toInt();

  m_background.setTextureRect(QRectF(0,0,backgroundRectX,backgroundRectY));

  return true;
}

bool ViewWorld::write(QJsonObject &obj) const {
  QJsonObject world_obj;
  world()->write(world_obj);
  obj["world"] = world_obj;

  obj["width"] = size().width();
  obj["height"] = size().height();
  obj["scale"] = factor();
  obj["background"] = m_background.source();
  obj["backgroundRectX"] = m_background.textureRect().width();
  obj["backgroundRectY"] = m_background.textureRect().height();

  return true;
}

void ViewWorld::visibleAreaChanged() {
  DisplayItem::visibleAreaChanged();

  game()->lightSystem()->visibleAreaChanged(visibleArea());
  world()->setVisibleRect(visibleArea());
  emit world()->object()->cameraLocationChanged();
}

void ViewWorld::mouseMoveEvent(QMouseEvent *event) {
  DisplayItem::mouseMoveEvent(event);

  const qreal move = 2.5;
  QPoint p = event->pos();
  QPointF vec;
  if (p.x() <= m_scrollRadius) vec.rx() = -move;
  if (p.x() >= window()->size().width() - m_scrollRadius) vec.rx() = move;
  if (p.y() <= m_scrollRadius) vec.ry() = -move;
  if (p.y() >= window()->size().height() - m_scrollRadius) vec.ry() = move;

  m_cameraVector = vec;
}

void ViewWorld::keyPressEvent(QKeyEvent *e) {
  DisplayItem::keyPressEvent(e);
  if (e->key() == Qt::Key_F1) {
    e->accept();
    setLookAt(world()->m_player->position());
    setLookAt(effectiveLookAt());
  }
}

void ViewWorld::timerEvent(QTimerEvent *event) {
  DisplayItem::timerEvent(event);
  if (event->timerId() == m_timer) {
    QPointF p = lookAt() + m_cameraVector;
    p.rx() = std::max(p.rx(), 0.0);
    p.rx() = std::min(p.rx(), size().width());
    p.ry() = std::max(p.ry(), 0.0);
    p.ry() = std::min(p.ry(), size().height());

    setLookAt(p);
    setLookAt(effectiveLookAt());
  }
}
