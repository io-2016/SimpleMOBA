#include "SpriteSequence.hpp"
#include <QTimerEvent>
#include <cassert>

SpriteSequence::SpriteSequence(Item* parent)
    : SceneGraph::Item(parent),
      m_texture(this),
      m_currentSprite(),
      m_currentFrame(),
      m_timerId(-1) {}

void SpriteSequence::reset() {
  m_texture.setSource("");
  m_data.clear();
  m_currentSprite = 0;
  m_currentFrame = 0;
  m_timerId = -1;
}

void SpriteSequence::visibleChanged() {
  if (m_timerId != -1) {
    killTimer(m_timerId);
    m_timerId = -1;
  }

  if (m_currentSprite == nullptr) return;

  if (visible()) {
    m_timerId = startTimer(1000.0 / m_currentSprite->m_frameRate);
  }
}

void SpriteSequence::timerEvent(QTimerEvent* t) {
  if (t->timerId() == m_timerId) {
    if (m_currentFrame >= m_currentSprite->m_frameCount) {
      m_currentFrame = 0;
      setCurrentSprite(m_currentSprite->m_next.c_str());
    } else {
      setFrame(m_currentSprite, m_currentFrame++);
    }
  }
}

void SpriteSequence::currentSpriteChanged() {}

void SpriteSequence::addSprite(const SpriteData& data) {
  m_data[data.m_name] = data;
}

void SpriteSequence::setCurrentSprite(const char* name) {
  if (!m_currentSprite || std::string(name) != m_currentSprite->m_name) {
    if (m_data.find(name) == m_data.end()) return;

    m_currentSprite = &m_data[name];
    m_texture.setSource(m_currentSprite->m_image.c_str());

    visibleChanged();

    currentSpriteChanged();
  }

  m_currentFrame = 0;
  setFrame(m_currentSprite, m_currentFrame++);
}

const char* SpriteSequence::currentSprite() const {
  return m_currentSprite ? m_currentSprite->m_name.c_str() : "";
}

void SpriteSequence::setFrame(const SpriteData* data, uint frame) {
  qreal x = data->m_frameX;
  qreal y = data->m_frameY;
  qreal w = data->m_frameWidth;
  qreal h = data->m_frameHeight;

  m_texture.setTextureRect(QRectF(x + w * frame, y, w, h));
}
