#include "BaseItem.hpp"
#include "DisplayItem.hpp"

BaseItem::BaseItem(Item* parent) : SceneGraph::Item(parent), m_displayItem() {}

BaseItem::~BaseItem() {
  if (displayItem()) displayItem()->setFocusedObject(nullptr);
}

void BaseItem::matrixChanged() {
  if (displayItem()) displayItem()->focusedObjectPositionChanged();
}
