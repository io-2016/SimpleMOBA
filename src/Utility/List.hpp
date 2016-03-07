#ifndef LIST_HPP
#define LIST_HPP

template <class T>
class List {
 public:
  class Node {
   private:
    friend class List;

    Node* m_prev;
    Node* m_next;
    T m_data;

   public:
    Node(const T& data) : m_prev(), m_next(), m_data(data) {}

    Node* prev() const { return m_prev; }
    Node* next() const { return m_next; }
    T data() const { return m_data; }
  };

 private:
  Node* m_firstNode;
  Node* m_lastNode;

 public:
  List() : m_firstNode(), m_lastNode() {}

  ~List() { clear(); }

  void clear() {
    while (firstNode()) remove(firstNode());
  }

  void prepend(Node* node) {
    if (!m_firstNode)
      m_firstNode = m_lastNode = node;
    else {
      node->m_next = m_firstNode;
      m_firstNode->m_prev = node;

      m_firstNode = node;
    }
  }

  void append(Node* node) {
    if (!m_lastNode)
      m_lastNode = m_firstNode = node;
    else {
      node->m_prev = m_lastNode;
      m_lastNode->m_next = node;

      m_lastNode = node;
    }
  }

  void remove(Node* node) {
    if (node->m_prev) node->m_prev->m_next = node->m_next;
    if (node->m_next) node->m_next->m_prev = node->m_prev;

    if (m_firstNode == node) m_firstNode = node->m_next;
    if (m_lastNode == node) m_lastNode = node->m_prev;

    node->m_prev = node->m_next = nullptr;
  }

  Node* firstNode() const { return m_firstNode; }
};

#endif  // LIST_HPP
