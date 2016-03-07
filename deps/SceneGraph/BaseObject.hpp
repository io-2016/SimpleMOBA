#ifndef BASEOBJECT_HPP
#define BASEOBJECT_HPP

namespace SceneGraph {

class BaseObject {
 private:
  BaseObject* m_parent;
  BaseObject* m_prev;
  BaseObject* m_next;
  BaseObject* m_firstChild;
  BaseObject* m_lastChild;

 public:
  BaseObject(BaseObject* parent = nullptr);
  virtual ~BaseObject();

  void appendChild(BaseObject* node);
  void removeChild(BaseObject* node);

  inline BaseObject* parent() const { return m_parent; }
  inline BaseObject* prev() const { return m_prev; }
  inline BaseObject* next() const { return m_next; }
  inline BaseObject* firstChild() const { return m_firstChild; }
  inline BaseObject* lastChild() const { return m_lastChild; }
};
}

#endif  // BASEOBJECT_HPP
