#ifndef TEXTUREDITEM_HPP
#define TEXTUREDITEM_HPP

#include "SceneGraph/Item.hpp"

class QOpenGLTexture;

class TexturedItem : public SceneGraph::Item {
 private:
  QString m_source;
  QSize m_textureSize;

 public:
  explicit TexturedItem(SceneGraph::Item* = nullptr);

  inline const QString& source() const { return m_source; }
  void setSource(const QString&);

  QOpenGLTexture* texture() const;
};

#endif  // TEXTUREDITEM_HPP
