#include "Chain.hpp"
#include "QBox2D/Fixture/Box2DChain.hpp"
#include "QBox2D/Fixture/Box2DEdge.hpp"
#include "QBox2D/QBody.hpp"
#include "QBox2D/QFixture.hpp"
#include "QBox2D/QWorld.hpp"
#include "Graphics/Primitives.hpp"
#include "Geometry/Circle.hpp"
#include "Geometry/Vector2d.hpp"
#include "Geometry/Functions.hpp"
#include "Entities/Gun.hpp"
#include "Entities/World.hpp"
#include "Utility/Utility.hpp"
#include <QPainterPath>
#include <QPolygonF>
#include <QJsonObject>
#include <QJsonArray>

static b2Vec2 tob2Vec2(QPointF p) { return b2Vec2(p.x(), p.y()); }

Chain::Chain(Item* parent) : QBody(parent) {}

Chain::~Chain() {}

void Chain::setVertices(const std::vector<QPointF>& v) {
  m_vertices = v;
  createChain();
}

void Chain::cutCircle(Circle circle) {
  if (m_vertices.size() == 0) return;
  circle.setCenter(circle.pos());

  QPainterPath cr;
  cr.addEllipse(circle.x - circle.r, circle.y - circle.r, 2 * circle.r,
                2 * circle.r);

  QPolygonF polygon;
  for (QPointF p : m_vertices) polygon.append(p);
  QPainterPath chain;
  chain.addPolygon(polygon);

  if (!chain.intersects(cr)) return;

  chain = chain.subtracted(cr);

  for (const QPolygonF& poly : chain.toSubpathPolygons()) {
    std::vector<Vector2d> pts(poly.begin(), poly.end() - 1);

    if (std::fabs(Geometry::area(pts.begin(), pts.end())) > 5.f) {
      Chain* chain = new Chain(world());
      chain->setVertices(std::vector<QPointF>(pts.begin(), pts.end()));
      chain->initializeLater(world());

      static_cast<World*>(world())->itemSet()->addBody(chain);
    }
  }

  m_vertices.clear();
  destroyLater();
}

bool Chain::testPoint(const QPointF& point) const {
  std::vector<Vector2d> vert(m_vertices.begin(), m_vertices.end() - 1);
  return Geometry::pointInPolygon(vert.begin(), vert.end(), Vector2d(point));
}

bool Chain::read(const QJsonObject& obj) {
  std::vector<QPointF> pts;
  QJsonArray array = obj["vertices"].toArray();
  for (int i = 0; i < array.size(); i++)
    pts.push_back(Utility::Json::toPoint(array[i].toObject()));
  setVertices(pts);

  return true;
}

bool Chain::write(QJsonObject& obj) const {
  obj["class"] = QString("Chain");

  QJsonArray array;
  std::vector<QPointF> pts = vertices();
  for (uint i = 0; i < pts.size(); i++)
    array.append(Utility::Json::toObject(pts[i]));
  obj["vertices"] = array;

  return true;
}

void Chain::createChain() {
  std::vector<Vector2d> tmp(m_vertices.begin(), m_vertices.end());
  tmp = Geometry::simplifyPolygon(tmp, 0.2);

  m_vertices = std::vector<QPointF>(tmp.begin(), tmp.end());
  std::vector<QPointF> pts = vertices();

  size_t it = pts.size() - 1, prev = it - 1, next = 1;
  for (size_t i = 0; i < pts.size(); i++) {
    QPointF vec = pts[i] - pts[it];
    QPointF vecp = pts[it] - pts[prev], vecn = pts[next] - pts[i];

    Box2DEdge* fixture = new Box2DEdge(this);
    fixture->setVisible(false);
    fixture->setShadowCaster(false);
    addFixture(fixture);

    b2EdgeShape& edge = fixture->edgeShape();
    edge.Set(tob2Vec2(pts[it]), tob2Vec2(pts[i]));

    if (QPointF::dotProduct(vec, vecp) > 0) {
      edge.m_hasVertex0 = true;
      edge.m_vertex0 = tob2Vec2(pts[prev]);
    }
    if (QPointF::dotProduct(vec, vecn) > 0) {
      edge.m_hasVertex3 = true;
      edge.m_vertex3 = tob2Vec2(pts[next]);
    }

    prev = it, it = i;
    next = next + 1 < pts.size() ? next + 1 : 0;
  }
  Box2DChain* chain = new Box2DChain;
  chain->setVertices(pts);
  chain->setShadowCaster(false);
  chain->setSensor(true);

  addFixture(chain);
}

void Chain::beginContact(QFixture* f, b2Contact*) {
  if (f->body()->type() != Bullet::key()) return;

  cutCircle(Circle(Vector2d(f->body()->worldCenter()), 5));
}
