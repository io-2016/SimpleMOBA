#include "AddChain.hpp"
#include "QBox2D/QWorld.hpp"
#include "QBox2D/QChain.hpp"
#include "Utility/Utility.hpp"
#include "MapEditor.hpp"

AddChain::AddChain(MapEditor* p) : SubAction(p), m_state(), m_object(this) {}

void AddChain::reset() {
  m_state = 0;
  m_pts.clear();
  update();
}

void AddChain::mousePressEvent(QMouseEvent*) {}

void AddChain::mouseReleaseEvent(QMouseEvent* event) {
  m_pts.push_back(mapFromScreen(event->pos()));
  m_state |= DirtyState::Points;
  update();
}

void AddChain::mouseMoveEvent(QMouseEvent* event) {
  m_mousePos = mapFromScreen(event->pos());

  m_state |= DirtyState::MousePos;
  update();
}

void AddChain::keyPressEvent(QKeyEvent* event) {
  if (event->key() == Qt::Key_Return) {
    QChain* chain = new QChain(world());
    chain->setVertices(m_pts);
    chain->initialize(world());

    m_state |= DirtyState::Finished;
    update();

    world()->itemSet()->addBody(chain);

    finished();
  }
}

SceneGraph::Node* AddChain::synchronize(SceneGraph::Node* n) {
  if (pts().size() == 0) {
    return nullptr;
  }

  Node* node = static_cast<Node*>(n);

  if (m_state & DirtyState::Points) {
    node = new Node(pts());
    m_state ^= DirtyState::Points;
  }

  if (m_state & DirtyState::MousePos) {
    node->setLastPoint(m_mousePos);
    m_state ^= DirtyState::MousePos;
  }

  if (m_state & DirtyState::Finished) {
    node = nullptr;

    m_pts.clear();
    m_state ^= DirtyState::Finished;
  }

  return node;
}

AddChain::Node::Node(std::vector<QPointF> pts)
    : m_geometry({{2, GL_FLOAT}}, pts.size() + 1, sizeof(QVector2D)) {
  setMaterial(&m_material);
  setGeometry(&m_geometry);

  m_geometry.setDrawingMode(GL_LINE_STRIP);
  QVector2D* array = m_geometry.vertexData<QVector2D>();
  for (size_t i = 0; i < pts.size(); i++)
    array[i] = QVector2D(pts[i].x(), pts[i].y());
  array[pts.size()] = array[pts.size() - 1];
  m_geometry.updateVertexData();
}

void AddChain::Node::setLastPoint(QPointF p) {
  QVector2D* array = m_geometry.vertexData<QVector2D>();
  array[m_geometry.vertexCount() - 1] = QVector2D(p.x(), p.y());
  m_geometry.updateVertexData();
}
