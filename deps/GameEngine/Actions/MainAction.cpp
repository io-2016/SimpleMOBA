#include "MainAction.hpp"
#include "QBox2D/QWorld.hpp"
#include <memory>

MainAction::MainAction(QWorld *w,
                       std::unique_ptr<FileActionResolver> resolver,
                       std::unique_ptr<MapEditorCallback> mapEditorCallback)
    : Action(w, w), m_mapEditor(this, std::move(mapEditorCallback)),
      m_fileAction(this, std::move(resolver)) {}

MainAction::~MainAction() {}

void MainAction::subActionEnabledChanged(SubAction *action) {
  Action::subActionEnabledChanged(action);

  if (!action->enabled()) {
    world()->setFocus(true);
  }
}
