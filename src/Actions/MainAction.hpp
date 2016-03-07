#ifndef MAINACTION_HPP
#define MAINACTION_HPP
#include "Action.hpp"
#include "MapEditor.hpp"
#include "FileAction.hpp"

class QWorld;

class MainAction : public Action {
 private:
  MapEditor m_mapEditor;
  FileAction m_fileAction;

 protected:
  void subActionEnabledChanged(SubAction*);

 public:
  MainAction(QWorld*);
  ~MainAction();

  inline MapEditor* mapEditor() { return &m_mapEditor; }
  inline FileAction* fileAction() { return &m_fileAction; }
};

#endif  // MAINACTION_HPP
