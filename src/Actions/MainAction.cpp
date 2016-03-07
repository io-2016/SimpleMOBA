#include "MainAction.hpp"
#include "Entities/World.hpp"
#include "Entities/ViewWorld.hpp"
#include "Entities/Player.hpp"

MainAction::MainAction(World* w)
    : Action(w, w), m_mapEditor(this), m_fileAction(this) {}

MainAction::~MainAction() {}

void MainAction::subActionEnabledChanged(SubAction* action) {
  Action::subActionEnabledChanged(action);
  ViewWorld* view = world()->view();

  if (action->enabled()) {
    view->setFocusedObject(nullptr);
    view->setFlickable(true);
  } else {
    if (world()->player())
      world()->player()->setFocus(true);
    else
      world()->setFocus(true);

    view->setFocusedObject(world()->player());
    view->setFlickable(false);
  }
}
