#ifndef BASEITEM_HPP
#define BASEITEM_HPP
#include "SceneGraph/Item.hpp"

class DisplayItem;

class BaseItem : public SceneGraph::Item {
 private:
  friend class DisplayItem;

  DisplayItem* m_displayItem;

 protected:
  void matrixChanged();

 public:
  explicit BaseItem(SceneGraph::Item* parent = nullptr);
  ~BaseItem();

  inline DisplayItem* displayItem() const { return m_displayItem; }
};

#endif  // BASEITEM_HPP
