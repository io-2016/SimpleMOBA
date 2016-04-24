#include "Path.hpp"

#include <QVector2D>
#include <set>

#include "QBox2D/QWorld.hpp"
#include "QBox2D/QFixture.hpp"

namespace {
class Finder {
  struct Node {
    Node(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator<(const Node &other) const {
      if (x < other.x) return true;
      if (other.x < x) return false;
      return y < other.y;
    }

    bool operator==(const Node &other) const {
      if (x != other.x || y != other.y) return false;
      return true;
    }

    Node operator+(const std::pair<int, int> &p) const {
      return Node(x + p.first, y + p.second);
    }

    float distanceSquared(const Node &other) const {
      float dx = other.x - x;
      float dy = other.y - y;
      return dx * dx + dy * dy;
    }

    int x, y;
  };

  class RaycastCallback : public b2RayCastCallback {
   public:
    RaycastCallback() = default;

    float ReportFixture(b2Fixture *fixture, const b2Vec2 &, const b2Vec2 &,
                        float32) override {
      m_fixtures.push_back(QFixture::toQFixture(fixture));
      return 1.f;
    }

    const std::vector<QFixture *> &fixtures() const { return m_fixtures; }

   private:
    std::vector<QFixture *> m_fixtures;
  };

 public:
  Finder(QPointF a, QPointF b, QWorld *w, int res = 8) {
    m_world = w;
    m_res = res;
    QVector2D base(b - a);
    QVector2D baseTransposed(-base.y(), base.x());
    m_stepX = base / float(res * 2);
    m_stepY = QVector2D(-m_stepX.y(), m_stepX.x());
    m_origin = a;
    Node start;
    if (nodeUnobstructed(start)) {
      m_priQueue.insert(start);
      m_known[start] = {0.f, start};
      while (bfs())
        ;
      rebuildPath();
    }
  }

  std::vector<QPointF> &path() { return m_path; }

 private:
  bool bfs() {
    if (m_priQueue.empty()) return false;
    auto p = *m_priQueue.begin();
    m_priQueue.erase(p);
    const std::vector<std::pair<int, int>> offsets = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (auto offset : offsets) {
      Node tmp = p + offset;
      if (nodeAcessibleFrom(tmp, p)) {
        float dist = tmp.distanceSquared(p) + m_known[p].first;
        if (m_known.find(tmp) == m_known.end()) {
          m_known[tmp] = {dist, p};
          m_priQueue.insert(tmp);
        } else {
          if (dist < m_known[tmp].first) {
            m_known[tmp] = {dist, p};
          }
        }
      }
    }
    return true;
  }

  void rebuildPath() {
    Node current(2 * m_res, 0);
    if (m_known.find(current) == m_known.end()) return;
    while (true) {
      m_path.push_back(nodeLocation(current));
      if (m_known.find(current) == m_known.end()) break;
      Node next = m_known[current].second;
      if (next == current) break;
      current = next;
    }
  }

  QPointF nodeLocation(const Node &n) const {
    return m_origin + (m_stepX * float(n.x) + m_stepY * float(n.y)).toPointF();
  }

  bool nodeUnobstructed(const Node &n) const {
    auto fixtures = m_world->fixturesUnderPoint(nodeLocation(n));
    for (auto f : fixtures) {
      if (!f->isSensor()) {
        return false;
      }
    }
    return true;
  }

  bool nodeAcessibleFrom(const Node &tgt, const Node &src) {
    if (!nodeUnobstructed(tgt)) return false;
    if (abs(tgt.x) > m_res * 4 || abs(tgt.y) > m_res * 4) return false;
    RaycastCallback cb;
    m_world->rayCast(&cb, nodeLocation(src), nodeLocation(tgt));
    for (auto f : cb.fixtures()) {
      if (!f->isSensor()) {
        return false;
      }
    }
    return true;
  }

  bool isEnd(const Node &n) const { return n == Node(m_res * 2, 0); }

  QWorld *m_world;
  int m_res;
  QVector2D m_stepX, m_stepY;
  QPointF m_origin;
  std::map<Node, std::pair<float, Node>> m_known;
  std::set<Node> m_priQueue;
  std::vector<QPointF> m_path;
};
}  // namespace

Path::Path(QPointF a, QPointF b, QWorld* w) {
  Finder fnd(a, b, w, 8);
  m_points = fnd.path();
}

const std::vector<QPointF> &Path::points() const { return m_points; }

QPointF Path::at(float) const {
  // TODO
  return QPointF();
}

float Path::length() const {
  // TODO
  return 0.f;
}
