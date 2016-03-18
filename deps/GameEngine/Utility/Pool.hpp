#ifndef POOL_HPP
#define POOL_HPP
#include <unordered_map>

template <class Value, class Key = std::string>
class Pool {
 private:
  std::unordered_map<Key, Value> m_pool;

  Pool() {}

 public:
  static Pool* instance() {
    static Pool<Value, Key> pool;
    return &pool;
  }

  Value get(const Key& key) const {
    auto it = m_pool.find(key);
    return it != m_pool.end() ? it->second : Value();
  }

  void insert(const Key& key, const Value& value) { m_pool[key] = value; }

  typename std::unordered_map<Key, Value>::const_iterator begin() const {
    return m_pool.begin();
  }

  typename std::unordered_map<Key, Value>::const_iterator end() const {
    return m_pool.end();
  }

  void clear() { m_pool.clear(); }
};

#endif  // POOL_HPP
