#include "Path.hpp"

#include <QVector2D>
#include <set>

#include "QBox2D/QFixture.hpp"
#include "QBox2D/QWorld.hpp"

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

    int distanceSquared(const Node &other) const {
      int dx = other.x - x;
      int dy = other.y - y;
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

  class CompareNodes {
   public:
    CompareNodes(Node target) : m_target(target) {}

    bool operator()(const Node &a, const Node &b) const {
      int alenSq;
      int blenSq;

      alenSq = a.distanceSquared(Node());
      blenSq = b.distanceSquared(Node());
      if (alenSq < blenSq) {
        return true;
      }
      if (blenSq < alenSq) {
        return false;
      }

      alenSq = a.distanceSquared(m_target);
      blenSq = b.distanceSquared(m_target);
      if (alenSq < blenSq) {
        return true;
      }
      if (blenSq < alenSq) {
        return false;
      }

      return a < b;
    }

   private:
    Node m_target;
  };

 public:
  Finder(QPointF a, QPointF b, QBody *skip, qreal radius, QWorld *w,
         int resolution)
      : m_priQueue(CompareNodes(Node(2 * resolution, 0))) {
    m_world = w;
    QVector2D base(b - a);
    while ((2 * (resolution - 1)) > radius) {
      resolution -= 1;
    }
    m_res = resolution;
    m_skip = skip;
    m_radius = radius;
    QVector2D baseTransposed(-base.y(), base.x());
    m_stepX = base / float(resolution * 2);
    m_stepY = QVector2D(-m_stepX.y(), m_stepX.x());
    m_origin = a;
    Node start;
    m_priQueue.insert(start);
    m_known[start] = {0.f, start};
    const int maxGraphNodeCount = 1024;
    while (dfs() && m_known.size() < maxGraphNodeCount)
      ;
    rebuildPath();
  }

  std::vector<QPointF> &path() { return m_path; }

 private:
  bool dfs() {
    if (m_priQueue.empty()) return false;
    auto p = *m_priQueue.begin();
    m_priQueue.erase(p);
    if (isEnd(p)) {
      return false;
    }
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
    auto fixtures = m_world->fixturesUnderPoint(nodeLocation(n), m_radius);
    for (auto f : fixtures) {
      if (!f->isSensor()) {
        if (f->body() == m_skip) {
          return true;
        }
        return false;
      }
    }
    return true;
  }

  bool nodeAcessibleFrom(const Node &tgt, const Node &src) {
    RaycastCallback cb;
    m_world->rayCast(&cb, nodeLocation(src), nodeLocation(tgt));
    for (auto f : cb.fixtures()) {
      if (!f->isSensor()) {
        if (f->body() == m_skip) {
          return true;
        }
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
  std::set<Node, CompareNodes> m_priQueue;
  std::vector<QPointF> m_path;
  QBody *m_skip;
  qreal m_radius;
};
}  // namespace

Path::Path(QPointF a, QPointF b, QBody *skip, qreal radius, QWorld *w) {
  build(a, b, skip, radius, w);
}

const std::vector<QPointF> &Path::points() const { return m_points; }

void Path::build(QPointF a, QPointF b, QBody *skip, qreal radius, QWorld *w) {
  const int pathfindingResulotion = 32;
  Finder fnd(a, b, skip, radius, w, pathfindingResulotion);
  m_points = fnd.path();
}
