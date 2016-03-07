#ifndef DELETEITEM_HPP
#define DELETEITEM_HPP
#include "SubAction.hpp"

class DeleteItem : public SubAction {
 protected:
  void mouseReleaseEvent(QMouseEvent *event);

  ActionObject m_object;

 public:
  DeleteItem(MapEditor *);

  inline QString name() const { return "DeleteItem"; }
};

#endif  // DELETEITEM_HPP
