#ifndef UTILITY_HPP
#define UTILITY_HPP
#include <QObject>
#include <random>

namespace Utility {

template <class T>
T* findAncestor(const QObject* obj) {
  while (obj && !qobject_cast<T*>(obj)) obj = obj->parent();

  return qobject_cast<T*>(obj);
}

template <class T>
T* findAncestor(QObject* obj) {
  while (obj && !qobject_cast<T*>(obj)) obj = obj->parent();

  return qobject_cast<T*>(obj);
}

namespace Json {
QJsonObject toObject(QRectF);
QRectF toRect(const QJsonObject&);

QJsonObject toObject(QPointF);
QPointF toPoint(const QJsonObject&);
}

class Random {
 private:
  std::random_device m_device;
  std::mt19937 m_rng;

  Random();

 public:
  static Random& instance();

  static qreal real(qreal a, qreal b);
};
}

namespace std {

template <class A, class B>
struct hash<std::pair<A*, B*>> {
  size_t operator()(const std::pair<A*, B*>& pair) const {
    return ((long long)pair.first << 32LL) + (long long)pair.second;
  }
};
}

#endif  // UTILITY_HPP
