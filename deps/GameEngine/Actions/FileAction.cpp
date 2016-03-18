#include "FileAction.hpp"
#include "MainAction.hpp"

FileAction::FileAction(MainAction *action,
                       std::unique_ptr<FileActionResolver> resolver)
    : SubAction(action, action->world()), m_object(this), m_saveMap(this),
      m_loadMap(this), m_resolver(std::move(resolver)) {}

FileAction::~FileAction() {}

void FileAction::load(QString path) const { m_resolver->load(path); }

void FileAction::dump(QString path) const { m_resolver->dump(path); }

SaveMapActionObject::SaveMapActionObject(SaveMapAction *action)
    : ActionObject(action) {}

SaveMapAction *SaveMapActionObject::action() const {
  return static_cast<SaveMapAction *>(ActionObject::action());
}

void SaveMapActionObject::dump(QString path) {
  action()->m_fileAction->dump(path);
}

SaveMapAction::SaveMapAction(FileAction *action)
    : SubAction(action, action->world()), m_object(this), m_fileAction(action) {
}

LoadMapActionObject::LoadMapActionObject(LoadMapAction *action)
    : ActionObject(action) {}

LoadMapAction *LoadMapActionObject::action() const {
  return static_cast<LoadMapAction *>(ActionObject::action());
}

void LoadMapActionObject::load(QString path) {
  action()->m_fileAction->load(path);
  action()->setFocus(true);
}

LoadMapAction::LoadMapAction(FileAction *action)
    : SubAction(action, action->world()), m_object(this), m_fileAction(action) {
}

FileActionResolver::~FileActionResolver() {}
