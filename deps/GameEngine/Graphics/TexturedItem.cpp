#include "TexturedItem.hpp"
#include "SceneGraph/Window.hpp"

TexturedItem::TexturedItem(Item* parent) : SceneGraph::Item(parent) {
  setVisible(false);
}

void TexturedItem::setSource(const QString& path) {
  if (m_source != path) {
    m_source = path;
    if (!path.isEmpty())
      setVisible(true);
    else
      setVisible(false);
  }
}

QOpenGLTexture* TexturedItem::texture() const {
  return window()->texture(source().toStdString().c_str());
}
