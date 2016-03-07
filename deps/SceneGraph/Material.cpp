#include "Material.hpp"
#include "Renderer.hpp"
#include <cassert>

namespace SceneGraph {

Material::Material() {}

void ColorMaterial::ColorShader::initialize() {
  Shader::initialize();

  m_matrix = program()->uniformLocation("matrix");
  m_color = program()->uniformLocation("color");
}

const char* ColorMaterial::ColorShader::vertexShader() const {
  return GLSL(attribute vec4 position; uniform mat4 matrix;
              void main() { gl_Position = matrix * position; });
}

const char* ColorMaterial::ColorShader::fragmentShader() const {
  return GLSL(uniform vec4 color; void main() { gl_FragColor = color; });
}

std::vector<std::string> ColorMaterial::ColorShader::attribute() const {
  return {"position"};
}

void ColorMaterial::ColorShader::updateState(const Material* m,
                                             const RenderState& state) {
  const ColorMaterial* data = static_cast<const ColorMaterial*>(m);

  program()->setUniformValue(m_matrix, state.matrix());
  program()->setUniformValue(m_color, data->m_color);
}

void TextureMaterial::TextureShader::initialize() {
  Shader::initialize();

  initializeOpenGLFunctions();
  m_matrix = program()->uniformLocation("matrix");
  m_texture = program()->uniformLocation("texture");
}

const char* TextureMaterial::TextureShader::vertexShader() const {
  return GLSL(attribute vec4 position; attribute vec2 tcoord;
              uniform mat4 matrix; varying vec2 texcoord;

              void main() {
                texcoord = tcoord.xy;
                gl_Position = matrix * position;
              });
}

const char* TextureMaterial::TextureShader::fragmentShader() const {
  return GLSL(uniform sampler2D texture; varying vec2 texcoord;

              void main() {
                gl_FragColor = max(texture2D(texture, texcoord), vec4(0));
              });
}

std::vector<std::string> TextureMaterial::TextureShader::attribute() const {
  return {"position", "tcoord"};
}

void TextureMaterial::TextureShader::updateState(const Material* material,
                                                 const RenderState& state) {
  const TextureMaterial* m = static_cast<const TextureMaterial*>(material);
  assert(m->texture());

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, m->texture()->textureId());
  program()->setUniformValue(m_texture, 0);
  program()->setUniformValue(m_matrix, state.matrix());
}

void VertexColorMaterial::VertexColorShader::initialize() {
  Shader::initialize();

  m_matrix = program()->uniformLocation("matrix");
}

const char* VertexColorMaterial::VertexColorShader::vertexShader() const {
  return GLSL(attribute vec4 position; attribute vec4 color;
              uniform mat4 matrix; varying vec4 fcolor;

              void main() {
                fcolor = color;
                gl_Position = matrix * position;
              });
}

const char* VertexColorMaterial::VertexColorShader::fragmentShader() const {
  return GLSL(varying vec4 fcolor;

              void main() { gl_FragColor = fcolor; });
}

std::vector<std::string> VertexColorMaterial::VertexColorShader::attribute()
    const {
  return {"position", "color"};
}

void VertexColorMaterial::VertexColorShader::updateState(
    const Material*, const RenderState& state) {
  program()->setUniformValue(m_matrix, state.matrix());
}

TextureMaterial::TextureMaterial() : m_texture() {}
}
