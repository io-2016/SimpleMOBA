#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP
#include "AddBody.hpp"
#include "AddChain.hpp"
#include "BodyEdit.hpp"
#include "DeleteItem.hpp"
#include "GrabItem.hpp"
#include "Utility/BaseItem.hpp"

class MainAction;

class MapEditorCallback : public Callback {
public:
  bool m_enabled;

  MapEditorCallback();
};

class MapEditor : public SubAction {
private:
  AddChain m_addChain;
  AddBody m_addBody;
  GrabItem m_grabItem;
  DeleteItem m_deleteItem;
  BodyEdit m_bodyEdit;

  std::vector<Action *> m_action;
  ActionObject m_object;

  std::unique_ptr<MapEditorCallback> m_callback;

protected:
  void enabledChanged();

public:
  MapEditor(MainAction *, std::unique_ptr<MapEditorCallback> call);

  inline AddChain *addChain() { return &m_addChain; }
  inline AddBody *addBody() { return &m_addBody; }
  inline GrabItem *grabItem() { return &m_grabItem; }
  inline DeleteItem *deleteItem() { return &m_deleteItem; }
  inline BodyEdit *bodyEdit() { return &m_bodyEdit; }

  inline QString name() const { return "MapEditor"; }
};

#endif // MAPEDITOR_HPP
