#include "SubAction.hpp"
#include "MapEditor.hpp"

SubAction::SubAction(Action *parent, QWorld* w)
    : Action(parent, w), m_parentAction(parent) {}

void SubAction::finished() {
  Action::finished();
  m_parentAction->subActionFinished(this);
}

void SubAction::enabledChanged() {
  Action::enabledChanged();
  m_parentAction->subActionEnabledChanged(this);
}
