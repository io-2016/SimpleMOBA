#ifndef FILEACTION_HPP
#define FILEACTION_HPP
#include "SubAction.hpp"

class MainAction;
class FileAction;
class SaveMapAction;
class LoadMapAction;

class SaveMapActionObject : public ActionObject {
 private:
  Q_OBJECT

 public:
  SaveMapActionObject(SaveMapAction*);

  SaveMapAction* action() const;

  Q_INVOKABLE void dump(QString path);
};

class SaveMapAction : public SubAction {
 private:
  SaveMapActionObject m_object;

 public:
  SaveMapAction(FileAction*);

  inline QString name() const { return "SaveMap"; }
};

class LoadMapActionObject : public ActionObject {
 private:
  Q_OBJECT

 public:
  LoadMapActionObject(LoadMapAction*);

  LoadMapAction* action() const;

  Q_INVOKABLE void load(QString);
};

class LoadMapAction : public SubAction {
 private:
  LoadMapActionObject m_object;

 public:
  LoadMapAction(FileAction*);

  inline QString name() const { return "LoadMap"; }
};

class FileAction : public SubAction {
 private:
  ActionObject m_object;
  SaveMapAction m_saveMap;
  LoadMapAction m_loadMap;

 public:
  FileAction(MainAction*);
  ~FileAction();

  inline SaveMapAction* saveMapAction() { return &m_saveMap; }
  inline LoadMapAction* loadMapAction() { return &m_loadMap; }

  inline QString name() const { return "FileAction"; }
};

#endif  // FILEACTION_HPP
