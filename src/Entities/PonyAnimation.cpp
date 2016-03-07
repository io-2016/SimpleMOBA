#include "PonyAnimation.hpp"
#include "Pony.hpp"

PonyAnimation::PonyAnimation(Pony* parent)
    : SpriteSequence(parent), m_owner(parent), m_flip(false) {}

void PonyAnimation::initialize(const char* source) {
  reset();

  const uint frameRate = 10;
  const uint width = 2880;
  const uint height = 1152;
  const uint frameWidth = 96;
  const uint frameHeight = 96;

  SpriteData data;
  data.m_image = source;
  data.m_frameRate = frameRate;
  data.m_frameWidth = (qreal)frameWidth / width;
  data.m_frameHeight = (qreal)frameHeight / height;

  data.m_name = "walking";
  data.m_next = data.m_name;
  data.m_frameCount = 6;
  data.m_frameX = data.m_frameWidth * 12.0;
  data.m_frameY = data.m_frameHeight * 3.0;
  addSprite(data);

  data.m_name = "prepareJump";
  data.m_next = "jumping";
  data.m_frameCount = 6;
  data.m_frameX = data.m_frameWidth * 0.0;
  data.m_frameY = data.m_frameHeight * 10.0;
  addSprite(data);

  data.m_name = "jumping";
  data.m_next = data.m_name;
  data.m_frameCount = 6;
  data.m_frameX = data.m_frameWidth * 12.0;
  data.m_frameY = data.m_frameHeight * 5.0;
  addSprite(data);

  data.m_name = "standStill";
  data.m_next = data.m_name;
  data.m_frameCount = 1;
  data.m_frameX = data.m_frameWidth * 2.0;
  data.m_frameY = data.m_frameHeight * 0.0;
  addSprite(data);

  data.m_name = "preparePunch";
  data.m_next = "punch";
  data.m_frameCount = 5;
  data.m_frameX = data.m_frameWidth * 0.0;
  data.m_frameY = data.m_frameHeight * 9.0;
  addSprite(data);

  data.m_name = "punch";
  data.m_next = "standStill";
  data.m_frameRate *= 0.5;
  data.m_frameCount = 1;
  data.m_frameX = data.m_frameWidth * 5.0;
  data.m_frameY = data.m_frameHeight * 9.0;
  addSprite(data);
}

void PonyAnimation::updateState(unsigned added, unsigned /*removed*/) {
  if (added & Creature::TurnedLeft) {
    if (!m_flip) {
      scale(-1, 1);
      translate(-1, 0);
      m_flip = true;
    }

  } else if (added & Creature::TurnedRight) {
    if (m_flip) {
      scale(-1, 1);
      translate(-1, 0);
      m_flip = false;
    }
  }

  if (strcmp(currentSprite(), "preparePunch") == 0)
    return;
  else if (strcmp(currentSprite(), "prepareJump") == 0)
    return;

  if (owner()->currentState() & Creature::OnGround) {
    if (owner()->currentState() & (Creature::GoingLeft | Creature::GoingRight))
      setCurrentSprite("walking");
    else
      setCurrentSprite("standStill");
  }
}

void PonyAnimation::currentSpriteChanged() {
  if (strcmp(currentSprite(), "punch") == 0)
    owner()->punch();
  else if (strcmp(currentSprite(), "jumping") == 0)
    owner()->jump();
}
