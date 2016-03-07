#ifndef BOX2DBOX_HPP
#define BOX2DBOX_HPP

#include "Fixture.hpp"
#include "Graphics/TexturedRectangle.hpp"

class Box2DBox : public Fixture {
 private:
  QSizeF m_size;
  TexturedRectangle m_texturedRectangle;
  TexturedRectangle m_normalMapRectangle;

 protected:
  b2Shape* createShape() const;

 public:
  explicit Box2DBox(SceneGraph::Item* = nullptr);

  inline QRectF textureRect() const {
    return m_texturedRectangle.textureRect();
  }
  void setTextureRect(QRectF);

  std::vector<QPointF> vertices() const;

  inline QSizeF size() const { return m_size; }
  void setSize(QSizeF);

  bool read(const QJsonObject&);
  bool write(QJsonObject&) const;
};

#endif  // BOX2DBOX_HPP
