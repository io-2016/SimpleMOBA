#ifndef QCHAIN_HPP
#define QCHAIN_HPP
#include "Geometry/Circle.hpp"
#include "QBox2D/QBody.hpp"

class QChain : public QBody {
private:
  std::vector<QPointF> m_vertices;

  void createChain();

public:
  QChain(SceneGraph::Item * = nullptr);
  ~QChain();

  const std::vector<QPointF> &vertices() const { return m_vertices; }
  void setVertices(const std::vector<QPointF> &v);

  void cutCircle(Circle);

  bool testPoint(const QPointF &point) const;

  bool read(const QJsonObject &);
  bool write(QJsonObject &) const;
};

#endif // QCHAIN_HPP
