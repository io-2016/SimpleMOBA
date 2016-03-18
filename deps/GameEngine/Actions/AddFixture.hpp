#ifndef ADDFIXTURE_HPP
#define ADDFIXTURE_HPP
#include "SubAction.hpp"

class QFixture;

class AddFixture : public SubAction {
 public:
  AddFixture(Action*);
  virtual QFixture* fixture() const = 0;
};

#endif  // ADDFIXTURE_HPP
