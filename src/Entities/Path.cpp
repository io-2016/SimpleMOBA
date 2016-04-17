#include "Path.hpp"

#include <QVector2D>
#include <set>

#include "World.hpp"

class Finder {
  struct node {
    node(int x = 0, int y = 0) : x(x), y(y) {}

    bool operator<(const node &other) const {
      if (x < other.x) return true;
      if (other.x < x) return false;
      return y < other.y;
    }

    bool operator==(const node &other) const {
      if (x != other.x || y != other.y) return false;
      return true;
    }

    node operator+(const std::pair<int, int> &p) const {
      return node(x + p.first, y + p.second);
    }

    float distanceSquared(const node &other) const {
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
  Finder(QPointF a, QPointF b, World *w, int res = 8) {
    m_world = w;
    m_res = res;
    QVector2D base(b - a);
    QVector2D baseTransposed(-base.y(), base.x());
    m_stepX = base / float(res * 2);
    m_stepY = QVector2D(-m_stepX.y(), m_stepX.x());
    m_origin = a;
    node start;
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
    std::vector<std::pair<int, int>> offsets = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};
    for (auto offset : offsets) {
      node tmp = p + offset;
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
    node current(2 * m_res, 0);
    if (m_known.find(current) == m_known.end()) return;
    while (true) {
      m_path.push_back(nodeLocation(current));
      if (m_known.find(current) == m_known.end()) break;
      node next = m_known[current].second;
      if (next == current) break;
      current = next;
    }
  }

  QPointF nodeLocation(const node &n) const {
    return m_origin + (m_stepX * float(n.x) + m_stepY * float(n.y)).toPointF();
  }

  bool nodeUnobstructed(const node &n) const {
    auto fixtures = m_world->fixturesUnderPoint(nodeLocation(n));
    for (auto f : fixtures) {
      if (!f->isSensor()) {
        return false;
      }
    }
    return true;
  }

  bool nodeAcessibleFrom(const node &tgt, const node &src) {
    // qDebug() << "RC from (" << src.x << src.y << ") to (" << tgt.x << tgt.y
    // << ")" << nodeLocation(src) << nodeLocation(tgt);
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

  bool isEnd(const node &n) const { return n == node(m_res * 2, 0); }

  World *m_world;
  int m_res;
  QVector2D m_stepX, m_stepY;
  QPointF m_origin;
  std::map<node, std::pair<float, node>> m_known;
  std::set<node> m_priQueue;
  std::vector<QPointF> m_path;
};

Path::Path(QPointF a, QPointF b, World *w) {
  /*const int resolution = 8;
  QVector2D base(b - a);
  QVector2D baseTransposed(-base.y(), base.x());
  QVector2D stepX = base / float(resolution * 2);
  QVector2D stepY(-stepX.y(), stepX.x());

  QPointF origin((QVector2D(a) - base / 2.f - baseTransposed / 2.f).toPointF());

  m_points.push_back(a + QPointF(1.f, 1.f));
  m_points.push_back(b + QPointF(1.f, 1.f));
  for (int i = 0; i < resolution * 2 + 1; ++i) {
    for (int j = 0; j < resolution * 4 + 1; ++j) {
      QPointF pnt = origin + (stepX * float(j) + stepY * float(i)).toPointF();
      auto fixtures = w->fixturesUnderPoint(pnt);
      bool any = false;
      for (auto f : fixtures) {
        if (!f->isSensor()) {
          any = true;
        }
      }
      if (!any) {
        m_points.push_back(pnt);
      }
    }
  }*/
  Finder fnd(a, b, w, 8);
  m_points = fnd.path();
}

const std::vector<QPointF> &Path::points() const { return m_points; }
