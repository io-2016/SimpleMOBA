#ifndef PONY_HPP
#define PONY_HPP
#include "Creature.hpp"
#include "PonyAnimation.hpp"
#include "Lighting/StaticLight.hpp"

class Pony : public Creature {
 private:
  PonyAnimation m_body;
  PonyAnimation m_wings;

  std::string m_bodySource;
  std::string m_wingsSource;

  QSizeF m_size;

 protected:
  void currentStateChanged(unsigned added, unsigned removed);

 public:
  Pony(Item * = nullptr);
  void initialize(QWorld *);

  inline QSizeF size() const { return m_size; }
  inline void setSize(QSizeF s) { m_size = s; }

  void setCurrentSprite(const char *);

  void jumpRequested();
  void punchRequested();

  bool read(const QJsonObject &);
  bool write(QJsonObject &) const;
};

#endif  // PONY_HPP
