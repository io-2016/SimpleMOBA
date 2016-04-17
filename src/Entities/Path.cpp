#include "Path.hpp"

#include <QVector2D>
#include <set>

#include "World.hpp"


Path::Path(QPointF a, QPointF b, World *w) {
  const int resolution = 8;
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
  }
  
  typedef std::pair<int, int> point;
  point start(0, 0), end(resolution * 2, 0);
  
}

const std::vector<QPointF> &Path::points() const
{
  return m_points;
}
