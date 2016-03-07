#ifndef PONYANIMATION_HPP
#define PONYANIMATION_HPP
#include "Graphics/SpriteSequence.hpp"

class Creature;
class Pony;

class PonyAnimation : public SpriteSequence {
 private:
  friend class Pony;

  Pony* m_owner;
  std::string m_source;

  bool m_flip;

  void updateState(unsigned added, unsigned removed);

 protected:
  void currentSpriteChanged();

 public:
  explicit PonyAnimation(Pony* = nullptr);

  void initialize(const char* source);

  inline Pony* owner() const { return m_owner; }
};

#endif  // PONYANIMATION_HPP
