#include "MapEditor.hpp"
#include "MainAction.hpp"

MapEditor::MapEditor(MainAction *item, std::unique_ptr<MapEditorCallback> call)
    : SubAction(item, item->world()), m_addChain(this), m_addBody(this),
      m_grabItem(this), m_deleteItem(this), m_bodyEdit(this),
      m_action({addChain(), addBody(), grabItem(), deleteItem(), bodyEdit()}),
      m_object(this), m_callback(std::move(call)) {
  for (Action *action : m_action)
    action->setWorld(world());
}

void MapEditor::enabledChanged() {
  SubAction::enabledChanged();
  if (m_callback) {
    m_callback->m_enabled = enabled();
    m_callback->onTriggered();
  }
}

MapEditorCallback::MapEditorCallback() : m_enabled() {}
