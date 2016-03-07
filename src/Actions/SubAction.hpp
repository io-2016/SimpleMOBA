#ifndef SUBACTION_HPP
#define SUBACTION_HPP
#include "Action.hpp"

class MapEditor;

class SubAction : public Action {
 private:
  Action* m_parentAction;

 protected:
  void enabledChanged();

 public:
  SubAction(Action* parent, QWorld* w = nullptr);

  void finished();

  virtual QString name() const = 0;
};

#endif  // SUBACTION_HPP
