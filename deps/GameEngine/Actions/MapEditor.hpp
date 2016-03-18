#ifndef MAPEDITOR_HPP
#define MAPEDITOR_HPP
#include "AddChain.hpp"
#include "AddBody.hpp"
#include "GrabItem.hpp"
#include "DeleteItem.hpp"
#include "BodyEdit.hpp"
#include "Utility/BaseItem.hpp"

class MainAction;

class MapEditor : public SubAction {
 private:
  AddChain m_addChain;
  AddBody m_addBody;
  GrabItem m_grabItem;
  DeleteItem m_deleteItem;
  BodyEdit m_bodyEdit;

  std::vector<Action*> m_action;
  ActionObject m_object;

 protected:
 public:
  MapEditor(MainAction*);

  inline AddChain* addChain() { return &m_addChain; }
  inline AddBody* addBody() { return &m_addBody; }
  inline GrabItem* grabItem() { return &m_grabItem; }
  inline DeleteItem* deleteItem() { return &m_deleteItem; }
  inline BodyEdit* bodyEdit() { return &m_bodyEdit; }

  inline QString name() const { return "MapEditor"; }
};

#endif  // MAPEDITOR_HPP
