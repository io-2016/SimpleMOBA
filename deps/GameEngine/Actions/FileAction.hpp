#ifndef FILEACTION_HPP
#define FILEACTION_HPP
#include "SubAction.hpp"
#include <memory>

class MainAction;
class FileAction;
class SaveMapAction;
class LoadMapAction;

class SaveMapActionObject : public ActionObject {
private:
  Q_OBJECT

public:
  SaveMapActionObject(SaveMapAction *);

  SaveMapAction *action() const;

  Q_INVOKABLE void dump(QString path);
};

class SaveMapAction : public SubAction {
private:
  friend class SaveMapActionObject;

  SaveMapActionObject m_object;
  FileAction* m_fileAction;

public:
  SaveMapAction(FileAction *);

  inline QString name() const { return "SaveMap"; }
};

class LoadMapActionObject : public ActionObject {
private:
  Q_OBJECT

public:
  LoadMapActionObject(LoadMapAction *);

  LoadMapAction *action() const;

  Q_INVOKABLE void load(QString);
};

class LoadMapAction : public SubAction {
private:
  friend class LoadMapActionObject;

  LoadMapActionObject m_object;
  FileAction* m_fileAction;

public:
  LoadMapAction(FileAction *);

  inline QString name() const { return "LoadMap"; }
};

class FileActionResolver {
public:
  virtual ~FileActionResolver();

  virtual void load(QString) const = 0;
  virtual void dump(QString) const = 0;
};

class FileAction : public SubAction {
private:
  ActionObject m_object;
  SaveMapAction m_saveMap;
  LoadMapAction m_loadMap;
  std::unique_ptr<FileActionResolver> m_resolver;

public:
  FileAction(MainAction *, std::unique_ptr<FileActionResolver>);
  ~FileAction();

  inline SaveMapAction *saveMapAction() { return &m_saveMap; }
  inline LoadMapAction *loadMapAction() { return &m_loadMap; }

  void load(QString) const;
  void dump(QString) const;

  inline QString name() const { return "FileAction"; }
};

#endif // FILEACTION_HPP
