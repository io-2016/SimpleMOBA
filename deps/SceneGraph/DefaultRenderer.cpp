#include "DefaultRenderer.hpp"
#include "Material.hpp"
#include "Shader.hpp"
#include "Node.hpp"
#include "Geometry.hpp"
#include <cassert>
#include <QElapsedTimer>
namespace SceneGraph {

DefaultRenderer::DefaultRenderer() : Renderer() { initializeOpenGLFunctions(); }

void DefaultRenderer::renderGeometryNode(GeometryNode* node,
                                         const RenderState& state) {
  Material* material = node->material();
  assert(material);
  Shader* shader = material->shader();
  assert(shader);

  if (!shader->initialized()) shader->initialize();

  shader->bind();
  shader->activate();

  shader->updateState(material, state);

  Geometry* g = node->geometry();
  assert(g);

  g->bind(shader->attributeLocation());
  if (g->indexCount())
    glDrawElements(g->drawingMode(), g->indexCount(), g->indexType(),
                   g->indexData());
  else
    glDrawArrays(g->drawingMode(), 0, g->vertexCount());
  g->release();

  shader->deactivate();
}

void DefaultRenderer::render() {
  glClearColor(1, 1, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Renderer::render();

  glActiveTexture(GL_TEXTURE0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
}
