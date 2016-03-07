#include "FileAction.hpp"
#include "MainAction.hpp"
#include "Entities/World.hpp"
#include "Entities/ViewWorld.hpp"
#include "Entities/Game.hpp"

FileAction::FileAction(MainAction *action)
    : SubAction(action, action->world()),
      m_object(this),
      m_saveMap(this),
      m_loadMap(this) {}

FileAction::~FileAction() {}

SaveMapActionObject::SaveMapActionObject(SaveMapAction *action)
    : ActionObject(action) {}

SaveMapAction *SaveMapActionObject::action() const {
  return static_cast<SaveMapAction *>(ActionObject::action());
}

void SaveMapActionObject::dump(QString path) {
  action()->world()->view()->game()->dump(path);
}

SaveMapAction::SaveMapAction(FileAction *action)
    : SubAction(action, action->world()), m_object(this) {}

LoadMapActionObject::LoadMapActionObject(LoadMapAction *action)
    : ActionObject(action) {}

LoadMapAction *LoadMapActionObject::action() const {
  return static_cast<LoadMapAction *>(ActionObject::action());
}

void LoadMapActionObject::load(QString path) {
  action()->world()->view()->game()->load(path);
  action()->setFocus(true);
}

LoadMapAction::LoadMapAction(FileAction *action)
    : SubAction(action, action->world()), m_object(this) {}
