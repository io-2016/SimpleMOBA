#ifndef DISPLAYITEM_HPP
#define DISPLAYITEM_HPP

#include "SceneGraph/Item.hpp"

class BaseItem;
class DisplayItem;
class QWheelEvent;
class QHoverEvent;

class DisplayItemFrame : public SceneGraph::Item {
 private:
  friend class DisplayItem;

  DisplayItem *m_displayItem;
  QSizeF m_size;

 protected:
  virtual void sizeChanged();

 public:
  explicit DisplayItemFrame(SceneGraph::Item * = nullptr);
  ~DisplayItemFrame();

  inline QSizeF size() const { return m_size; }
  void setSize(QSizeF);
};

class DisplayItem : public SceneGraph::Item {
 private:
  friend class DisplayItemFrame;
  friend class BaseItem;

  DisplayItemFrame *m_frame;
  BaseItem *m_focusedObject;

  QRectF m_visibleArea;
  QPointF m_effectiveLookAt;
  QPointF m_lookAt;
  bool m_flickable;
  bool m_buttonDown;

  QPointF m_startPoint;
  qreal m_distance;

  QSizeF m_size;
  qreal m_factor;

  void focusedObjectPositionChanged();

  void returnLookAtToBounds();
  void returnScaleToBounds();
  void updateVisibleArea();
  void setVisibleArea(QRectF);

 protected:
  void wheelEvent(QWheelEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void touchEvent(QTouchEvent *event);

  virtual void visibleAreaChanged();

 public:
  explicit DisplayItem(DisplayItemFrame *parent = nullptr);
  ~DisplayItem();

  inline QSizeF size() const { return m_size; }
  void setSize(QSizeF);

  inline qreal factor() const { return m_factor; }
  void setFactor(qreal);

  inline QRectF visibleArea() const { return m_visibleArea; }

  inline QPointF lookAt() const { return m_lookAt; }
  void setLookAt(QPointF);

  inline QPointF effectiveLookAt() const { return m_effectiveLookAt; }
  void setEffectiveLookAt(QPointF);

  inline BaseItem *focusedObject() const { return m_focusedObject; }
  void setFocusedObject(BaseItem *);

  inline bool flickable() const { return m_flickable; }
  void setFlickable(bool);
};

#endif  // DISPLAYITEM_HPP
