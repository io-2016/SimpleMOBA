#ifndef ITEMSET_HPP
#define ITEMSET_HPP
#include "SceneGraph/Item.hpp"
#include <unordered_set>

class World;
class QBody;

class ItemSet : public SceneGraph::Item {
 private:
  World* m_world;
  std::unordered_set<QBody*> m_body;

 public:
  ItemSet(World* world);
  ~ItemSet();

  void clear();
  bool contains(QBody*);

  void addBody(QBody*);
  void removeBody(QBody*);

  void write(QJsonObject&) const;
  void read(const QJsonObject&);
};

#endif  // ITEMSET_HPP
